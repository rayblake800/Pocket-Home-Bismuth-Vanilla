#include "ConfigAppMenuItem.h"
#include "DesktopEntryMenuItem.h"
#include "FolderMenuItem.h"
#include "BaseFolderMenuItem.h"
#include "AppMenuItemFactory.h"

/**
 * Create the folder menu item that defines the base menu folder
 */
AppMenuItem::Ptr AppMenuItemFactory::createBaseFolderItem
(DesktopEntries& desktopEntries)
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
AppMenuItem::Ptr AppMenuItemFactory::create
(const AppConfigFile::AppFolder& appFolder, DesktopEntries& desktopEntries)
{
    return new FolderMenuItem(appFolder,desktopEntries);
}

AppMenuItemFactory::AppMenuItemFactory() { }

AppMenuItemFactory::~AppMenuItemFactory() { }
