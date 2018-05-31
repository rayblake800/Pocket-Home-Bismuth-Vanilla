#include "ConfigAppMenuItem.h"
#include "DesktopEntryMenuItem.h"
#include "FolderMenuItem.h"
#include "BaseFolderMenuItem.h"
#include "AppMenuItemFactory.h"

AppMenuItemFactory::AppMenuItemFactory(DesktopEntryLoader& desktopEntries) :
desktopEntries(desktopEntries) { }

/**
 * Create the folder menu item that defines the base menu folder
 */
AppMenuItem::Ptr AppMenuItemFactory::createBaseFolderItem()
{
    return setFactory(new BaseFolderMenuItem());
}

/**
 * Get an AppMenuItem for an application link provided by the AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create
(const AppConfigFile::AppItem& appItem)
{
    return setFactory(new ConfigAppMenuItem(appItem));
}

/**
 * Get an AppMenuItem for an application link that was read from a desktop
 * entry file
 */
AppMenuItem::Ptr AppMenuItemFactory::create(const DesktopEntry& desktopEntry)
{
    return setFactory(new DesktopEntryMenuItem(desktopEntry));
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create
(const AppConfigFile::AppFolder& appFolder)
{
    return setFactory(new FolderMenuItem(appFolder, desktopEntries));
}

