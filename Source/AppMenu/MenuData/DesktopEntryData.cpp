#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryData.h"

//localized text keys: 
static const constexpr char * remove_link_to = "remove_link_to";
static const constexpr char * question_mark = "question_mark";
static const constexpr char * will_hide = "will_hide";
static const constexpr char * edit_app = "edit_app";

AppMenu::DesktopEntryData::DesktopEntryData(const DesktopEntry& desktopEntry) :
desktopEntry(desktopEntry), Localized("AppMenu::DesktopEntryData") { }

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
 * Sets the menu item's displayed title.
 */
void AppMenu::DesktopEntryData::setTitle(const juce::String& title)
{
    try
    {
        desktopEntry.setName(title);
        signalDataChanged(DataField::title);
    }
    catch(DesktopEntryFormatError e)
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
    catch(DesktopEntryFormatError e)
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
    catch(DesktopEntryFormatError e)
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
void AppMenu::DesktopEntryData::setCategories(const juce::StringArray& categories)
{
    try
    {
        desktopEntry.setCategories(categories);
        signalDataChanged(DataField::categories);
    }
    catch(DesktopEntryFormatError e)
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
    catch(DesktopEntryFileError e)
    {
        DBG("DesktopEntryData::" << __func__ << ": Failed to write changes:"
                << e.what());
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryData::" << __func__ << ": Failed to write changes:"
                                << e.what());
    }
}

/*
 * Gets an appropriate title to use for a deletion confirmation 
 *         window.
 */
juce::String AppMenu::DesktopEntryData::getConfirmDeleteTitle() const
{
    return localeText(remove_link_to) 
            + getTitle() + localeText(question_mark);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation 
 *         window.
 */
juce::String AppMenu::DesktopEntryData::getConfirmDeleteMessage() const
{
    return localeText(will_hide);
}

/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String AppMenu::DesktopEntryData::getEditorTitle() const
{
    return localeText(edit_app);
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
    catch(DesktopEntryFileError e)
    {
        DBG("DesktopEntryData::" << __func__ 
                << ": Failed to hide entry:" << e.what());
    }
}
