/* 
 * @file   AppMenuComponent.h
 *
 * AppMenuComponent is a scrolling application menu that loads its
 * contents from system .Desktop files and an AppConfigFile object.
 * The menu is divided into folders, which can contain different sets of menu 
 * buttons to launch applications or open sub-folders.  The user can navigate
 * through these folders and select menu items to launch applications or open
 * new folders.  The menu also provides controls for adding, editing, and 
 * deleting menu entries.
 */


#pragma once
#include <set>
#include <atomic>
#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../Basic Components/OverlaySpinner.h"
#include "../../../Configuration/Configurables/ConfigurableComponent.h"
#include "../../../AppLauncher.h"
#include "../../../GridLayoutManager.h"
#include "../IconThread.h"
#include "../DesktopEntries.h"
#include "Menu Editors/AppMenuPopupEditor.h"
#include "Menu Items/AppMenuItem.h"
#include "AppMenuFolder.h"
#include "AppMenuButton.h"

class AppMenuComponent : public Component, public ConfigurableComponent {
public:
    /**
     * @param componentKey loads menu bounds from config
     * @param loadingSpinner reference to an overlay spinner that sits over
     * the PageComponent holding this AppMenuComponent.
     */
    AppMenuComponent(String componentKey,
            OverlaySpinner& loadingSpinner);

    virtual ~AppMenuComponent();

    /**
     * @return true if the AppMenuComponent is currently in the loading state
     */
    bool isLoading() const;

    /**
     * Exit the loading state, hiding the spinner and enabling
     * user input
     */
    void exitLoadingState();

    /**
     * Open the pop-up menu used for editing menu items.
     * @param selectedButton if null, open a generic folder editing
     * menu. otherwise, open the menu for editing this particular menu
     * button.
     */
    void openPopupMenu(AppMenuButton::Ptr selectedButton);

    /**
     * Respond to key events if input is not being ignored.
     * This opens the pop-up menu or reloads the AppMenu if the correct 
     * shortcuts are detected, otherwise it lets the AppMenuComponent's subclass
     * determine how to handle the key event.
     * @param key
     * @return true if the keyPress is handled by the menu, false otherwise.
     */
    bool keyPressed(const KeyPress& key) override;

    /**
     * This method will run whenever this component is resized.  Classes
     * inheriting AppMenuComponent should override this instead of 
     * void resized(). This method will run before the menu folders are updated
     * for the new bounds.
     */
    virtual void menuResized() {
    }

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
     * @param folder must be a valid folder index, or the active folder index
     * will not change.
     */
    void setActiveFolderIndex(int folder);

    /**
     * Get the index of the active folder.  AppMenuComponents should center the
     * folder layout on the active folder, and apply key controls to that
     * folder's menu items.
     * @return The active folder index, usually the last open folder.
     * Returns 0 if no folders are open.
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
     * @param index
     * @return the number of buttons in the folder at this index, or -1
     * if there is no folder at this index.
     */
    int getFolderSize(int index);

    /**
     * @param index
     * @return the minimum width, in pixels, that the folder at this index
     * should have, or -1 if there is no folder at this index.
     */
    int getMinimumFolderWidth(int index);

    /**
     * @param index the index of one of this menu's AppMenuFolders
     * @return the selected menu button index for the folder, or -1 if
     * the folder at the index parameter doesn't exist or has no selected menu
     * button.
     */
    int getFolderSelectedIndex(int index) const;

    /**
     * Updates the folder component layout, optionally animating the transition.
     */
    void layoutFolders();

    /**
     * @return the maximum number of menu item columns to show on screen
     */
    int getMaxColumns() const;

    /**
     * @return the maximum number of menu item rows to show on screen
     */
    int getMaxRows() const;

    /**
     * @return the maximum number of menu items to show on screen
     */
    inline int buttonsPerPage() const {
        return getMaxColumns() * getMaxRows();
    }

private:
    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem provides folder menu items
     * @param buttonMap is used by the folder to recycle menu buttons
     * @param iconThread is used by the folder to load button icons
     */
    virtual AppMenuFolder* createFolderObject
    (AppMenuItem::Ptr folderItem,
            std::map<String, AppMenuButton::Ptr>& buttonMap,
            IconThread& iconThread) = 0;

    /**
     * Check to see if any changes have occurred that justifies changing
     * folder layout.  This does not need to account for the initial layout,
     * changes to menu bounds, folders opening and closing, and selection
     * of a new active folder, as all those events will update folder layout
     * without checking this value.
     * 
     * @param activeFolder may be needed to determine if folder layouts should
     * change.
     * @return true to regenerate the layout, false to leave it as it is.
     */
    virtual bool layoutChanged(const AppMenuFolder* activeFolder) = 0;

    /**
     * Return the bounds where the given folder should be placed in the menu.
     * @param folder
     * @param folderIndex
     * @return 
     */
    virtual Rectangle<int> updateFolderBounds(const AppMenuFolder* folder,
            int folderIndex) = 0;

    /**
     * Uses key events to control the menu in whatever way is appropriate
     * for the specific AppMenuComponent.
     * @param key
     * @param activeFolder
     * @return true iff the key press was used.
     */
    virtual bool folderKeyPressed
    (const KeyPress& key, AppMenuFolder* activeFolder) = 0;

    /**
     * Updates the layout if row/column size changes, otherwise handle
     * changes like any other ConfigurableComponent.
     * @param config the configFile containing the updated data value
     * @param key the key of property that has changed
     */
    void loadConfigProperties(ConfigFile* config, String key);

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
     * @param folderItem defines the folder and provides all 
     * AppMenuItem objects.
     */
    void openFolder(AppMenuItem::Ptr folderItem);

    /**
     * @return the selected button in the active folder, or nullptr if
     * the active folder is empty or no folders are open.
     */
    AppMenuButton::Ptr getSelectedButton();

    /**
     * Adds and shows a new pop-up editor component, safely removing any
     * previous editor.
     * @param editor
     */
    void showPopupEditor(AppMenuPopupEditor* editor);

    /**
     * Create and show a pop-up editor component for a button in this menu.
     * This will only work if the button is in the active folder and the 
     * menu is not in the loading state. 
     * @param button
     */
    void showMenuButtonEditor(AppMenuButton::Ptr button);

    /**
     * Sets what should happen when a button is left clicked.
     * This opens selected buttons, and selects unselected buttons
     * @param button
     */
    void onButtonClick(AppMenuButton::Ptr button);

    /**
     * Click AppMenuButtons on left click, open the pop-up menu
     * on right click or control click.
     */
    virtual void mouseDown(const MouseEvent &event) override;

    /**
     * Enter or exit the loading state, where the component shows the
     * loading spinner and disables user input.
     * @param loading
     */
    void setLoadingState(bool loading);

    /**
     * @return true iff the menu is loading, empty, animating, or otherwise
     * in a state where user input should be ignored.
     */
    bool ignoringInput() const;


    //Holds the AppFolder components that organize the menu buttons.
    OwnedArray<AppMenuFolder> openFolders;

    //Stores each button by name, so buttons don't need to be re-loaded
    //every time you close a folder and open it again.
    std::map<String, AppMenuButton::Ptr> buttonNameMap;

    //this gets passed to AppMenuButtons to load button icons.
    IconThread iconThread;
    //keyboard shortcuts
    static const String openPopupMenuBinding;
    static const String reloadMenuBinding;
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
    static DesktopEntries desktopEntries;
    //Tracks if desktop entries are loading or applications are launching in
    //another thread
    std::atomic<bool> loadingState;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};
