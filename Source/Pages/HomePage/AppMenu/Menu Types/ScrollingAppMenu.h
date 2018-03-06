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

    /**
     * Use key presses for menu navigation, setting specific controls based on 
     * AppMenu type. Other classes may call this to pass on or simulate
     * key events.
     * @param key
     * @return true if the key press was used.
     */
    bool keyPressed(const KeyPress& key);

protected:
    /**
     * Updates the folder component layout, optionally animating the transition.
     * @param animateTransition if true, animate component changes rather than
     * immediately updating folder bounds.
     */
    void layoutFolders(bool animateTransition);

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    AppMenuFolder* createFolderObject(AppMenuItem::Ptr folderItem); 
};
