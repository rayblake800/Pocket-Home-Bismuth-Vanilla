#include "DesktopEntryLoader.h"
#include "AppJSON.h"

/* SharedResource object key */
const juce::Identifier AppJSON::resourceKey = "AppJSON";
/* JSON configuration file name */
static const constexpr char * jsonFilename = "apps.json";

/* Menu item JSON keys: */
static const juce::Identifier nameKey = "name";
static const juce::Identifier iconKey = "icon";
static const juce::Identifier commandKey = "command";
static const juce::Identifier termKey = "launch in terminal";
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
 * Gets all menu items within a folder in the application menu.
 */
juce::Array<juce::var> AppJSON::getMenuItems
    (const juce::Array<int> folderIndex) const
{
    using namespace juce;
    var folder = menuItems;
    for(int index : folderIndex)
    {
        if(!folder.isArray())
        {
            return {};
        }
        folder = folder[index];
        if(!folder.isObject())
        {
            return {};
        }
        folder = folder.getProperty(itemListKey, var());
    }
    if(!folder.isArray())
    {
        return {};
    }
    Array<var> menuItems;
    menuItems.addArray(folder.getArray());
    StringArray desktopEntryCategories;
    var categoryVars = folder.getProperty(categoryKey, var());
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
    var folder = menuItems;
    for(int i : folderIndex)
    {
        var nextFolder = folder[i].getProperty(itemListKey, var());
        if(!nextFolder.isArray())
        {
            break;
        }
        folder = nextFolder;
    }
    folder.insert(index, newItem);
    if (writeChangesNow)
    {
        writeChanges();
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
    var folder = menuItems;
    for(int i : folderIndex)
    {
        folder = folder[i].getProperty(itemListKey, var());
        if(!folder.isArray())
        {
            return;
        }
    }
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
