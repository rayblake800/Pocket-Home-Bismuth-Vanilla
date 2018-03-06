/**
 * @file PagedAppMenu.h
 * 
 * TODO finish documentation
 */

#pragma once
#include "../../../../Configuration/Configurables/ConfigurableImageButton.h"
#include "../AppMenuComponent.h"

class PagedAppMenu : public AppMenuComponent, public Button::Listener {
public:
    /**
     * @param loadingSpinner
     */
    PagedAppMenu(OverlaySpinner& loadingSpinner);
    virtual ~PagedAppMenu();

    /**
     * Use key presses for menu navigation, setting specific controls based on 
     * AppMenu type. Other classes may call this to pass on or simulate
     * key events.
     * @param key
     * @return true if the key press was used.
     */
    bool keyPressed(const KeyPress& key) override;

protected:
    /**
     * Updates the folder component layout, optionally animating the transition.
     * @param animateTransition if true, animate component changes rather than
     * immediately updating folder bounds.
     */
    virtual void layoutFolders(bool animateTransition) override;

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    AppMenuFolder* createFolderObject(AppMenuItem::Ptr folderItem) override;

private:
    /**
     * Handles navigation button controls
     * @param 
     */
    void buttonClicked(Button*) override;

    //navigation buttons
    ConfigurableImageButton closeFolderBtn;
    ConfigurableImageButton pageLeft;
    ConfigurableImageButton pageRight;


    //############################ PageAppFolder ###############################

    
};
