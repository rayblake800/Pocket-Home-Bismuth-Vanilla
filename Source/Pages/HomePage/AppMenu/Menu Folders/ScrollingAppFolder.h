/*
 * @file ScrollingAppFolder.h
 * 
 * ScrollingAppFolder is a menu folder component that arranges menu buttons
 * into a vertical scrolling list.
 */
#pragma once
#include "../AppMenuFolder.h"

class ScrollingAppFolder : public AppMenuFolder {
public:
    ScrollingAppFolder(AppMenuItem::Ptr folderItem, MouseListener* btnListener,
            std::map<String, AppMenuButton::Ptr>& buttonNameMap,
            IconThread& iconThread);
    virtual ~ScrollingAppFolder();

    /**
     * Create an AppMenuButton component for an AppMenuItem.
     * @param menuItem
     */
    AppMenuButton::Ptr createMenuButton(AppMenuItem::Ptr menuItem);

    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * @return a Layout containing all items in the button array.
     */
    virtual GridLayoutManager::Layout buildFolderLayout
    (Array<AppMenuButton::Ptr>& buttons);

    /**
     * @return the width in pixels of the longest menu item title in
     * the folder.
     */
    int getTextWidth();
private:
    int maxTextWidth = 0;

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

        /**
         * @return the width in pixels of this button's title
         */
        int getTitleWidth();

    private:
        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollingMenuButton)
    };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollingAppFolder)
};

