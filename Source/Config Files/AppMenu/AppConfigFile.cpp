#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "Utils.h"

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenuItem AppConfigFile::getRootFolderItem()
{
    using namespace juce;
    auto appJSON = getReadLockedResource();
    return appJSON->getRootFolderItem();
}

/*
 * Adds a new menu item to the list of items shown in a menu folder.
 */
AppMenuItem AppConfigFile::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        AppMenuItem& parentFolder,
        const int index) 
{
    auto appJSON = getWriteLockedResource();
    appJSON->addMenuItem(title, icon, command, launchInTerm, categories,
            parentFolder, index);
    return parentFolder.getFolderItem(index);
}
