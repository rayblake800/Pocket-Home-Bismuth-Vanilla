/* 
 * @file   AppMenuComponent.h
 *
 * AppMenuComponent is a scrolling application menu that loads its
 * contents from system .Desktop files and an AppConfigFile object.
 * It provides user interface tools for launching applications, and for
 * editing its own menu entries.
 */

#pragma once
#include <atomic>
#include <set>
#include "../../Basic Components/OverlaySpinner.h"
#include "../../Configuration/Configurables/ConfigurableComponent.h"
#include "../Popup Editor Components/AppMenuPopupEditor.h"
#include "../AppMenuButton/AppMenuButton.h"
#include "../AppMenuButton/AppMenuItem.h"
#include "../AppLauncher.h"
#include "../IconThread.h"
#include "../DesktopEntries.h"

/**
 * TODO:
 * - Turn this into an abstract class, add inheriting class 
 *   ScrollingAppMenuComponent
 * - Add additional menu styles in new inheriting classes, e.g. 
 *   TabbedAppMenuComponent
 */
class AppMenuComponent : public Component,
public ConfigurableComponent {
public:
    AppMenuComponent(String componentKey,AppConfigFile& appConfig);
    virtual ~AppMenuComponent();

    /**
     * Loads all app menu buttons
     */
    void loadButtons();

    /**
     * Holding an unmoving pop-up window in a scrolling menu component is
     * less than ideal, it's better if a parent component can deal with
     * displaying any pop-up editor components.
     * 
     * If this function is called, when this component would create a pop-up
     * editor component, instead of adding it to itself, it will pass the
     * editor to this callback function.
     * 
     * @param callback should pass the editor component to another component
     * that is prepared to handle it.
     */
    void setPopupCallback(std::function<void(AppMenuPopupEditor*) > callback);

    
    /**
     * AppMenuComponent should be passed all key events from the page component
     * that contains it, as long as the menu isn't either loading or animating.
     * AppMenuComponent subclasses are then free to use key events in whatever
     * way works best for them.
     * @param key
     * @return true iff the key press was used by the AppMenu
     */
    virtual bool keyPressed(const KeyPress& key) = 0;
    
    
    //################  AppMenuButton Management   #############################

    /**
     * Select a new AppMenuButton relative to the current selected button.
     * If there is no currently selected button, index 0 will be selected.
     * 
     * @param indexOffset is added to the current selected index to find the
     * new selection index.  If this index is invalid, the selection will not
     * change.
     */
    void changeSelection(int indexOffset);

    /**
     * Trigger a click for the selected button.
     */
    void clickSelected();

    /**
     * Open the pop-up menu used for editing this object.
     * @param selectionMenu if true, the pop-up menu contains only options for 
     * editing the selected menu button.
     */
    void openPopupMenu(bool selectionMenu);

    /**
     * @return the index of the active button column.
     */
    int activeColumn();


    //#################### Folder Management  ############################
    /**
     * Open an application category folder, creating AppMenuButtons for all
     * associated desktop applications.
     * @param categoryName the categories include in the folder
     */
    void openFolder(Array<String> categoryNames);

    /**
     * close the topmost open folder, removing all contained buttons
     */
    void closeFolder();
    //######################## App Launching #################################

    /**
     * @return true if currently loading information or a new child process.
     */
    bool isLoading();

    /**
     * Makes the menu stop waiting to load something, re-enabling
     * user input.
     */
    void stopWaitingForLoading();

protected:

    /**
     * AppMenuComponent has no asset files or colors, so this can be implemented
     * as an empty function.
     */
    void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours) {
    };

    /**
     * Gets the selected button in the active button column.
     * @return a pointer to the selected button if one exists, or nullptr
     * otherwise.
     */
    AppMenuButton* getSelectedButton();
    
    /**
     * Resize all child components.
     */
    virtual void resized() override;

    //all buttons in each column
    std::vector<std::vector<AppMenuButton::Ptr>> buttonColumns;

    //current button selection(if any) for each open column
    std::vector<AppMenuButton::Ptr> selected;

    //this gets passed to AppMenuButtons to load button icons.
    IconThread iconThread;
private:

    /**
     * Create a new menu button component.
     * @param menuItem menu data to be held by the component
     */
    virtual AppMenuButton::Ptr createMenuButton
    (AppMenuItem* menuItem, int rowIndex, int columnIndex) = 0;
    /**
     * Display the spinner that indicates application or button loading. This 
     * will also disable input.
     */
    void showLoadingSpinner();

    /**
     * Hide the loading spinner, re-enabling user input.
     */
    void hideLoadingSpinner();

    /**
     * Sets what should happen when a button is left clicked.
     * This opens selected buttons, and selects unselected button
     * @param button
     */
    void onButtonClick(AppMenuButton* button);

    /**
     * Returns a pop-up editor component for updating the selected button.
     * @return either an editor component, or nullptr if no button is selected.
     */
    AppMenuPopupEditor* getEditorForSelected();

    /**
     * Add a new application button to the active menu column.
     * @param appButton will be added to the bottom of the active menu column.
     */
    virtual void addButton(AppMenuButton::Ptr appButton);

    /**
     * Handles the placement of new button components in the menu bounds
     */
    virtual void addButtonComponent(AppMenuButton* appButton) = 0;

    /**
     * Change which button is selected in the active menu column.
     * @param index should correspond to a button in the active menu column.
     * If the index is out of bounds, nothing will happen.
     */
    void selectIndex(int index);

    /**
     * Swaps the positions of two different buttons in the menu. This updates
     * their column and index values, swaps their bounds, and changes their
     * placements in the buttonColumns array.
     * @param button1
     * @param button2
     */
    void swapButtons(AppMenuButton* button1, AppMenuButton* button2);

    /**
     * Scroll the menu so that the selected button is centered.
     * @param animatedScroll sets if the menu should animate its scrolling,
     * or just jump immediately to the destination position.
     */
    virtual void scrollToSelected(bool animatedScroll = true) = 0;

    /**
     * handle all AppMenuButton clicks
     * @param event
     */
    virtual void mouseDown(const MouseEvent &event) override;



    /**
     * Exit the loading state when visibility is lost, enter the loading state
     * if necessary when visibility is gained.
     */
    void visibilityChanged() override;

    //Application configuration object used to fetch folder definitions and
    //favorites/pinned applications
    AppConfigFile& appConfig;

    //Handles application launching when application buttons are selected and
    //clicked.
    AppLauncher appLauncher;

    //Callback function that sets how PopupEditorComponents will be added
    //to the screen.
    std::function<void(AppMenuPopupEditor*) > showPopupCallback;

    //Loading spinner overlay to display when loading buttons or launching
    //applications.
    OverlaySpinner loadingSpinner;

    //Holds any pop-up editors created to edit AppMenuButtons, if
    //showPopupCallback isn't set to handle them in some better way. 
    ScopedPointer<AppMenuPopupEditor> buttonEditor;

    //Loads .desktop application file data from all appropriate locations,
    //and grants access to their data so they can be used to create 
    //AppMenuButtons
    DesktopEntries desktopEntries;

    //True iff desktopEntries are loading in another thread.
    std::atomic<bool> loadingAsync;


    //Stores each button by name, so buttons don't need to be re-loaded
    //every time you close a folder and open it again.
    std::map<String, AppMenuButton::Ptr> buttonNameMap;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};


