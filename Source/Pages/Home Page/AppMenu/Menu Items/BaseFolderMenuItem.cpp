#include "AppMenuItemFactory.h"
#include "BaseFolderMenuItem.h"


/**
 * Check if this button is for an application folder
 * 
 * @return true
 */
bool BaseFolderMenuItem::isFolder() const
{
    return true;
}

/**
 * @return all menu items in this folder
 */
Array<AppMenuItem::Ptr> BaseFolderMenuItem::getFolderItems() const
{
    AppConfigFile config;
    Array<AppMenuItem::Ptr> folderItems;

    Array<AppConfigFile::AppItem> favorites = config.getFavorites();
    for (const AppConfigFile::AppItem& app : favorites)
    {
        folderItems.add(create(app));
    }

    Array<AppConfigFile::AppFolder> folders = config.getFolders();
    for (const AppConfigFile::AppFolder& folder : folders)
    {
        folderItems.add(create(folder));
    }
    DBG("BaseFolderMenuItem::" << __func__ << ": created " << folderItems.size()
            << " folder items.");
    return folderItems;
}
