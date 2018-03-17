#include "AppMenuItem.h"

AppMenuItem::AppMenuItem() { }

AppMenuItem::~AppMenuItem() { }

/**
 * @return true if this menu item is an application folder.
 */
bool AppMenuItem::isFolder() const
{
    return false;
}

/**
 * @return all menu items in this folder, or an empty array if this isn't
 * a folder.
 */
Array<AppMenuItem::Ptr> AppMenuItem::getFolderItems() const
{
    return {};
}

/**
 * @return the display name of the associated application.
 */
String AppMenuItem::getAppName() const
{
    return String();
}

/**
 * @return the application shell command or directory path.
 */
String AppMenuItem::getCommand() const
{
    return String();
}

/**
 * @return true iff this menu item is an application that launches in
 * the terminal. 
 */
bool AppMenuItem::isTerminalApp() const
{
    return false;
}

/**
 * @return true iff changing this menu item makes changes to .desktop or
 * .directory files.
 */
bool AppMenuItem::changesDesktopEntries() const
{
    return false;
}

/**
 * @return all application categories linked to this menu item.
 */
StringArray AppMenuItem::getCategories() const
{
    return {};
}

/**
 * @return the name or path used to load the icon file. 
 */
String AppMenuItem::getIconName() const
{
    return String();
}

/**
 * Return true if this menu item has an index that can be moved by a given 
 * amount.
 */
bool AppMenuItem::canChangeIndex(int offset) const
{
    return false;
}

/**
 * @return true iff this menu item and another share the same
 * properties
 */
bool AppMenuItem::operator==(const AppMenuItem& toCompare) const {
    return getAppName() == toCompare.getAppName()
            && getCategories() == toCompare.getCategories()
            && getCommand() == toCompare.getCommand()
            && isFolder() == toCompare.isFolder()
            && isTerminalApp() == toCompare.isTerminalApp()
            && getIconName() == toCompare.getIconName()
            && changesDesktopEntries() == toCompare.changesDesktopEntries();
 }

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
String AppMenuItem::getConfirmDeleteTitle() const
{
    return String("Delete ") + getAppName() + String("?");
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
String AppMenuItem::getConfirmDeleteMessage() const
{
    return "Really delete this menu item?";
}

/**
 * @return true iff this menu item has categories that can be edited,
 * defaults to false.
 */
bool AppMenuItem::hasEditableCategories() const
{
    return false;
}

/**
 * @return true iff this menu item has a command that can be edited,
 * defaults to false.
 */
bool AppMenuItem::hasEditableCommand() const
{
    return false;
}

/**
 * @return the title to display over an editor for this menu item. 
 */
String AppMenuItem::getEditorTitle() const
{
    return "Edit Menu Item";
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > AppMenuItem::getEditorCallback()
{
    return [](AppMenuPopupEditor*)
    {
    };
}

/**
 * Removes the source of this menu item's data
 * @return true iff the source was removed.
 */
bool AppMenuItem::removeMenuItemSource()
{
    return false;
}

/**
 * If possible, change the index of this menu item by some
 * offset amount.
 * @param offset will be added to the menu item's current index, if
 * possible.
 * @return true iff the operation succeeded.
 */
bool AppMenuItem::moveDataIndex(int offset)
{
    return false;
}
