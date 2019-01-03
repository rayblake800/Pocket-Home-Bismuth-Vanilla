#define APPMENU_IMPLEMENTATION
#include "AppMenu_EntryLoader.h"
#include "AppMenu_EntryData.h"
#include "AppMenu_EntryActions.h"
#include "AppMenu_MenuFile.h"
#include "AppMenu_MenuItem.h"
#include "DesktopEntry/Loader.h"

/*
 * Cancels any pending DesktopEntry::Loader callbacks the loader created.
 */
AppMenu::EntryLoader::~EntryLoader()
{    
    if(initialLoadingID != 0)
    {
        DesktopEntry::Loader entryLoader;
        entryLoader.clearCallback(initialLoadingID);
        initialLoadingID = 0;
    }
}

/*
 * Loads all desktop entry menu items for the first time.
 */
void AppMenu::EntryLoader::initialEntryLoad(){
    AppMenu::MenuFile appConfig;
    MenuItem rootFolder = appConfig.getRootFolderItem();
    loadFolderEntries(rootFolder);
}

/*
 * Loads and adds all desktop entry menu items that belong in a menu folder 
 * item, or in any of its child folder items.
 */
void AppMenu::EntryLoader::loadFolderEntries(MenuItem folderItem) 
{
    DesktopEntry::Loader entryLoader;
    initialLoadingID = entryLoader.waitUntilLoaded([this, folderItem]()
    {
        // The folder should have zero desktop entry items at this point!
        jassert(folderItem.getMovableChildCount() 
                == folderItem.getFolderSize());

        DesktopEntry::Loader entryLoader;
        juce::Array<DesktopEntry::EntryFile> allEntries 
            = entryLoader.getAllEntries();
        EntryActions::recursiveFolderAction(folderItem, [this, &allEntries]
            (MenuItem folder)
        {
            EntryActions::addEntryItems(folder, allEntries);
        });
    });
}
