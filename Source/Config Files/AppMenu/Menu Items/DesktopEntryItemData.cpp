#include "Utils.h"
#include "MainConfigFile.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryItemData.h"

//localized text keys: 
static const constexpr char * remove_link_to = "remove_link_to";
static const constexpr char * question_mark = "question_mark";
static const constexpr char * will_hide = "will_hide";
static const constexpr char * edit_app = "edit_app";

DesktopEntryItemData::DesktopEntryItemData(const DesktopEntry& desktopEntry) :
desktopEntry(desktopEntry), Localized("DesktopEntryItemData") { }

/*
 * Gets the menu item's displayed title.
 */
juce::String DesktopEntryItemData::getTitle() const
{
    return desktopEntry.getName();
}

/*
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String DesktopEntryItemData::getIconName() const
{
    return desktopEntry.getIcon();
}

/*
 * Gets the menu item's application launch command.
 */
juce::String DesktopEntryItemData::getCommand() const
{
    return desktopEntry.getExec();
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool DesktopEntryItemData::getLaunchedInTerm() const
{
    return desktopEntry.getLaunchedInTerm();
}

/*
 * Gets the application categories connected to this menu item.
 */
juce::StringArray DesktopEntryItemData::getCategories() const
{
    return desktopEntry.getCategories();
}

/*
 * Sets the menu item's displayed title.
 */
void DesktopEntryItemData::setTitle(const juce::String& title)
{
    try
    {
        desktopEntry.setName(title);
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryItemData::" << __func__
                << ": Invalid title " << title);
    }
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void DesktopEntryItemData::setIconName(const juce::String& iconName)
{
    try
    {
        desktopEntry.setIcon(iconName);
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryItemData::" << __func__
                << ": Invalid icon " << iconName);
    }
}

/*
 * Sets the menu item's application launch command.
 */
void DesktopEntryItemData::setCommand(const juce::String& newCommand)
{
    try
    {
        desktopEntry.setExec(newCommand);
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryItemData::" << __func__
                << ": Invalid launch command " << newCommand);
    }
}

/*
 * Sets if this menu item runs its command in a new terminal window. */
void DesktopEntryItemData::setLaunchedInTerm(const bool termLaunch)
{
    desktopEntry.setLaunchedInTerm(termLaunch);
}

/*
 * Sets the application categories connected to this menu item.
 */
void DesktopEntryItemData::setCategories(const juce::StringArray& categories)
{
    try
    {
        desktopEntry.setCategories(categories);
    }
    catch(DesktopEntryFormatError e)
    {
        DBG("DesktopEntryItemData::" << __func__
                << ": Invalid categories " << categories.joinIntoString(";"));
    }
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int DesktopEntryItemData::getMovableChildCount() const
{
    return 0;
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void DesktopEntryItemData::saveChanges()
{
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntryFileError e)
    {
        DBG("DesktopEntryItemData::" << __func__ << ": Failed to write changes:"
                << e.what());
    }
}

/*
 * Gets an appropriate title to use for a deletion confirmation 
 *         window.
 */
juce::String DesktopEntryItemData::getConfirmDeleteTitle() const
{
    return localeText(remove_link_to) 
            + getTitle() + localeText(question_mark);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation 
 *         window.
 */
juce::String DesktopEntryItemData::getConfirmDeleteMessage() const
{
    return localeText(will_hide);
}

/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String DesktopEntryItemData::getEditorTitle() const
{
    return localeText(edit_app);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool DesktopEntryItemData::isEditable(const DataField dataField) const
{
    return true;
}

/*
 * Sets this menu item's desktop entry as hidden for the current user.
 */
void DesktopEntryItemData::deleteFromSource()
{
    desktopEntry.setIfDisplayed(false);
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntryFileError e)
    {
        DBG("DesktopEntryItemData::" << __func__ 
                << ": Failed to hide entry:" << e.what());
    }
}


