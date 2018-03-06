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
    AppMenuComponent(String componentKey, OverlaySpinner& loadingSpinner);

    virtual ~AppMenuComponent();

    /**
     * @return true if the AppMenuComponent is currently in the loading state
     */
    bool isLoading();

    /**
     * Exit the loading state, hiding the spinner and enabling
     * user input
     */
    void exitLoadingState();

    /**
     * Open the pop-up menu used for editing this object.
     * @param selectionMenu if true, the pop-up menu contains only options for 
     * editing the selected menu button in the active folder.
     */
    void openPopupMenu(bool selectionMenu);

    /**
     * Use key presses for menu navigation, setting specific controls based on 
     * AppMenu type. Other classes may call this to pass on or simulate
     * key events.
     * @param key
     * @return true if the key press was used.
     */
    virtual bool keyPressed(const KeyPress& key) = 0;
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
    int getActiveFolderIndex();

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
     * Updates the folder component layout, optionally animating the transition.
     * @param animateTransition if true, animate component changes rather than
     * immediately updating folder bounds.
     */
    virtual void layoutFolders(bool animateTransition) = 0;

    //Holds the AppFolder components that organize the menu buttons.
    OwnedArray<AppMenuFolder> openFolders;
    //Stores each button by name, so buttons don't need to be re-loaded
    //every time you close a folder and open it again.
    std::map<String, AppMenuButton::Ptr> buttonNameMap;

    //this gets passed to AppMenuButtons to load button icons.
    IconThread iconThread;
    
    //app grid dimensions, saved from config
    int maxRows = 1;
    int maxColumns = 1;
    int buttonsPerPage = 1;
private:

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    virtual AppMenuFolder* createFolderObject
    (AppMenuItem::Ptr folderItem) = 0;


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
    void resized() override;

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
     * This will only work if the button is in the last opened folder and the 
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
     * Click AppMenuButtons on left click, open the popup menu
     * on right click or control click.
     */
    virtual void mouseDown(const MouseEvent &event) override;

    /**
     * Enter or exit the loading state, where the component shows the
     * loading spinner and disables user input.
     * @param loading
     */
    void setLoadingState(bool loading);

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
