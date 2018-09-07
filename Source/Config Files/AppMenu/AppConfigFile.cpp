#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "Utils.h"

/**
 * Gets the main list of application shortcuts. 
 */
juce::Array<AppItem> AppConfigFile::getFavorites()
{
    auto config = getReadLockedResource();
    return config->getFavorites();
}

/**
 * Add a new app to the list of pinned favorite apps in the config file.
 */
void AppConfigFile::addFavoriteApp
(AppItem newApp, int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addFavoriteApp(newApp,index,writeChangesNow);
}

/**
 * Remove an app from the list of favorite applications
 */
void AppConfigFile::removeFavoriteApp(int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->removeFavoriteApp(index,writeChangesNow);
}

/**
 * Find the index of an AppItem in favorites.
 */
int AppConfigFile::getFavoriteIndex(AppItem toFind)
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
void AppConfigFile::addAppFolder
(AppFolder newFolder, int index, bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addAppFolder(newFolder,index,writeChangesNow);
}

/**
 * Remove a folder from the list of AppFolders.
 */
void AppConfigFile::removeAppFolder(int index, bool writeChangesNow)
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


