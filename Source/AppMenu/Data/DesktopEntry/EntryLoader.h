/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/AppMenu.h"
#include "DesktopEntry/DesktopEntry.h"

/**
 * @file  AppMenu/Data/DesktopEntry/EntryLoader.h 
 *
 * @brief  Finds and adds desktop entry menu items to menu folders.
 *
 * Each menu folder may contain a list of application categories. When a menu
 * folder is initialized, an EntryLoader is used to find all desktop entry
 * file objects with matching categories, and add them to the folder as child
 * menu items.
 */
class AppMenu::EntryLoader
{         
public:
    EntryLoader() { }

    /**
     * @brief  Cancels any pending DesktopEntry::Loader callbacks the loader
     *         created.
     */
    virtual ~EntryLoader();

    /**
     * @brief  Loads all desktop entry menu items for the first time.
     */
    void initialEntryLoad();

    /**
     * @brief  Loads and adds all desktop entry menu items that belong in a
     *         menu folder item, or in any of its child folder items.
     *
     * @param folderItem  The folder menu item to update.
     */
    void loadFolderEntries(MenuItem folderItem);

private:
    /* Holds the loading callback ID if waiting for the desktop entry thread to
       load entry files. */
    DesktopEntry::CallbackID initialLoadingID = 0;
};
/* Only include this file directly in the AppMenu implementation! */
#endif
