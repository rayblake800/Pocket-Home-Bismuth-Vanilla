/* 
 * @file   AppMenuComponent.h
 *
 * AppMenuComponent is a scrolling application menu that loads its
 * contents from system .Desktop files and an AppConfigFile object.
 * It provides user interface tools for launching applications, and for
 * editing its own menu entries.
 */

#pragma once
#include <set>
#include "../../../Basic Components/OverlaySpinner.h"
#include "../../../Configuration/Configurables/ConfigurableComponent.h"
#include "../../../AppLauncher.h"
#include "../../../GridLayoutManager.h"
#include "../Popup Editor Components/AppMenuPopupEditor.h"
#include "../AppMenuButton/AppMenuButton.h"
#include "../AppMenuButton/AppMenuItem.h"
#include "../IconThread.h"
#include "../DesktopEntries.h"

class AppMenuComponent : public Component, public ConfigurableComponent{
public:
    /**
     * 
     * @param componentKey
     * @param loadingSpinner
     */
    AppMenuComponent(String componentKey,OverlaySpinner& loadingSpinner);

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
     * editing the selected menu button.
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
     * close the last opened folder, removing all contained buttons from
     * view
     */
    void closeFolder();

    /**
     * Trigger a click for the selected button in the last opened AppFolder
     */
    void clickSelected();

    /**
     * Open a PopupEditorComponent for editing the selected button in the last
     * opened AppFolder.
     */
    void openEditorForSelected();
    
    /**
     * Offset the selected button index in the last opened AppFolder.
     * @param offset will be added to the index if index + offset is a valid
     * index.
     * @return true if the selected button index changed, false if index +
     * offset was an invalid index and the selection did not change. 
     */
    bool changeSelection(int offset);

    /**
     * Updates the folder component layout, optionally animating the transition.
     * @param animateTransition if true, animate component changes rather than
     * immediately updating folder bounds.
     */
    virtual void layoutFolders(bool animateTransition) = 0;
private:

    /**
     * Create a folder component object from a folder menu item.
     * @param folderItem
     */
    virtual AppFolder* createFolderObject(AppMenuItem::Ptr folderItem) = 0;

    /**
     * Reposition child components, and update folder layout without animating.
     */
    void resized() override;

    /**
     * @return the selected button in the last opened folder, or nullptr if
     * the last folder is empty or no folders are open.
     */
    AppMenuButton::Ptr getSelectedButton();

    /**
     * Open an application category folder, creating or adding 
     * AppMenuButtons for all associated desktop applications.
     * 
     * @param folderItem defines the folder and provides all 
     * AppMenuItem objects.
     */
    void openFolder(AppMenuItem::Ptr folderItem);

    /**
     * Adds and shows a new pop-up editor component, safely removing any previous
     * editor.
     * @param editor
     */
    void showPopupEditor(PopupEditorComponent* editor);
    
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

    //Launches applications from the menu
    AppLauncher appLauncher;
    OverlaySpinner& loadingSpinner;
    ScopedPointer<AppMenuPopupEditor> buttonEditor;
    static DesktopEntries desktopEntries;
    //this gets passed to AppMenuButtons to load button icons.
    IconThread iconThread;
    //Tracks if desktop entries are loading or applications are launching in
    //another thread
    bool loadingState = false;
    //Allow for safe asynchronous access to the loading state
    CriticalSection loadingLock;
    //Stores each button by name, so buttons don't need to be re-loaded
    //every time you close a folder and open it again.
    std::map<String, AppMenuButton::Ptr> buttonNameMap;
    OwnedArray<AppFolder> openFolders;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);

protected:
    //############################# AppFolder ##################################

    /**
     * AppMenuComponents divide their menu items into distinct folder
     * components.  Individual menu buttons are managed through the folder
     * they're contained within.
     */
    class AppFolder : public Component {
    public:
        /**
         * Creates a new folder component, loading menu buttons from a
         * folder menu item.
         * 
         * @param folderItem 
         * @param btnListener will be assigned to listen to all folder
         * menu buttons.
         */
        AppFolder(AppMenuItem::Ptr folderItem, MouseListener* btnListener);

        virtual ~AppFolder();

        /**
         * Create AppMenuButton components for a set of AppMenuItems.
         * @param menuItems
         */
        Array<AppMenuButton::Ptr> createMenuButtons
        (Array<AppMenuItem::Ptr> menuItems) = 0;

        /**
         * Reload all folder menu buttons from their source menu item.
         */
        void reload();

        /**
         * @return number of menu buttons in the folder.
         */
        int size();

        /**
         * Find the index of a menu button in this folder
         * 
         * @param menuButton
         * @return the button's index, or -1 if the button is not in this
         * folder.
         */
        bool getButtonIndex(AppMenuButton::Ptr menuButton);

        /**
         * @return The selected menu button in this folder.
         * This will only return nullptr if there are no buttons in 
         * this folder.
         */
        AppMenuButton::Ptr getSelectedButton();

        /**
         * Set this folder's selected menu button
         * @param index
         * @return true if index was valid and the selection changed.
         */
        bool selectIndex(int index);

        /**
         * @return the index of the selected menu button.
         */
        int getSelectedIndex();

        /**
         * Insert a new button to the folder at a specific index,
         * shifting forward any buttons at indices equal or greater
         * than the index. 
         * 
         * @param index should be between 0 and appFolder.size(),
         * inclusive.  Values outside of this range will be rounded to
         * the nearest valid value.
         */
        void insertButton(AppMenuButton::Ptr newButton, int index);

        /**
         * Remove the button at a given index, shifting back any buttons
         * at greater indices to fill the gap.
         * 
         * @param index should be between 0 and appFolder.size(), 
         * inclusive, otherwise this method will do nothing.
         */
        void removeButton(int index);

        /**
         * Swap the indices and positions of two buttons in the folder.
         * Both indices must be valid, or nothing will happen.
         * @param btnIndex1
         * @param btnIndex2
         */
        void swapButtons(int btnIndex1, int btnIndex2);

    protected:
        /**
         * Set the relative spacing of the folder component layout.
         * 
         * @param margin space between components and the edge of the
         * folder component, as a fraction of folder width.
         * 
         * @param xPadding horizontal space between folder child
         * components, as a fraction of folder width.
         * 
         * @param yPadding vertical space between folder child
         * components, as a fraction of folder height.
         */
        void setSpacing(float margin, float xPadding, float yPadding);
    private:
        /**
         * Reposition folder buttons when folder bounds change.
         */
        void resized() override;

        /**
         * Clear folderLayout,remove all child components, reload the
         * button layout, and re-add the layout buttons as child
         * components.
         */
        virtual void layoutButtons();

        /**
         * Given a list of folder buttons, return an appropriate layout
         * for positioning them in the folder component.
         * 
         * @param buttons
         * @return a Layout containing all items in the button array.
         */
        virtual GridLayoutManager::Layout buildFolderLayout
        (Array<AppMenuButton::Ptr> buttons) = 0;

        /**
         * @param index
         * @return true for any valid button index
         */
        inline bool validBtnIndex(int index) {
            return index >= 0 && index < folderButtons.size();
        }

        MouseListener* btnListener = nullptr;
        AppMenuItem::Ptr sourceFolderItem = nullptr;
        GridLayoutManager folderLayout;
        float margin = 0;
        float xPadding = 0;
        float yPadding = 0;
        Array<AppMenuButton::Ptr> folderButtons;
        int selectedIndex = 0;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppFolder)
    };
};
