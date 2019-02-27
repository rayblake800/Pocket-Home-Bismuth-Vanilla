#define APPMENU_IMPLEMENTATION
#include "AppMenu_MenuItem.h"

/*
 * Creates a menu item from some source of menu data. 
 */
AppMenu::MenuItem::MenuItem(ItemData::Ptr dataSource) :
Nullable(dataSource) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenu::MenuItem::MenuItem(const MenuItem& toCopy) :
Nullable(toCopy.getData()) { }

/*
 * Gets the menu item's displayed title.
 */
juce::String 
AppMenu::MenuItem::getTitle() const
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
juce::String 
AppMenu::MenuItem::getIconName() const
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
juce::String 
AppMenu::MenuItem::getCommand() const
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
bool AppMenu::MenuItem::getLaunchedInTerm() const
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
juce::StringArray 
AppMenu::MenuItem::getCategories() const
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
AppMenu::MenuItem 
AppMenu::MenuItem::getParentFolder() const
{
    if(isNull())
    {
        return MenuItem();
    }
    return MenuItem(getData()->getParentFolder());
}

/*
 * Gets the menu item's index within its folder.
 */
int AppMenu::MenuItem::getIndex() const
{
    if(isNull())
    {
        return -1;
    }
    return getData()->getIndex();
}

/*
 * Gets an optional unique ID tied to this menu item.
 */
juce::String AppMenu::MenuItem::getID() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getID();
}

/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenu::MenuItem::isFolder() const
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
int AppMenu::MenuItem::getFolderSize() const
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
int AppMenu::MenuItem::getMovableChildCount() const
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
AppMenu::MenuItem 
AppMenu::MenuItem::getFolderItem(const int folderIndex) const
{
    if(isNull())
    {
        return MenuItem(); 
    }
    return MenuItem(getData()->getChild(folderIndex));
}

/*
 * Gets all menu items held within this menu item.
 */
juce::Array<AppMenu::MenuItem> 
AppMenu::MenuItem::getFolderItems() const
{
    if(isNull())
    {
        return {}; 
    }
    juce::Array<MenuItem> folderItems;
    for(int i = 0; i < getData()->getFolderSize(); i++)
    {
        folderItems.add(MenuItem(getData()->getChild(i)));
    }
    return folderItems;
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String 
AppMenu::MenuItem::getConfirmDeleteTitle() const
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
juce::String 
AppMenu::MenuItem::getConfirmDeleteMessage() const
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
juce::String 
AppMenu::MenuItem::getEditorTitle() const
{
    if(isNull())
    {
        return juce::String();
    }
    return getData()->getEditorTitle();
}

/*
 * Checks if two menu items represent the same item in the menu.
 */
bool AppMenu::MenuItem::operator==(const MenuItem& toCompare) const
{
    return getData() == toCompare.getData();
}

/*
 * Checks if two menu items do not represent the same item in the menu.
 */
bool AppMenu::MenuItem::operator!=(const MenuItem& toCompare) const
{
   return !operator==(toCompare);
}

/*
 * Compares this menu item with another.
 */
bool AppMenu::MenuItem::operator<(const MenuItem& toCompare) const
{
    return getTitle() < toCompare.getTitle();
}

/*
 * If this MenuItem is not null, add a Listener object to its menu data.
 */
void AppMenu::MenuItem::addListener(Listener* toAdd)
{
    if(!isNull())
    {
        getData()->addListener(toAdd);
    }
}

/*
 * If this menuItem is not null, make a Listener object stop tracking its menu 
 * data.
 */
void AppMenu::MenuItem::removeListener(Listener* toRemove)
{
    if(!isNull())
    {
        getData()->removeListener(toRemove);
    }
}

/*
 * Checks if a data field within the menu item can be edited.
 */
bool AppMenu::MenuItem::isEditable(const DataField dataField) const
{
    if(isNull())
    {
        return false;
    }
    return getData()->isEditable(dataField);
}

/*
 * Sets the menu item's displayed title.
 */
void AppMenu::MenuItem::setTitle(const juce::String& title)
{
    if(!isNull() && isEditable(DataField::title) 
            && title != getData()->getTitle())
    {
        getData()->setTitle(title);
    }
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenu::MenuItem::setIconName(const juce::String& iconName)
{
    if(!isNull() && isEditable(DataField::icon) 
            && iconName != getData()->getIconName())
    {
        getData()->setIconName(iconName);
    }
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenu::MenuItem::setCommand(const juce::String& command)
{
    if(!isNull() && isEditable(DataField::command)
            && command != getData()->getCommand())
    {
        getData()->setCommand(command);
    }
}

/*
 * Sets if the menu item runs its command in a new terminal window.
 */
void AppMenu::MenuItem::setLaunchedInTerm(const bool launchInTerm){
    if(!isNull() && isEditable(DataField::termLaunchOption)
            && launchInTerm != getData()->getLaunchedInTerm())
    {
        getData()->setLaunchedInTerm(launchInTerm);
    }
}

/*
 * Sets the application categories connected to the menu item.
 */
void AppMenu::MenuItem::setCategories(const juce::StringArray& categories)
{
    if(!isNull() && isEditable(DataField::categories) 
            && categories != getData()->getCategories())
    {
        getData()->setCategories(categories);
    }
}

/*
 * Attempts to insert a menu item into this menu item, saving the change to the 
 * folder menu item's data source.
 */
bool AppMenu::MenuItem::insertChild(MenuItem childItem, const int index)
{
    if(!isNull() && !childItem.isNull())
    {
        return getData()->insertChild(childItem.getData(), index);
    }
    return false;
}

/*
 * Attempts to remove a menu item from the menu, optionally saving the change to
 * the menu item's data source.
 */
bool AppMenu::MenuItem::remove(const bool updateSource)
{
    if(!isNull())
    {
        return getData()->remove(updateSource);
    }
    return false;
}

/*
 * Swaps the positions of two folder items, saving the change to the folder menu
 * item's data source if both folder items were user-movable.
 */
bool AppMenu::MenuItem::swapChildren(const int childIdx1, const int childIdx2)
{
    if(!isNull())
    {
        return getData()->swapChildren(childIdx1, childIdx2);
    }
    return false;
}

/*
 * Saves all changes made to a menu item back to the menu item's data source.
 */
void AppMenu::MenuItem::saveChanges()
{
    if(!isNull())
    {
        ItemData* data = getData();
        data->saveChanges();
    }
}
