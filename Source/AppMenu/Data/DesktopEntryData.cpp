#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "DesktopEntry/FormatError.h"
#include "DesktopEntry/FileError.h"
#include "AppMenu/Data/DesktopEntryData.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "AppMenu::DesktopEntryData";

/* Localized text value keys: */
static const juce::Identifier removeLinkToTextKey  = "removeLinkTo";
static const juce::Identifier questionMarkTextKey  = "questionMark";
static const juce::Identifier willHideEntryTextKey = "willHideEntry";
static const juce::Identifier editAppTextKey       = "editApp";

/*
 * Creates menu item data from a desktop entry. 
 */
AppMenu::DesktopEntryData::DesktopEntryData
(const DesktopEntry::EntryFile& desktopEntry) :
desktopEntry(desktopEntry), Locale::TextUser(localeClassKey) { }

/*
 * Gets the menu item's displayed title.
 */
juce::String AppMenu::DesktopEntryData::getTitle() const
{
    return desktopEntry.getName();
}

/*
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String AppMenu::DesktopEntryData::getIconName() const
{
    return desktopEntry.getIcon();
}

/*
 * Gets the menu item's application launch command.
 */
juce::String AppMenu::DesktopEntryData::getCommand() const
{
    return desktopEntry.getExec();
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool AppMenu::DesktopEntryData::getLaunchedInTerm() const
{
    return desktopEntry.getLaunchedInTerm();
}

/*
 * Gets the application categories connected to this menu item.
 */
juce::StringArray AppMenu::DesktopEntryData::getCategories() const
{
    return desktopEntry.getCategories();
}

/*
 * Gets the unique desktop file ID tied to this menu item.
 */
juce::String AppMenu::DesktopEntryData::getID() const
{
    return desktopEntry.getDesktopFileID();
}

/*
 * Sets the menu item's displayed title.
 */
void AppMenu::DesktopEntryData::setTitle(const juce::String& title)
{
    try
    {
        desktopEntry.setName(title);
        signalDataChanged(DataField::title);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__
                << ": Invalid title " << title);
    }
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenu::DesktopEntryData::setIconName(const juce::String& iconName)
{
    try
    {
        desktopEntry.setIcon(iconName);
        signalDataChanged(DataField::icon);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__
                << ": Invalid icon " << iconName);
    }
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenu::DesktopEntryData::setCommand(const juce::String& newCommand)
{
    try
    {
        desktopEntry.setExec(newCommand);
        signalDataChanged(DataField::command);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__
                << ": Invalid launch command " << newCommand);
    }
}

/*
 * Sets if this menu item runs its command in a new terminal window. */
void AppMenu::DesktopEntryData::setLaunchedInTerm(const bool termLaunch)
{
    desktopEntry.setLaunchedInTerm(termLaunch);
    signalDataChanged(DataField::termLaunchOption);
}

/*
 * Sets the application categories connected to this menu item.
 */
void AppMenu::DesktopEntryData::setCategories
(const juce::StringArray& categories)
{
    try
    {
        desktopEntry.setCategories(categories);
        signalDataChanged(DataField::categories);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__
                << ": Invalid categories " << categories.joinIntoString(";"));
    }
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int AppMenu::DesktopEntryData::getMovableChildCount() const
{
    return 0;
}

/*
 * Checks if this menu item could be moved within its folder.
 */
bool AppMenu::DesktopEntryData::isMovable() const 
{
    return false;
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenu::DesktopEntryData::saveChanges()
{
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntry::FileError e)
    {
        DBG("DesktopEntryData::" << __func__ << ": Failed to write changes:"
                << e.what());
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__ << ": Failed to write changes:"
                                << e.what());
    }
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String AppMenu::DesktopEntryData::getConfirmDeleteTitle() const
{
    return localeText(removeLinkToTextKey) 
            + getTitle() + localeText(questionMarkTextKey);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String AppMenu::DesktopEntryData::getConfirmDeleteMessage() const
{
    return localeText(willHideEntryTextKey);
}

/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String AppMenu::DesktopEntryData::getEditorTitle() const
{
    return localeText(editAppTextKey);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool AppMenu::DesktopEntryData::isEditable(const DataField dataField) const
{
    return true;
}

/*
 * Sets this menu item's desktop entry as hidden for the current user.
 */
void AppMenu::DesktopEntryData::deleteFromSource()
{
    desktopEntry.setIfDisplayed(false);
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntry::FileError e)
    {
        DBG("DesktopEntryData::" << __func__ 
                << ": Failed to hide entry:" << e.what());
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("DesktopEntryData::" << __func__ 
                << ": Failed to hide entry:" << e.what());
    }
}
