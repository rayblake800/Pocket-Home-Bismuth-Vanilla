/**
 * @file ScrollingAppMenu.h
 * 
 * TODO finish documentation
 */

#pragma once
#include "AppMenuComponent.h"

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
    AppFolder* createFolderObject(AppMenuItem::Ptr folderItem);

private:

    /**
     *
     */
    class ScrollingAppFolder : public AppFolder {
    public:
        ScrollingAppFolder();
        virtual ~ScrollingAppFolder();

        /**
         * Create AppMenuButton components for a set of AppMenuItems.
         * @param menuItems
         */
        Array<AppMenuButton::Ptr> createMenuButtons
        (Array<AppMenuItem::Ptr> menuItems);

        /**
         * Given a list of folder buttons, return an appropriate layout
         * for positioning them in the folder component.
         * 
         * @param buttons
         * @return a Layout containing all items in the button array.
         */
        virtual GridLayoutManager::Layout buildFolderLayout
        (Array<AppMenuButton::Ptr> buttons);
    };

    /**
     *
     */
    class ScrollingMenuButton : public AppMenuButton {
    public:
        /**
         * 
         * @param menuItem
         * @param iconThread
         * @param name
         */
        ScrollingMenuButton(AppMenuItem* menuItem, IconThread& iconThread,
                String name = String());
        virtual ~ScrollingMenuButton();
    private:
        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;
    };
};
