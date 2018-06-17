#pragma once
#include <set>
#include <atomic>
#include "JuceHeader.h"
#include "OverlaySpinner.h"
#include "ConfigurableComponent.h"
#include "AppLauncher.h"
#include "LayoutManager.h"
#include "WindowFocus.h"
#include "IconThread.h"
#include "MainConfigFile.h"
#include "AppConfigFile.h"
#include "DesktopEntryLoader.h"
#include "AppMenuPopupEditor.h"
#include "AppMenuItemFactory.h"
#include "AppMenuFolder.h"
#include "AppMenuButton.h"

/** 
 * @file   AppMenuComponent.h
 *
 * @brief AppMenuComponent is a scrolling application menu that loads its
 * contents from system .Desktop files and an AppConfigFile object.
 * 
 * The menu is divided into folders, which can contain different sets of menu 
 * buttons to launch applications or open sub-folders.  The user can navigate
 * through these folders and select menu items to launch applications or open
 * new folders.  The menu also provides controls for adding, editing, and 
 * deleting menu entries.
 */


class AppMenuComponent : public Component, public ConfigurableComponent,
private WindowFocus::Listener
{
public:
    /**
     * @param componentKey     Loads menu bounds from config.
     * 
     * @param loadingSpinner   Reference to an overlay spinner that sits over
     *                         the PageComponent holding this AppMenuComponent.
     */
    AppMenuComponent(String componentKey, OverlaySpinner& loadingSpinner);

    virtual ~AppMenuComponent();

    /**
     * Checks if the menu is currently in the loading state, where it is busy
     * loading menu items or launching an application.
     * 
     * @return true if the AppMenuComponent is currently in the loading state.
     */
    bool isLoading() const;

    /**
     * Exit the loading state, hiding the spinner and enabling
     * user input
     */
    void exitLoadingState();

    /**
     * Open the pop-up menu used for editing menu items.
     *
     * @param selectedButton  If null, open a generic folder editing
     *                         menu. otherwise, open the menu for editing this 
     *                         particular menu button.
     */
    void openPopupMenu(AppMenuButton::Ptr selectedButton);

    /**
     * Respond to key events if input is not being ignored.
     * This opens the pop-up menu or reloads the AppMenu if the correct 
     * shortcuts are detected, otherwise it lets the AppMenuComponent's subclass
     * determine how to handle the key event.
     *
     * @param key
     * 
     * @return true if the keyPress is handled by the menu, false otherwise.
     */
    bool keyPressed(const KeyPress& key) override;

    /**
     * This method will run whenever this component is resized.  Classes
     * inheriting AppMenuComponent should override this instead of 
     * void resized(). This method will run before the menu folders are updated
     * for the new bounds.
     */
    virtual void menuResized() { }

protected:
    /**
     * Load and display the base menu folder that contains favorite 
     * application shortcuts and all other folders. Any existing open folders
     * will first be closed.  If the base folder was open, its selected 
     * index will be saved and re-selected.
     */
    void loadBaseFolder();

    /**
     * Close the active folder, removing it and all folders after it from
     * the menu component.
     */
    void closeFolder();

    /**
     * Closes the active folder, unless the active folder is at index 0,
     * in which case it will reload the folder.
     */
    void closeNonBaseFolder();

    /**
     * Sets which open folder is currently focused.  This value will change to
     * the index of the last open folder whenever a folder opens or closes.
     *
     * @param folder  This must be a valid folder index, or the active folder 
     *                 index will not change.
     */
    void setActiveFolderIndex(int folder);

    /**
     * Get the index of the active folder.  AppMenuComponents should center the
     * folder layout on the active folder, and apply key controls to that
     * folder's menu items.
     *
     * @return the active folder index, usually the last open folder.
     *          Returns 0 if no folders are open.
     */
    int getActiveFolderIndex() const;

    /**
     * Trigger a click for the selected button in the active AppFolder
     */
    void clickSelected();

    /**
     * Open a PopupEditorComponent for editing the selected button in the active
     * AppFolder.
     */
    void openEditorForSelected();


    /**
     * @return the number of currently open folder components
     */
    int getNumFolders() const;

    /**
     * Gets the menu size of an open folder.
     *
     * @param index  The folder index.
     *
     * @return the number of buttons in the folder at this index, or -1
     *          if there is no folder at this index.
     */
    int getFolderSize(int index);

    /**
     * Get the minimum permitted component width of an open folder.
     *
     * @param index  The folder index.
     * 
     * @return the minimum width, in pixels, that the folder at this index
     *          should have, or -1 if there is no folder at this index.
     */
    int getMinimumFolderWidth(int index);

    /**
     * Gets the selected menu index of an open folder.
     *
     * @param index  The folder index.
     *
     * @return  the selected menu button index for the folder, or -1 if
     *           the folder at the index parameter doesn't exist or has no 
     *           selected menu button.
     */
    int getFolderSelectedIndex(int index) const;

    /**
     * Updates the folder component layout, optionally animating the transition.
     * 
     * @param animate  Iff true, the folder transition will be animated.
     */
    void layoutFolders(bool animate = false);

    /**
     * @return the maximum number of menu item columns to show on screen.
     */
    int getMaxColumns() const;

    /**
     * @return the maximum number of menu item rows to show on screen.
     */
    int getMaxRows() const;

    /**
     * @return the maximum number of menu items to show on screen
     */
    inline int buttonsPerPage() const
    {
        return getMaxColumns() * getMaxRows();
    }

    /**
     * Sets how unselected menu buttons respond to clicks.
     *
     * @param newVal if set to true, clicking unselected menu buttons 
     *         only selects them. If set to false, clicking them also
     *         immediately launches their application or opens their folder.
     *         This is set to true by default.
     */
    void setOnlyTriggerSelected(bool newVal);
    
    
    /**
     * Check if input should be ignored.
     *
     * @return true iff the menu is loading, empty, animating, or otherwise
     *          in a state where user input should be ignored.
     */
    bool ignoringInput() const;

private:
    /**
     * Create a folder component object from a folder menu item.
     *
     * @param folderItem  Provides all folder menu items.
     *
     * @param buttonMap   A map of all created buttons by name. This is used 
     *                     by the folder to recycle menu buttons.
     */
    virtual AppMenuFolder* createFolderObject(
            AppMenuItem::Ptr folderItem,
            std::map<String, AppMenuButton::Ptr>& buttonMap) = 0;

    /**
     * Return the bounds where the given folder should be placed in the menu.
     *
     * @param folder       One of the AppMenu folder components.
     *
     * @param folderIndex  The folder's index in the AppMenu.
     *
     * @return  updated folder bounds to apply.
     */
    virtual Rectangle<int> updateFolderBounds(const AppMenuFolder* folder,
            int folderIndex) = 0;

    /**
     * Uses key events to control the menu in whatever way is appropriate
     * for the specific AppMenuComponent.
     *
     * @param key
     * 
     * @param activeFolder  The current active folder component.
     * 
     * @return  true iff the key press was used.
     */
    virtual bool folderKeyPressed
    (const KeyPress& key, AppMenuFolder* activeFolder) = 0;

    /**
     * Exits the loading state if the window loses focus.
     */
    void windowFocusLost() override;

    /**
     * Updates the layout if row/column size changes.
     *
     * @param key     the key of property that has changed
     */
    void extraConfigValueChanged(String key) final override;

    /**
     * Reposition child components, and update folder layout without animating.
     */
    void resized() final override;

    /**
     * Open an application category folder, creating or adding 
     * AppMenuButtons for all associated desktop applications.
     * If any folders after the active folder are already open, they
     * will first be closed.
     * 
     * @param folderItem  This defines the folder and provides all 
     *                     AppMenuItem objects.
     */
    void openFolder(AppMenuItem::Ptr folderItem);

    /**
     * @return the selected button in the active folder, or nullptr if
     *          the active folder is empty or no folders are open.
     */
    AppMenuButton::Ptr getSelectedButton();

    /**
     * Adds and shows a new pop-up editor component, safely removing any
     * previous editor.
     *
     * @param editor
     */
    void showPopupEditor(AppMenuPopupEditor* editor);

    /**
     * Create and show a pop-up editor component for a button in this menu.
     * This will only work if the button is in the active folder and the 
     * menu is not in the loading state. 
     *
     * @param button
     */
    void showMenuButtonEditor(AppMenuButton::Ptr button);

    /**
     * Sets what should happen when a button is left clicked.
     * This opens selected buttons, and selects unselected buttons
     *
     * @param button
     */
    void onButtonClick(AppMenuButton::Ptr button);

    /**
     * Click AppMenuButtons on left click, open the pop-up menu
     * on right click or control click.
     *
     * @paeam event
     */
    virtual void mouseDown(const MouseEvent& event) override;

    /**
     * Enter or exit the loading state, where the component shows the
     * loading spinner and disables user input.
     * 
     * @param loading
     */
    void setLoadingState(bool loading);

    //Folder transition animation duration, in milliseconds.
    const int animationDuration = 350;

    //Holds the AppFolder components that organize the menu buttons.
    OwnedArray<AppMenuFolder> openFolders;

    //Stores each button by name, so buttons don't need to be re-loaded
    //every time you close a folder and open it again.
    std::map<String, AppMenuButton::Ptr> buttonNameMap;

    //creates all menu items
    AppMenuItemFactory menuItemFactory;

    //keyboard shortcuts
    static const String openPopupMenuBinding;
    static const String reloadMenuBinding;
    //The shared iconThread should exist as long as the AppMenu does.
    IconThread iconThread;
    //Sets if clicking unselected buttons immediately triggers
    //their action, or if it just selects them.
    bool onlyTriggerSelected = true;
    //app grid dimensions, saved from config
    int maxRows = 1;
    int maxColumns = 1;
    //currently focused folder index
    int activeFolderIndex = 0;
    //Launches applications from the menu
    AppLauncher appLauncher;
    //Page lading spinner reference
    OverlaySpinner& loadingSpinner;
    //Holds the menu editor component that can be accessed via the pop-up menu
    ScopedPointer<AppMenuPopupEditor> buttonEditor;
    //Loads system applications from .Desktop files
    static DesktopEntryLoader desktopEntries;
    //Tracks if desktop entries are loading or applications are launching in
    //another thread
    std::atomic<bool> loadingState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};
