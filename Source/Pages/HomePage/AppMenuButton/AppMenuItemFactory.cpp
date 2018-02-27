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
AppMenuItem* AppMenuItemFactory::create(const AppConfigFile::AppItem& appItem)
{
    return new ConfigAppMenuItem(appItem);
}

/**
 * Get an AppMenuItem for an application link that was read from a desktop
 * entry file
 */
AppMenuItem* AppMenuItemFactory::create(const DesktopEntry& desktopEntry)
{
    return new DesktopEntryMenuItem(desktopEntry);
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile
 */
AppMenuItem* AppMenuItemFactory::create(const AppConfigFile::AppFolder& appFolder)
{
    return new FolderMenuItem(appFolder);
}