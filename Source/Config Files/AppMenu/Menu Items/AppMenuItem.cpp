#include "AppMenuItem.h"

/*
 * Creates a menu item from some source of menu data. 
 */
AppMenuItem::AppMenuItem(MenuItemData::Ptr dataSource) :
dataSource(dataSource) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenuItem::AppMenuItem(const AppMenuItem& toCopy) :
dataSource(toCopy.dataSource) { }

/*
 * Checks if this menu item holds valid menu data.
 */
bool AppMenuItem::isNull()
{
    return dataSource == nullptr;
}

/*
 * Gets the menu item's displayed title.
 */
juce::String AppMenuItem::getTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource->getTitle();
}

/*
 * Gets the menu item's icon name.
 */
juce::String AppMenuItem::getIconName() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource->getIconName();
}

/*
 * Gets any launch command associated with this menu item.
 */
juce::String AppMenuItem::getCommand() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource->getCommand();
}

/*
 * Checks if this menu item is a terminal application.
 */
bool AppMenuItem::getLaunchedInTerm() const
{
    if(isNull())
    {
        return false;
    }
    return dataSource->getLaunchedInTerm();
}

/*
 * Gets all application categories associated with this menu item.
 */
juce::StringArray AppMenuItem::getCategories() const
{
    if(isNull())
    {
        return juce::StringArray();
    }
    return dataSource->getCategories();
}

/*
 * Gets the folder menu item that contains this menu item.
 */
AppMenuItem AppMenuItem::getParentFolder() const
{
    if(isNull())
    {
        return AppMenuItem();
    }
    return AppMenuItem(dataSource->getParent());
}

/*
 * Gets the menu item's index within its folder.
 */
const MenuIndex& AppMenuItem::getIndex() const
{
    if(isNull())
    {
        return -1;
    }
    return dataSource->getIndex();
}

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenuItem::isFolder() const
{
    if(isNull())
    {
        return false; 
    }
    return dataSource->isFolder();
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int AppMenuItem::getFolderSize() const
{
    if(isNull())
    {
        return 0; 
    }
    return dataSource->getFolderSize();
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int AppMenuItem::getMovableChildCount() const
{
    if(isNull())
    {
        return 0; 
    }
    return dataSource->getMovableChildCount();
} 

/*
 * Gets a menu item held within this menu item.
 */
AppMenuItem AppMenuItem::getFolderItem(const int folderIndex) const
{
    if(isNull())
    {
        return AppMenuItem(); 
    }
    return AppMenuItem(dataSource->getChild(folderIndex));
}

/*
 * Gets all menu items held within this menu item.
 */
juce::Array<AppMenuItem> AppMenuItem::getFolderItems() const
{
    if(isNull())
    {
        return {}; 
    }
    juce::Array<AppMenuItem> folderItems;
    juce::Array<MenuItemData::Ptr> childData = dataSource->getChildren();
    for(const MenuItemData::Ptr& item : childData)
    {
        folderItems.add(AppMenuItem(item);
    }
    return folderItems;
}

/*
 * Compares this menu item with another.
 */
bool AppMenuItem::operator==(const AppMenuItem& toCompare) const
{
    if(dataSource == toCompare.dataSource)
    {
        return true;
    }
    return getTitle() == toCompare.getTitle()
        && getIconName() == toCompare.getIconName()
        && getCommand() == toCompare.getCommand()
        && getLaunchedInTerm() == toCompare.getLaunchedInTerm()
        && getCategories() == toCompare.getCategories();
}

/*
 * Compares this menu item with another.
 */
bool AppMenuItem::operator<(const AppMenuItem& toCompare) const
{
    return getTitle() < toCompare.getTitle();
}

/*
 * Gets a menu item's internal data object.
 */
MenuItemData::Ptr AppMenuItem::Editor::getMenuItemData
(AppMenuItem& menuItem) const
{
    return menuItem.dataSource;
}

/*
 * Removes a menu item's internal data object.
 */
MenuItemData::Ptr AppMenuItem::Editor::clearMenuItemData
(AppMenuItem& menuItem) const
{
    menuItem.dataSource = nullptr;
}

/*
 * Checks if this menu item has categories that may be edited.
 */
bool AppMenuItem::hasEditableCategories() const
{
    if(isNull())
    {
        return false;
    }
    return dataSource.isEditable(MenuItemData::DataField::categories);
}

/*
 * Checks if the menu item has an editable command field.
 */
bool AppMenuItem::hasEditableCommand() const
{
    if(isNull())
    {
        return false;
    }
    return dataSource.isEditable(MenuItemData::DataField::command);
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String AppMenuItem::getConfirmDeleteTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource.getConfirmDeleteTitle();
}

/*
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String AppMenuItem::getConfirmDeleteMessage() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource.getConfirmDeleteMessage();
}

/*
 * Gets a title to use when editing this menu item.
 */
juce::String AppMenuItem::getEditorTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return dataSource.getEditorTitle();
}
