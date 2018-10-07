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
    (const juce::Array<int>& folderIndex)
{
    using namespace juce;
    auto appJSON = getReadLockedResource();
    Array<var> menuVars = appJSON->getMenuItems(folderIndex);
    Array<AppMenuItem> menuItems;

    for(const var& menuVar : menuVars)
    {
        int index = menuItems.size();
        if(menuVar.isObject())
        {
            ConfigItemData configData(menuVar, index, folderIndex);
            menuItems.add(AppMenuItem(&configData));
        }
        else if(menuVar.isString())
        {
            DesktopEntryLoader entryLoader;
            DesktopEntry menuEntry = entryLoader.getDesktopEntry(menuVar);
            DesktopEntryItemData entryData(menuEntry, index, folderIndex);
            menuItems.add(AppMenuItem(&entryData));
        }
        else
        {
            DBG("AppConfigFile::" << __func__ << ": Invalid menu item data "
                    << menuVar.toString());
            continue;
        }
    }
    return menuItems;
}

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenuItem AppConfigFile::getRootMenuItem()
{
    using namespace juce;
    DynamicObject* rootObject = new DynamicObject();
    {
        auto appJSON = getReadLockedResource();
        Array<var> menuItems = appJSON->getMenuItems({});
        rootObject->setProperty(itemListKey, menuItems);
    }
    var rootJSON(rootObject);
    ConfigItemData rootData(rootJSON, -1, {});
    return AppMenuItem(&rootData);
}

/*
 * Adds a new menu item to the list of items shown in a menu folder.
 */
void AppConfigFile::addMenuItem(
        const AppMenuItem& menuItem, 
        const int index,
        const juce::Array<int> folderIndex, 
        const bool writeChangesNow) 
{
    using namespace juce;
    DynamicObject* menuObject = new DynamicObject();
    menuObject->setProperty(nameKey, menuItem.getTitle());
    menuObject->setProperty(iconKey, menuItem.getIconName());
    menuObject->setProperty(commandKey, menuItem.getCommand());
    menuObject->setProperty(termKey, menuItem.getLaunchedInTerm());
    menuObject->setProperty(categoryKey, menuItem.getCategories());
    var menuVar(menuObject);
    auto appJSON = getWriteLockedResource();
    appJSON->addMenuItem(menuVar, index, folderIndex, writeChangesNow);
}

