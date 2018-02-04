/*
  ==============================================================================

    ScrollingAppMenu.h
    Created: 2 Feb 2018 9:48:24pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "AppMenuComponent.h"

class ScrollingAppMenu : public AppMenuComponent {
public:
    ScrollingAppMenu(AppConfigFile& appConfig);
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

private:
    /**
     * Create a new menu button component.
     * @param menuItem menu data to be held by the component
     */
    virtual  AppMenuButton::Ptr createMenuButton
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
};
