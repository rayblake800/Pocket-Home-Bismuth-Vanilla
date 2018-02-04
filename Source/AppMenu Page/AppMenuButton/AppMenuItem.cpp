#include "AppMenuItem.h"

AppMenuItem::AppMenuItem()
{
}

AppMenuItem::~AppMenuItem()
{
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
