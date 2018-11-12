#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "AppMenu/Data/JSON/ConfigFile.h"
/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenu::MenuItem AppMenu::ConfigFile::getRootFolderItem()
{
    using namespace juce;
    auto appJSON = getReadLockedResource();
    return appJSON->getRootFolderItem();
}

/*
 * Adds a new menu item to the list of items shown in a menu folder.
 */
AppMenu::MenuItem AppMenu::ConfigFile::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        MenuItem& parentFolder,
        const int index) 
{
    auto appJSON = getWriteLockedResource();
    appJSON->addMenuItem(title, icon, command, launchInTerm, categories,
            parentFolder, index);
    return parentFolder.getFolderItem(index);
}

AppMenu::ConfigFile::Listener::Listener() : 
    Config::FileResource::Listener(JSONResource::resourceKey, 
            []()->SharedResource* { return new JSONResource(); }) { }
