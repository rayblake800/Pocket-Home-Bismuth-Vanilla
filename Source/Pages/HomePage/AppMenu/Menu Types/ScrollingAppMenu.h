/**
 * @file ScrollingAppMenu.h
 * 
 * ScrollingAppMenu displays an application menu as scrolling vertical
 * lists of menu items.
 */

#pragma once
#include "../AppMenuComponent.h"

class ScrollingAppMenu : public AppMenuComponent {
public:
    /**
     *
     * @param loadingSpinner
     */
    ScrollingAppMenu(OverlaySpinner& loadingSpinner);
    virtual ~ScrollingAppMenu();

private:
    /**
     * Use key presses for menu navigation
     * key events.
     * @param key
     * @param activeFolder
     * @return true if the key press was used.
     */
    bool folderKeyPressed(const KeyPress& key, AppMenuFolder* activeFolder) override;

    /**
     * Check to see if any changes have occurred that justifies changing
     * folder layout.  This does not need to account for the initial layout,
     * changes to menu bounds, folders opening and closing, and selection
     * of a new active folder, as all those events will update folder layout
     * without checking this value.
     * @return true iff the selected index changed.
     */
    bool layoutChanged(const AppMenuFolder* activeFolder) override;


    /**
     * Return the bounds where the given folder should be placed in the menu.
     * @param folder
     * @param folderIndex
     * @return 
     */
    Rectangle<int> updateFolderBounds(const AppMenuFolder* folder,
            int folderIndex) override;

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    AppMenuFolder* createFolderObject
    (AppMenuItem::Ptr folderItem,
            std::map<String, AppMenuButton::Ptr>& buttonMap,
            IconThread& iconThread) override;
    
    //Define animation duration for scrolling appMenus
    const int scrollingAnimationDuration = 100;
};
