#include "Utils.h"
#include "MainConfigFile.h"
#include "DesktopEntryFormatError.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryItemData.h"

DesktopEntryItemData::DesktopEntryItemData(const DesktopEntry& desktopEntry, 
        const MenuIndex& menuIndex) :
desktopEntry(desktopEntry),
MenuItemData(menuIndex),
Localized("DesktopEntryItemData") { }

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool DesktopEntryItemData::isFolder() const
{
    return desktopEntry.getType() == DesktopEntry::Type::directory;
}

/*
 * Gets the menu item's displayed title.
 */
juce::String DesktopEntryItemData::getTitle() const
{
    return desktopEntry.getName();
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
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String DesktopEntryItemData::getIconName() const
{
    return desktopEntry.getIcon();
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
 * Gets the application categories connected to this menu item.
 */
juce::StringArray DesktopEntryItemData::getCategories() const
{
    return desktopEntry.getCategories();
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
 * Gets the menu item's application launch command.
 */
juce::String DesktopEntryItemData::getCommand() const
{
    return desktopEntry.getExec();
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
 * Checks if this menu item launches an application in a new terminal window.
 */
bool DesktopEntryItemData::getLaunchedInTerm() const
{
    return desktopEntry.getLaunchedInTerm();
}

/*
 * Sets if this menu item runs its command in a new terminal window. */
void DesktopEntryItemData::setLaunchedInTerm(const bool termLaunch)
{
    desktopEntry.setLaunchedInTerm(termLaunch);
}

/*
 * Deletes this menu item data from its source JSON file.
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

/*
 * Writes all changes to this menu item back to its data source.
 */
void DesktopEntryItemData::updateSource()
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
 * Checks if this menu item can be moved within its menu folder.
 */
bool DesktopEntryItemData::canMoveIndex(const int offset)
{
    return false;
}


/*
 * Attempts to move this menu item within its menu folder.
 *                moved by the given offset value.
 */
bool DesktopEntryItemData::moveIndex(const int offset)
{
    return false;
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
bool DesktopEntryItemData::isEditable(const DataField dataField)
{
    return true;
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int DesktopEntryItemData::getFolderSize()
{
    return 0;
}


