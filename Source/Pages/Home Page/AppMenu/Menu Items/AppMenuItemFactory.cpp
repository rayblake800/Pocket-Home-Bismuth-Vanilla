#include "ConfigAppMenuItem.h"
#include "DesktopEntryMenuItem.h"
#include "FolderMenuItem.h"
#include "BaseFolderMenuItem.h"
#include "AppMenuItemFactory.h"

AppMenuItemFactory::AppMenuItemFactory(AppConfigFile& appConfig,
        MainConfigFile& mainConfig,
        DesktopEntries& desktopEntries) :
appConfig(appConfig),
mainConfig(mainConfig),
desktopEntries(desktopEntries) { }

/**
 * Create the folder menu item that defines the base menu folder
 */
AppMenuItem::Ptr AppMenuItemFactory::createBaseFolderItem()
{
    return new BaseFolderMenuItem(appConfig,
            mainConfig,
            *this);
}

/**
 * Get an AppMenuItem for an application link provided by the AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create
(const AppConfigFile::AppItem& appItem)
{
    return new ConfigAppMenuItem(appConfig, mainConfig, appItem);
}

/**
 * Get an AppMenuItem for an application link that was read from a desktop
 * entry file
 */
AppMenuItem::Ptr AppMenuItemFactory::create(const DesktopEntry& desktopEntry)
{
    return new DesktopEntryMenuItem(mainConfig, desktopEntry);
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile
 */
AppMenuItem::Ptr AppMenuItemFactory::create
(const AppConfigFile::AppFolder& appFolder)
{
    return new FolderMenuItem(appConfig,
            mainConfig,
            appFolder,
            *this,
            desktopEntries);
}

