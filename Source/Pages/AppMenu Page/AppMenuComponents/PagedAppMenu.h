
/**
 * @file PagedAppMenu
 * 
 * TODO: pretty much everything
 */

#pragma once
#include "AppMenuComponent.h"


class PagedAppMenu : public AppMenuComponent {
public:
    PagedAppMenu(AppConfigFile& appConfig);
    virtual ~PagedAppMenu();

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
     * @return true iff the key press was used by the AppMenu
     */
    virtual bool keyPressed(const KeyPress& key) override;

private:
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem* menuItem, int rowIndex, int columnIndex) override;

    virtual void resized() override;

    class PagedMenuButton : public AppMenuButton{
    public:
        PagedMenuButton(AppMenuItem* menuItem, IconThread& iconThread,
                int columnIndex, int rowIndex, String name = String());
        virtual ~PagedMenuButton();
    private:
        virtual void resized() override;
        int columnIndex;
        int rowIndex;
    };
};
