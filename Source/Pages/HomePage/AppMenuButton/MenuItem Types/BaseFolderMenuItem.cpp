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
Array<AppMenuItem:> BaseFolderMenuItem::getFolderItems() const { 
    AppConfigFile config;
    
}