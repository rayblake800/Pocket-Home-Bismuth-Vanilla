
#include "MenuItem Types/ConfigAppMenuItem.h"
#include "MenuItem Types/DesktopEntryMenuItem.h"
#include "MenuItem Types/FolderMenuItem.h"
#include "MenuItem Types/BaseFolderMenuItem.h"
#include "AppMenuItemFactory.h"

/**
 * Create the folder menu item that defines the base menu folder
 */
AppMenuItem::Ptr AppMenuItemFactory::createBaseFolderItem
(const DesktopEntries& desktopEntries)
{
    return new BaseFolderMenuItem(desktopEntries);
}

/**
 * Get an AppMenuItem for an application link provided by the AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create(const AppConfigFile::AppItem& appItem)
{
    return new ConfigAppMenuItem(appItem);
}

/**
 * Get an AppMenuItem for an application link that was read from a desktop
 * entry file
 */
AppMenuItem::Ptr AppMenuItemFactory::create(const DesktopEntry& desktopEntry)
{
    return new DesktopEntryMenuItem(desktopEntry);
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create(const AppConfigFile::AppFolder& appFolder)
{
    return new FolderMenuItem(appFolder);
}

AppMenuItemFactory::AppMenuItemFactory() { }

AppMenuItemFactory::~AppMenuItemFactory() { }
/*