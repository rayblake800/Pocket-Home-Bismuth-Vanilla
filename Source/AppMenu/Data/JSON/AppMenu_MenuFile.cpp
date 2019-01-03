#define APPMENU_IMPLEMENTATION
#include "AppMenu_MenuFile.h"
#include "AppMenu_MenuJSON.h"

/*
 * Creates the MenuJSON resource on construction if necessary.
 */
AppMenu::MenuFile::MenuFile() { }

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenu::MenuItem AppMenu::MenuFile::getRootFolderItem()
{
    SharedResource::LockedPtr<MenuJSON> appJSON = getReadLockedResource();
    return appJSON->getRootFolderItem();
}

/*
 * Adds a new menu item to the list of items shown in a menu folder.
 */
AppMenu::MenuItem AppMenu::MenuFile::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        MenuItem& parentFolder,
        const int index) 
{
    SharedResource::LockedPtr<MenuJSON> appJSON = getWriteLockedResource();
    appJSON->addMenuItem(title, icon, command, launchInTerm, categories,
            parentFolder, index);
    return parentFolder.getFolderItem(index);
}

