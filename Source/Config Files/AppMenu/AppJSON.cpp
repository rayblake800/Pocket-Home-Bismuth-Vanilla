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
int AppJSON::getFolderSize(const juce::Array<int> folderIndex,
        const bool includeDesktopEntries) const
{
    using namespace juce;
    if(!includeDesktopEntries)
    {
        var folder = getFolderVar(folderIndex);
        return ((folder.isArray()) ? folder.size() : -1);
    }
    var folder = menuItems;
    var folderItem;
    for(int i : folderIndex)
    {
        folderItem = folder[i];
        folder = folderItem.getProperty(itemListKey, var());
        if(!folderItem.isObject() || !folder.isArray())
        {
            return -1;
        }
    }
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
juce::Array<juce::var> AppJSON::getMenuItems
    (const juce::Array<int> folderIndex) const
{
    using namespace juce;
    var folderObject;
    var folderItems = menuItems;
    for(int i : folderIndex)
    {
        folderObject = folderItems[i];
        folderItems = folderObject.getProperty(itemListKey, var());
        if(!folderObject.isObject())
        {
            return {};
        }
    }
    Array<var> menuItems;
    if(folderItems.isArray())
    {
        menuItems.addArray(*folderItems.getArray());
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
            menuItems.add(var(entryID));
        }
    }
    DBG("AppJSON::" << __func__ << ": Found " << menuItems.size()
            << " menu items, " << folderItems.size() << " folder items and "
            << (menuItems.size() - folderItems.size()) << " desktop files from "
            << desktopEntryCategories.size() << " categories.");
    return menuItems;
}

/*
 * Adds a new menu item to the list of menu items.
 */
void AppJSON::addMenuItem(
        const juce::var& newItem, 
        const int index,
        const juce::Array<int> folderIndex, 
        const bool writeChangesNow)
{
    using namespace juce;
    var folder = getFolderVar(folderIndex);
    if(folder.isArray())
    {
        folder.insert(index, newItem);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/*
 * Removes an item from the menu.
 */
void AppJSON::removeMenuItem(const int index, 
        const juce::Array<int> folderIndex,
        const bool writeChangesNow)
{
    using namespace juce;
    var folder = getFolderVar(folderIndex);
    if (index >= 0 && index < folder.size())
    {
        folder.remove(index);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
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
juce::var AppJSON::getFolderVar(const juce::Array<int>& folderIndex) const
{
    using namespace juce;
    var folder = menuItems;
    for(int i : folderIndex)
    {
        folder = folder[i];
        folder = folder.getProperty(itemListKey, var());
        if(!folder.isArray())
        {
            return var();
        }
    }
    return folder;
}

