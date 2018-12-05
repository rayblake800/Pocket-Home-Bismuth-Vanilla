#define APPMENU_IMPLEMENTATION
#include "AppMenu/Data/DesktopEntry/EntryData.h"
#include "DesktopEntry/Loader.h"
#include "DesktopEntry/Exceptions/FormatError.h"
#include "DesktopEntry/Exceptions/FileError.h"
#include "Utils.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "AppMenu::EntryData";

/* Localized text value keys: */
static const juce::Identifier removeLinkToTextKey  = "removeLinkTo";
static const juce::Identifier questionMarkTextKey  = "questionMark";
static const juce::Identifier willHideEntryTextKey = "willHideEntry";
static const juce::Identifier editAppTextKey       = "editApp";

/*
 * Creates menu item data from a desktop entry. 
 */
AppMenu::EntryData::EntryData(const DesktopEntry::EntryFile& desktopEntry) :
    desktopEntry(desktopEntry), Locale::TextUser(localeClassKey) { }

/*
 * Gets the menu item's displayed title.
 */
juce::String AppMenu::EntryData::getTitle() const
{
    return desktopEntry.getName();
}

/*
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String AppMenu::EntryData::getIconName() const
{
    return desktopEntry.getIcon();
}

/*
 * Gets the menu item's application launch command.
 */
juce::String AppMenu::EntryData::getCommand() const
{
    return desktopEntry.getExec();
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool AppMenu::EntryData::getLaunchedInTerm() const
{
    return desktopEntry.getLaunchedInTerm();
}

/*
 * Gets the application categories connected to this menu item.
 */
juce::StringArray AppMenu::EntryData::getCategories() const
{
    return desktopEntry.getCategories();
}

/*
 * Gets the unique desktop file ID tied to this menu item.
 */
juce::String AppMenu::EntryData::getID() const
{
    return desktopEntry.getDesktopFileID();
}

/*
 * Sets the menu item's displayed title.
 */
void AppMenu::EntryData::setTitle(const juce::String& title)
{
    try
    {
        desktopEntry.setName(title);
        signalDataChanged(DataField::title);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__
                << ": Invalid title " << title);
    }
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenu::EntryData::setIconName(const juce::String& iconName)
{
    try
    {
        desktopEntry.setIcon(iconName);
        signalDataChanged(DataField::icon);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__
                << ": Invalid icon " << iconName);
    }
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenu::EntryData::setCommand(const juce::String& newCommand)
{
    try
    {
        desktopEntry.setExec(newCommand);
        signalDataChanged(DataField::command);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__
                << ": Invalid launch command " << newCommand);
    }
}

/*
 * Sets if this menu item runs its command in a new terminal window. 
 */
void AppMenu::EntryData::setLaunchedInTerm(const bool termLaunch)
{
    desktopEntry.setLaunchedInTerm(termLaunch);
    signalDataChanged(DataField::termLaunchOption);
}

/*
 * Sets the application categories connected to this menu item.
 */
void AppMenu::EntryData::setCategories(const juce::StringArray& categories)
{
    try
    {
        desktopEntry.setCategories(categories);
        signalDataChanged(DataField::categories);
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__
                << ": Invalid categories " << categories.joinIntoString(";"));
    }
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int AppMenu::EntryData::getMovableChildCount() const
{
    return 0;
}

/*
 * Checks if this menu item could be moved within its folder.
 */
bool AppMenu::EntryData::isMovable() const 
{
    return false;
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenu::EntryData::saveChanges()
{
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntry::FileError e)
    {
        DBG("EntryData::" << __func__ << ": Failed to write changes:"
                << e.what());
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__ << ": Failed to write changes:"
                << e.what());
    }
    DesktopEntry::Loader entryLoader;
    entryLoader.scanForChanges();
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String AppMenu::EntryData::getConfirmDeleteTitle() const
{
    return localeText(removeLinkToTextKey) 
            + getTitle() + localeText(questionMarkTextKey);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String AppMenu::EntryData::getConfirmDeleteMessage() const
{
    return localeText(willHideEntryTextKey);
}

/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String AppMenu::EntryData::getEditorTitle() const
{
    return localeText(editAppTextKey);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool AppMenu::EntryData::isEditable(const DataField dataField) const
{
    return true;
}

/*
 * Sets this menu item's desktop entry as hidden for the current user.
 */
void AppMenu::EntryData::deleteFromSource()
{
    desktopEntry.setIfDisplayed(false);
    try
    {
        desktopEntry.writeFile();
        DesktopEntry::Loader entryLoader;
        entryLoader.scanForChanges();
    }
    catch(DesktopEntry::FileError e)
    {
        DBG("EntryData::" << __func__
                << ": Failed to hide entry:" << e.what());
    }
    catch(DesktopEntry::FormatError e)
    {
        DBG("EntryData::" << __func__
                << ": Failed to hide entry:" << e.what());
    }
}
