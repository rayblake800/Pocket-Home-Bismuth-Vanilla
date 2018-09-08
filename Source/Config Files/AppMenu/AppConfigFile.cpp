#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "Utils.h"

/**
 * Gets the main list of application shortcuts. 
 */
juce::Array<AppShortcut> AppConfigFile::getShortcuts()
{
    auto config = getReadLockedResource();
    return config->getShortcuts();
}

/**
 * Add a new app to the list of pinned shortcuts in the config file.
 */
void AppConfigFile::addShortcut
(AppShortcut newApp, int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addShortcut(newApp,index,writeChangesNow);
}

/**
 * Remove a shortcut from the list of application shortcuts.
 */
void AppConfigFile::removeShortcut(int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->removeShortcut(index,writeChangesNow);
}

/**
 * Find the index of an AppShortcut in favorites.
 */
int AppConfigFile::getShortcutIndex(const AppShortcut& toFind)
{
    auto config = getReadLockedResource();
    return config->getFavoriteIndex(toFind);
}

/**
 * Gets the list of application menu folders.
 */
juce::Array<AppFolder> AppConfigFile::getFolders()
{
    auto config = getReadLockedResource();
    return config->getFolders();
}

/**
 * Add a new folder to the list of AppFolders in the config file.
 */
void AppConfigFile::addFolder
(AppFolder newFolder, int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addAppFolder(newFolder,index,writeChangesNow);
}

/**
 * Remove a folder from the list of AppFolders.
 */
void AppConfigFile::removeFolder(int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->removeAppFolder(index,writeChangesNow);
}

/**
 * Find the index of an AppFolder in the list of folders.
 */
int AppConfigFile::getFolderIndex(AppFolder toFind)
{
    auto config = getReadLockedResource();
    return config->getFolderIndex(toFind);
}


