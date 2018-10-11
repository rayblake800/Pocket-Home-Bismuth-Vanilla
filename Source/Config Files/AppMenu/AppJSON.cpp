#include "DesktopEntryLoader.h"
#include "AppJSON.h"

/* SharedResource object key */
const juce::Identifier AppJSON::resourceKey = "AppJSON";
/* JSON configuration file name */
static const constexpr char * jsonFilename = "apps.json";

/* Menu item JSON keys: */
static const juce::Identifier itemListKey = "folder items";
static const juce::Identifier categoryKey = "categories";

AppJSON::AppJSON() : ConfigJSON(resourceKey, jsonFilename)
{
    using namespace juce;
    // Load shortcuts:
    menuItems = initProperty<var>(itemListKey);
    DBG("AppJSON::" << __func__ << ": Read " << menuItems.size()
            << " menu items in the main menu folder.");
    loadJSONData();   
}

/*
 * Gets the number of menu items within a menu folder.
 */
int AppJSON::getFolderSize(const MenuIndex& folderIndex,
        const bool includeDesktopEntries) const
{
    using namespace juce;
    if(!includeDesktopEntries || folderIndex.getDepth() == -1)
    {
        var folder = getFolderVar(folderIndex);
        return ((folder.isArray()) ? folder.size() : -1);
    }
    var folderItem = getItemVar(folderIndex);
    if(!folderItem.isObject())
    {
        return -1;
    }
    var folder = folderItem.getProperty(itemListKey, var());
    int size = folder.size();
    var categories = folderItem.getProperty(categoryKey, var());
    if(categories.isArray())
    {
        StringArray categoryStrings;
        for(const var& category : *(categories.getArray()))
        {
            categoryStrings.add(category);
        }
        DesktopEntryLoader entryLoader;
        size += entryLoader.getCategoryListEntryIDs(categoryStrings).size();
    }
    return size;
}

/*
 * Gets all menu items within a folder in the application menu.
 */
juce::Array<juce::var> AppJSON::getMenuItems(const MenuIndex& folderIndex) const
{
    using namespace juce;
    // The root folder item needs to be handled differently because only its
    // folder items are defined.
    if(folderIndex.getDepth() == -1)
    {
        Array<var> itemList;
        itemList.addArray(*menuItems.getArray());
        return itemList;
    }
    var folderObject = getItemVar(folderIndex);
    if(!folderObject.isObject())
    {
        return {};
    }
    Array<var> itemList;
    var folderItems = folderObject.getProperty(itemListKey, var());
    if(folderItems.isArray())
    {
        itemList.addArray(*folderItems.getArray());
    }
    StringArray desktopEntryCategories;
    var categoryVars = folderObject.getProperty(categoryKey, var());
    for(int i = 0; i < categoryVars.size(); i++)
    {
        desktopEntryCategories.add(categoryVars[i]);
    }
    if(!desktopEntryCategories.isEmpty())
    {
        DesktopEntryLoader entryLoader;
        StringArray entryIDs = entryLoader.getCategoryListEntryIDs
            (desktopEntryCategories);
        for(const String& entryID : entryIDs)
        {
            itemList.add(var(entryID));
        }
    }
    DBG("AppJSON::" << __func__ << ": Found " << menuItems.size()
            << " menu items, " << folderItems.size() << " folder items and "
            << (menuItems.size() - folderItems.size()) << " desktop files from "
            << desktopEntryCategories.size() << " categories in folder "
            << folderIndex.toString());
    return itemList;
}

/*
 * Adds a new menu item to the list of menu items.
 */
void AppJSON::addMenuItem(
        const juce::var& newItem, 
        const MenuIndex& index,
        const bool writeChangesNow)
{
    using namespace juce;
    MenuIndex parentIndex = index.parentIndex();
    var folder = getFolderVar(parentIndex);
    if(folder.isArray())
    {
        folder.insert(index.folderIndex(), newItem);
        //updateCacheIndices(index, true);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/*
 * Removes an item from the menu.
 */
void AppJSON::removeMenuItem(const MenuIndex& index, const bool writeChangesNow)
{
    using namespace juce;
    var parentFolder = getFolderVar(index);
    if (index.folderIndex() >= 0 && index.folderIndex() < parentFolder.size())
    {
        parentFolder.remove(index.folderIndex());
        menuItemCache.erase(index);
        updateCacheIndices(index, false);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/*
 * Saves a menu item's data to the list of cached menu items.
 */
void AppJSON::addCachedMenuItem(MenuItemData::Ptr menuItem)
{
    menuItemCache[menuItem->getIndex()] = menuItem;
}

/*
 * Loads a cached menu item from the list of cached menu items.
 */
MenuItemData::Ptr AppJSON::getCachedMenuItem(const MenuIndex& cacheIndex)
{
    try
    {
        return menuItemCache.at(cacheIndex);
    }
    catch(std::out_of_range)
    {
        return nullptr;
    }
}

/*
 * Removes all cached menu items.
 */
void AppJSON::clearMenuItemCache()
{
    menuItemCache.clear();
}

/*
 * Copies all menu data back to the JSON configuration file.
 */
void AppJSON::writeDataToJSON()
{
    using namespace juce;
    updateProperty<var>(itemListKey, menuItems);
}

/*
 * Finds a folder within the folder data tree, and returns its data.
 */
juce::var AppJSON::getItemVar(const MenuIndex& folderIndex) const
{
    using namespace juce;
    var folder = menuItems;
    for(int depth = 0; depth <= folderIndex.getDepth(); depth++)
    {
        int index = folderIndex[depth];
        if(folder.size() <= index)
        {
            DBG("AppJSON::" << __func__ << ": Invalid index "
                    << folderIndex.toString() << ": Menu item at depth "
                    << depth << " is size " << folder.size() << ", needed "
                    << index << " or greater.");
            return var();
        }
        folder = folder[folderIndex[depth]];
        folder = folder.getProperty(itemListKey, var());
        if(!folder.isArray())
        {
            DBG("AppJSON::" << __func__ << ": Invalid index "
                    << folderIndex.toString() << ": Menu item at depth "
                    << depth << " is not a folder.");
            return var();
        }
    }
    return folder;
}
    
/*
 * Finds a menu item within the menu tree and return its parent's data.
 */
juce::var AppJSON::getParentVar(const MenuIndex& menuIndex) const
{
    MenuIndex parentIndex = menuIndex.parentIndex();
    return getItemVar(parentIndex);
}

/*
 * Gets a menu item's folder data.
 */
juce::var AppJSON::getFolderVar(const MenuIndex& folderIndex) const
{
    using namespace juce;
    return getItemVar(folderIndex).getProperty(itemListKey, var());
}
    
/*
 * Update cached item indices when a menu item is inserted or removed.
 */
void AppJSON::updateCacheIndices(const MenuIndex& changePoint,
        const bool itemInserted)
{
    using namespace juce;
    Array<MenuItemData::Ptr> updatedMenuItems;
    int insertDepth = changePoint.getDepth();
    int insertPos = changePoint[insertDepth];
    int offset = (itemInserted ? 1 : -1);
    for(std::pair<MenuIndex, MenuItemData::Ptr> cachedItem : menuItemCache)
    {
        if(cachedItem.first.getDepth() >= insertDepth
                && cachedItem.first[insertDepth] >= insertPos)
        {
            updatedMenuItems.add(cachedItem.second);
        }
    }
    for(const MenuItemData::Ptr& updatedItem : updatedMenuItems)
    {
        menuItemCache.erase(updatedItem->getIndex());
    }
    for(const MenuItemData::Ptr& updatedItem : updatedMenuItems)
    {
        updatedItem->updateIndex(insertDepth, offset);
        menuItemCache[updatedItem->getIndex()] = updatedItem;
    }
}
