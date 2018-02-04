#include "AppMenuItemFactory.h"

AppMenuItemFactory::AppMenuItemFactory()
{
}

AppMenuItemFactory::~AppMenuItemFactory()
{
}

/**
 * Get an AppMenuItem for an application link provided by the AppConfigFile
 */
AppMenuItem* AppMenuItemFactory::create
(AppConfigFile::AppItem appItem, AppConfigFile& config)
{
    return new ConfigAppMenuItem(appItem, config);
}

/**
 * Get an AppMenuItem for an application link that was read from a desktop
 * entry file
 */
AppMenuItem* AppMenuItemFactory::create(DesktopEntry desktopEntry)
{
    return new DesktopEntryMenuItem(desktopEntry);
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile
 */
AppMenuItem* AppMenuItemFactory::create
(AppConfigFile::AppFolder appFolder, AppConfigFile& config)
{
    return new FolderMenuItem(appFolder, config);
}