/**
 * @file ScrollingAppMenu.h
 * 
 * TODO finish documentation
 */

#pragma once
#include "AppMenuComponent.h"

class ScrollingAppMenu : public AppMenuComponent {
public:
    ScrollingAppMenu();
    virtual ~ScrollingAppMenu();

    /**
     * Add a new application button to the active menu column.
     * @param appButton will be added to the bottom of the active menu column.
     */
    virtual void addButtonComponent(AppMenuButton* appButton) override;

    /**
     * Scroll the menu so that the selected button is centered.
     * @param animatedScroll sets if the menu should animate its scrolling,
     * or just jump immediately to the destination position.
     */
    virtual void scrollToSelected(bool animatedScroll = true) override;

    /**
     * Receives all keyPress events and uses them for page navigation.
     * @param key
     * @return true iff the key press was used by the AppMenu
     */
    virtual bool keyPressed(const KeyPress& key) override;

private:
    /**
     * Create a new menu button component.
     * @param menuItem menu data to be held by the component
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem* menuItem, int rowIndex, int columnIndex) override;

    /**
     * Resize all child components.
     */
    virtual void resized() override;

    //top y-position of each open column
    std::vector<int> columnTops;
    //base component position
    int x_origin;
    int y_origin;

    /**
     * ScrollingMenuButton is an AppMenuButton that draws itself appropriately
     * for a ScrollingAppMenu.
     */
    class ScrollingMenuButton : public AppMenuButton{
    public:
        ScrollingMenuButton(AppMenuItem* menuItem, IconThread& iconThread,
                int columnIndex, int rowIndex, String name = String());
        virtual ~ScrollingMenuButton();
    private:
        /**
         * Re-calculates draw values whenever the button is resized
         */
        void resized() override;

        
        int columnIndex;
        int rowIndex;
    };
};
