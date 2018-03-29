/*
 * @file ScrollingAppFolder.h
 * 
 * ScrollingAppFolder is a menu folder component that arranges menu buttons
 * into a vertical scrolling list.
 */
#pragma once
#include "AppMenuFolder.h"

class ScrollingAppFolder : public AppMenuFolder
{
public:
    /**
     * @param folderItem     Menu item used to load all of this folder's menu
     *                        items.
     * 
     * @param btnListener    A listener to assign to all menu buttons.
     * 
     * @param buttonNameMap  Shared button map used to recycle menu buttons.
     * 
     * @param config         Component settings needed to setup folder 
     *                        components.
     */
    ScrollingAppFolder(
            AppMenuItem::Ptr folderItem,
            MouseListener* btnListener,
            std::map<String, AppMenuButton::Ptr>& buttonNameMap,
            ComponentConfigFile& config);

    virtual ~ScrollingAppFolder() { }

    /**
     * Create an AppMenuButton component for an AppMenuItem.
     * 
     * @param menuItem
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem::Ptr menuItem) override;

    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * 
     * @return a Layout containing all items in the button array.
     */
    virtual RelativeLayoutManager::Layout buildFolderLayout
    (Array<AppMenuButton::Ptr>& buttons);


    /**
     * @return the minimum width, in pixels, needed by this folder to
     *          display its contents properly. 
     */
    int getMinimumWidth() override;

private:
    /**
     * Get the font used by all buttons in this menu type.
     */
    static Font getButtonFont(ComponentConfigFile& config);

    ComponentConfigFile& config;

    /**
     * Custom AppMenuButton proportioned correctly for a scrolling menu.
     */
    class ScrollingMenuButton : public AppMenuButton
    {
    public:
        /**
         * @param menuItem     Provides the button's menu data.
         *  
         * @param name         Internal component name
         * 
         * @param config       
         */
        ScrollingMenuButton(
                AppMenuItem* menuItem,
                String name,
                ComponentConfigFile& config);

        virtual ~ScrollingMenuButton() { }

        /**
         * @return the width in pixels of this button's title
         */
        int getTitleWidth();

    private:
        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;

        ComponentConfigFile& config;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollingMenuButton)
    };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScrollingAppFolder)
};

