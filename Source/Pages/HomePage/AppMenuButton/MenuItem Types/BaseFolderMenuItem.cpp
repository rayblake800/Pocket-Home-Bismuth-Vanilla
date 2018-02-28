#include "BaseFolderMenuItem.h"

BaseFolderMenuItem::BaseFolderMenuItem(const DesktopEntries& desktopEntries) :
desktopEntries(desktopEntries) { }

BaseFolderMenuItem::~BaseFolderMenuItem() { }

/**
 * Check if this button is for an application folder
 * @return true
 */
bool BaseFolderMenuItem::isFolder()
{
    return true;
}

/**
 * @return all menu items in this folder
 */
Array<AppMenuItem::Ptr> BaseFolderMenuItem::getFolderItems() const { 
    AppConfigFile config;
    Array<AppMenuItem::Ptr> folderItems;
    
    Array<AppConfigFile::AppItem> favorites = config.getFavorites();
    for(const AppConfigFile::AppItem& app : favorites)
    {
		folderItems.add(AppMenuItemFactory::create(app));
	}
    
    Array<AppConfigFile::AppFolder> folders = config.getFolders();
    for(const AppConfigFile::AppFolder& folder : folders)
    {
		folderItems.add(AppMenuItemFactory::create(folder));
	}
	return folderItems; 
}
