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
juce::Array<AppMenuItem::Ptr> BaseFolderMenuItem::getFolderItems() const
{
    using namespace juce;
    AppConfigFile config;
    Array<AppMenuItem::Ptr> folderItems;

    Array<AppShortcut> shortcuts = config.getShortcuts();
    for (const AppShortcut& app : shortcuts)
    {
        folderItems.add(create(app));
    }

    Array<AppFolder> folders = config.getFolders();
    for (const AppFolder& folder : folders)
    {
        folderItems.add(create(folder));
    }
    //DBG("BaseFolderMenuItem::" << __func__ << ": created " << folderItems.size()
    //        << " folder items.");
    return folderItems;
}
