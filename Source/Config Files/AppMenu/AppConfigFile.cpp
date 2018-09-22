#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "Utils.h"

/*
 * Gets the main list of application shortcuts. 
 */
juce::Array<AppShortcut> AppConfigFile::getShortcuts()
{
    auto config = getReadLockedResource();
    return config->getShortcuts();
}

/*
 * Adds a new shortcut to the list of shortcuts shown in the AppMenu's main 
 * folder.
 */
void AppConfigFile::addShortcut
(const AppShortcut& shortcut, const int index, const bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addShortcut(shortcut,index,writeChangesNow);
}

/*
 * Removes a shortcut from the list of application shortcuts.
 */
void AppConfigFile::removeShortcut(const int index, const bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->removeShortcut(index,writeChangesNow);
}

/*
 * Finds the index of an application shortcut in the list.
 */
int AppConfigFile::getShortcutIndex(const AppShortcut& toFind)
{
    auto config = getReadLockedResource();
    return config->getShortcutIndex(toFind);
}

/*
 * Gets the list of application menu folders.
 */
juce::Array<AppFolder> AppConfigFile::getFolders()
{
    auto config = getReadLockedResource();
    return config->getFolders();
}

/*
 * Adds a new folder to show in the AppMenu.
 */
void AppConfigFile::addFolder(const AppFolder& newFolder, const int index, 
        const bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->addAppFolder(newFolder,index,writeChangesNow);
}

/*
 * Removes a folder from the list of AppFolders.
 */
void AppConfigFile::removeFolder(const int index, const bool writeChangesNow)
{
    auto config = getWriteLockedResource();
    config->removeAppFolder(index,writeChangesNow);
}

/*
 * Finds the index of an AppFolder in the list of folders.
 */
int AppConfigFile::getFolderIndex(const AppFolder& toFind)
{
    auto config = getReadLockedResource();
    return config->getFolderIndex(toFind);
}


