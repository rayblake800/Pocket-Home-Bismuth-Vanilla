// Disabled until redesign
#if 0
#pragma once
#include "AppMenuComponent.h"

/**
 * @file ScrollingAppMenu.h
 * 
 * @brief ScrollingAppMenu displays an application menu as scrolling vertical
 *        lists of menu items.
 */

class ScrollingAppMenu : public AppMenuComponent
{
public:
    /**
     * @param loadingSpinner   Reference to an overlay spinner that sits over
     *                         the PageComponent holding this AppMenuComponent.
     */
    ScrollingAppMenu(OverlaySpinner& loadingSpinner);

    virtual ~ScrollingAppMenu() { }

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
    bool folderKeyPressed(const juce::KeyPress& key, 
            AppMenuFolder* activeFolder) override;

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
    juce::Rectangle<int> updateFolderBounds(const AppMenuFolder* folder,
            int folderIndex) override;

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
    virtual AppMenuFolder* createFolderObject(const AppMenuItem& folderItem,
            std::map<juce::String, AppMenuButton::Ptr>& buttonMap) override;
};

//Disabled until redesign
#endif
