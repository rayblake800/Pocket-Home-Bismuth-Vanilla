#include "AppMenuItem.h"

/*
 * Creates a menu item from some source of menu data. 
 */
AppMenuItem::AppMenuItem(MenuItemData::Ptr dataSource) :
Nullable(dataSource) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenuItem::AppMenuItem(const AppMenuItem& toCopy) :
Nullable(toCopy.getData()) { }

/*
 * Gets the menu item's displayed title.
 */
juce::String AppMenuItem::getTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getTitle();
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
    return getData()->getIconName();
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
    return getData()->getCommand();
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
    return getData()->getLaunchedInTerm();
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
    return getData()->getCategories();
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
    return AppMenuItem(getData()->getParentFolder());
}

/*
 * Gets the menu item's index within its folder.
 */
const int AppMenuItem::getIndex() const
{
    if(isNull())
    {
        return -1;
    }
    return getData()->getIndex();
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
    return getData()->isFolder();
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
    return getData()->getFolderSize();
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
    return getData()->getMovableChildCount();
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
    return AppMenuItem(getData()->getChild(folderIndex));
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
    juce::Array<MenuItemData::Ptr> childData = getData()->getChildren();
    for(const MenuItemData::Ptr& item : childData)
    {
        folderItems.add(AppMenuItem(item));
    }
    return folderItems;
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
    return getData()->getConfirmDeleteTitle();
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
    return getData()->getConfirmDeleteMessage();
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
    return getData()->getEditorTitle();
}

/*
 * Compares this menu item with another.
 */
bool AppMenuItem::operator==(const AppMenuItem& toCompare) const
{
    if(getData() == toCompare.getData())
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
 * Checks if a data field within a menu item can be edited.
 */
bool AppMenuItem::Editor::isEditable
(AppMenuItem menuItem, const DataField dataField) const
{
    if(menuItem.isNull())
    {
        return false;
    }
    return menuItem.getData()->isEditable(dataField);
}

/*
 * Sets a menu item's displayed title.
 */
void AppMenuItem::Editor::setTitle
(AppMenuItem menuItem, const juce::String& title) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::title))
    {
        menuItem.getData()->setTitle(title);
    }
}

/*
 * Sets the name or path used to load a menu item's icon file.
 */
void AppMenuItem::Editor::setIconName
(AppMenuItem menuItem, const juce::String& iconName) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::icon))
    {
        menuItem.getData()->setIconName(iconName);
    }
}

/*
 * Sets a menu item's application launch command.
 */
void AppMenuItem::Editor::setCommand
(AppMenuItem menuItem, const juce::String command) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::command))
    {
        menuItem.getData()->setCommand(command);
    }
}

/*
 * Sets if a menu item runs its command in a new terminal window.
 */
void AppMenuItem::Editor::setLaunchedInTerm
(AppMenuItem menuItem, const bool launchInTerm) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::termLaunchOption))
    {
        menuItem.getData()->setLaunchedInTerm(launchInTerm);
    }
}

/*
 * Sets the application categories connected to a menu item.
 */
void AppMenuItem::Editor::setCategories
(AppMenuItem menuItem, const juce::StringArray categories) const
{
    if(!menuItem.isNull() && isEditable(menuItem, DataField::categories))
    {
        menuItem.getData()->setCategories(categories);
    }
}

/*
 * Attempts to replace a menu item in its parent folder, saving the change to 
 * the menu item's data source.
 */
bool AppMenuItem::Editor::replace
(AppMenuItem toReplace, AppMenuItem replacementItem) const
{
    if(!toReplace.isNull() && !replacementItem.isNull())
    {
        return toReplace.getData()->replace(replacementItem.getData());
    }
    return false;
}

/*
 * Attempts to insert a menu item into a folder menu item, saving the change to
 * the folder menu item's data source.
 */
bool AppMenuItem::Editor::insertChild
(AppMenuItem folderItem, AppMenuItem childItem, const int index) const
{
    if(!folderItem.isNull() && !childItem.isNull())
    {
        return folderItem.getData()->insertChild(childItem.getData(), index);
    }
    return false;
}

/*
 * Attempts to remove a menu item from the menu, saving the change to the menu
 * item's data source.
 */
bool AppMenuItem::Editor::remove(AppMenuItem toRemove) const
{
    if(!toRemove.isNull())
    {
        return toRemove.getData()->remove();
    }
    return false;
}

/*
 * Swaps the positions of two folder items, saving the change to the folder menu
 * item's data source.
 */
bool AppMenuItem::Editor::swapChildren
(AppMenuItem folderItem, const int childIdx1, const int childIdx2)
{
    if(!folderItem.isNull())
    {
        return folderItem.getData()->swapChildren(childIdx1, childIdx2);
    }
    return false;
}

/*
 * Saves all changes made to a menu item back to the menu item's data source.
 */
void AppMenuItem::Editor::saveChanges(AppMenuItem menuItem) const
{
    if(!menuItem.isNull())
    {
        menuItem.getData()->saveChanges();
    }
}
