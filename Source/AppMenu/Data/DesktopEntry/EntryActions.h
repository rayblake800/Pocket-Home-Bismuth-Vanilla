/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/AppMenu.h"
#include "AppMenu/Data/MenuItem.h"
#include "DesktopEntry/Loader.h"

/**
 * @file  AppMenu/Data/DesktopEntry/EntryActions.h
 *
 * @brief  Provides functions for adding and updating desktop entry data in 
 *         the AppMenu, shared by AppMenu::EntryLoader and 
 *         AppMenu::EntryUpdater.
 */
namespace AppMenu
{
    namespace EntryActions
    {
        /**
         * @brief  Recursively applies a function to a menu folder and all its
         *         subfolders.
         *
         * @param startingFolder  The initial folder to search.
         *
         * @param folderAction    An action to apply to each folder item.
         *
         */
        void recursiveFolderAction(MenuItem startingFolder,
                const std::function<void(MenuItem)> folderAction);

        /**
         * @brief  Applies a function to each desktop entry in a list that 
         *         shares categories with a folder menu item.
         *
         * @param folder        A folder menu item.
         *
         * @param entries       A list of desktop entries.
         *
         * @param entryAction   A function to call for each entry that shares a 
         *                      category with the folder.
         */
        void foreachMatchingEntry(MenuItem folder,
                const juce::Array<DesktopEntry::EntryFile>& entries,
                std::function<void(const DesktopEntry::EntryFile&)> 
                    entryAction);

        /**
         * @brief  Adds folder items created from desktop entry files to a 
         *         folder if the folder and the entry share application 
         *         categories.
         *
         * @param folder   A folder menu item.
         *
         * @param entries  A list of entries that may need to be added to the 
         *                 folder as new menu items.
         */
        void addEntryItems(MenuItem folder,
                const juce::Array<DesktopEntry::EntryFile>& entries);
        
        /**
         * @brief  Applies desktop entry updates to all matching menu items in a
         *         folder.
         *
         * @param folder   A folder menu item.
         *
         * @param entries  A list of entries to attempt to find and update 
         *                 within the folder.
         */
        void updateEntryItems(MenuItem folder,
            const juce::Array<DesktopEntry::EntryFile>& entries);

        /**
         * @brief  Gets an array containing all desktop entry menu items in a 
         *         folder.
         *
         * @param folder      A folder menu item to scan.
         *
         * @return            All of the folder's desktop entry menu items in 
         *                    order.
         */
        juce::Array<MenuItem> getDesktopEntryItems(const MenuItem folder);
    }
}
/* Only include this file directly in the AppMenu implementation! */
#endif
