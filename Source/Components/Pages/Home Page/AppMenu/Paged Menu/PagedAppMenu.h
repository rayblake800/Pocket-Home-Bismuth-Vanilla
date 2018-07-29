#pragma once
#include "NavButton.h"
#include "AppMenuComponent.h"

/**
 * @file PagedAppMenu.h
 * 
 * @brief PagedAppMenu displays all applications in a grid, spreading them across
 *        multiple pages.  
 * 
 * Only one page of one folder is visible at once. Touch
 * navigation buttons or hardware keys can be used to open or close folders,
 * and switch between folder pages.
 */

class PagedAppMenu : public AppMenuComponent, public juce::Button::Listener
{
public:
    /**
     * @param loadingSpinner   Reference to an overlay spinner that sits over
     *                         the PageComponent holding this AppMenuComponent.
     */
    PagedAppMenu(OverlaySpinner& loadingSpinner);

    virtual ~PagedAppMenu() { }

    /**
     * Update navigation buttons when the menu changes size.
     */
    void menuResized() override;

private:
    /**
     * Uses key events to navigate through the menu.
     * 
     * @param key            Represents a key pressed by the user.
     * 
     * @param activeFolder   The current open menu folder.
     * 
     * @return true iff the key press was used.
     */
    bool folderKeyPressed
    (const juce::KeyPress& key, AppMenuFolder* activeFolder) override;

    /**
     * Creates a folder component object from a folder menu item.
     * 
     * @param folderItem  A folder menu item that provides the new folder's menu
     *                    items.
     * 
     * @param buttonMap   A map of all previously created menu buttons, used
     *                    to re-load previously created menu buttons.  Any new
     *                    buttons created for this folder will be added to the
     *                    button map.
     */
    AppMenuFolder* createFolderObject(
            AppMenuItem::Ptr folderItem,
            std::map<juce::String, AppMenuButton::Ptr>& buttonMap) override;

    /**
     * Returns the bounds where the given folder should be placed in the menu.
     * 
     * @param folder       An open menu folder object.
     * 
     * @param folderIndex  The folder's index in the list of open folders.
     * 
     * @return  a rectangle specifying the location where the folder should be
     *          placed within the AppMenuComponent. 
     */
    virtual juce::Rectangle<int> updateFolderBounds
    (const AppMenuFolder* folder, int folderIndex) override;

    /**
     * Handles navigation button controls.
     * 
     * @param button   The button pressed by the user.  This should be pageLeft,
     *                 pageRight, or closeFolderBtn
     */
    void buttonClicked(juce::Button* button) override;

    //folder navigation key bindings
    static const juce::String pageLeftBinding;
    static const juce::String pageRightBinding;
    //navigation buttons
    NavButton closeFolderBtn;
    NavButton pageLeft;
    NavButton pageRight;
};
