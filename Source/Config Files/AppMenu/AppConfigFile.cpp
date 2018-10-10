#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "ConfigItemData.h"
#include "DesktopEntryItemData.h"
#include "DesktopEntryLoader.h"
#include "Utils.h"

/* Menu item JSON keys: */
static const juce::Identifier nameKey = "name";
static const juce::Identifier iconKey = "icon";
static const juce::Identifier commandKey = "command";
static const juce::Identifier termKey = "launch in terminal";
static const juce::Identifier itemListKey = "folder items";
static const juce::Identifier categoryKey = "categories";

/*
 * Gets all menu items within a menu folder.
 */
juce::Array<AppMenuItem> AppConfigFile::getMenuItems
(const MenuIndex& folderIndex)
{
    using namespace juce;
    auto appJSON = getReadLockedResource();
    Array<var> menuVars = appJSON->getMenuItems(folderIndex);
    Array<AppMenuItem> menuItems;

    for(const var& menuVar : menuVars)
    {
        MenuIndex index = folderIndex.childIndex(menuItems.size());
        MenuItemData::Ptr newItem;
        if(menuVar.isObject())
        {
            newItem = new ConfigItemData(menuVar, index);
            menuItems.add(AppMenuItem(newItem));
        }
        else if(menuVar.isString())
        {
            DesktopEntryLoader entryLoader;
            DesktopEntry menuEntry = entryLoader.getDesktopEntry(menuVar);
            newItem = new DesktopEntryItemData(menuEntry, index);
        }
        else
        {
            DBG("AppConfigFile::" << __func__ << ": Invalid menu item data "
                    << menuVar.toString());
            continue;
        }
        menuItems.add(AppMenuItem(newItem));
        appJSON->addCachedMenuItem(newItem);
    }
    return menuItems;
}

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenuItem AppConfigFile::getRootMenuItem()
{
    using namespace juce;
    auto appJSON = getReadLockedResource();
    MenuIndex rootIndex;
    MenuItemData::Ptr rootItem = appJSON->getCachedMenuItem(rootIndex);
    if(rootItem == nullptr)
    {
        DynamicObject* rootObject = new DynamicObject();
        Array<var> menuItems = appJSON->getMenuItems({});
        rootObject->setProperty(itemListKey, menuItems);
        var rootJSON(rootObject);
        rootItem = new ConfigItemData(rootJSON, MenuIndex());
        appJSON->addCachedMenuItem(rootItem);
    }
    return AppMenuItem(rootItem);
}

/*
 * Adds a new menu item to the list of items shown in a menu folder.
 */
void AppConfigFile::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        const MenuIndex& index,
        const bool writeChangesNow) 
{
    using namespace juce;
    DynamicObject* menuObject = new DynamicObject();
    menuObject->setProperty(nameKey,title);
    menuObject->setProperty(iconKey, icon);
    menuObject->setProperty(commandKey, command);
    menuObject->setProperty(termKey, launchInTerm);
    menuObject->setProperty(categoryKey, categories);
    var menuVar(menuObject);
    auto appJSON = getWriteLockedResource();
    appJSON->addMenuItem(menuVar, index, writeChangesNow);
    appJSON->addCachedMenuItem(new ConfigItemData(menuVar, index));
}

/*
 * Removes all cached menu items.
 */
void AppConfigFile::clearMenuItemCache()
{
    auto appJSON = getWriteLockedResource();
    appJSON->clearMenuItemCache();
}

