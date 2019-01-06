#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif

#pragma once
/**
 * @file  AppMenu_EntryUpdater.h
 *
 * @brief  Listens for changes to desktop entries, copying all changes over to 
 *         the menu.
 */

#include "DesktopEntry_UpdateListener.h"

namespace AppMenu { class EntryUpdater; }

/**
 *  Whenever the desktop entry loading thread checks for updates to desktop 
 * entry files, it sends each DesktopEntry::UpdateListener lists of all desktop 
 * file IDs that were added, updated, or removed. The EntryUpdater applies these
 * changes to the AppMenu::MenuItem menu tree. 
 *
 *  When new entries are found, the updater adds them as menu items to all 
 * folders that share application categories with the new entry. When existing 
 * entries are changed, the updater copies those changes to all menu items 
 * created from those entries, potentially adding or removing the items from 
 * folders if the entry's application categories were changed. When entries are
 * removed, the updater removes all menu items created from those entries from
 * all folders.
 */
class AppMenu::EntryUpdater : public DesktopEntry::UpdateListener
{         
public:
    EntryUpdater() { }

    virtual ~EntryUpdater() { }

private:
    /**
     * @brief  Checks new desktop entries to see if they should be added to the
     *         menu.
     *
     * @param entryFileIDs  The desktop file IDs of all new desktop entries.
     */
    virtual void entriesAdded(const juce::StringArray entryFileIDs) override;
    
    /**
     * @brief  Removes desktop entry menu buttons when their desktop entry files
     *         are deleted or hidden.
     *
     * @param entryFileIDs  The desktop file IDs of all removed or hidden
     *                      desktop entries.
     */
    virtual void entriesRemoved(const juce::StringArray entryFileIDs) override;
    
    /**
     * @brief  Refreshes desktop entry menu buttons when their desktop entry
     *         files change.
     *
     * @param entryFileIDs  The desktop file IDs of all updated desktop entries.
     */
    virtual void entriesUpdated(const juce::StringArray entryFileIDs) override;
};
