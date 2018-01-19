/* 
 * @file   AppMenuComponent.h
 *
 * AppMenuComponent is a scrolling application menu that loads its
 * contents from system .Desktop files and the AppConfigFile
 */

#ifndef APPMENU_H
#define APPMENU_H
#include <atomic>
#include "../Basic Components/OverlaySpinner.h"
#include "../Configuration/Configurables/ConfigurableComponent.h"
#include "Popup Editor Components/PopupEditorComponent.h"
#include "AppMenuButton/AppMenuButton.h"
#include "AppLauncher.h"
#include "IconThread.h"
#include "DesktopEntries.h"

/**
 * 
 * A menu for launching applications, populated with .Desktop file info
 */
class AppMenuComponent : public Component,
public ConfigurableComponent,
public Button::Listener {
public:
    AppMenuComponent(AppConfigFile& appConfig);
    virtual ~AppMenuComponent();

    /**
     * Loads all app menu buttons
     */
    void loadButtons();


    //################ AppMenuButton Management   #############################

    /**
     * Select the next appMenuButton in the active button column.
     */
    void selectNext();
    /**
     * Select the previous appMenuButton in the active button column.
     */
    void selectPrevious();
    /**
     * Trigger a click for the selected button.
     */
    void clickSelected();

    /**
     * Returns a popup editor component for updating the selected button.
     * @return either an editor component, or nullptr if no button is selected.
     */
    PopupEditorComponent* getEditorForSelected();

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
    /**
     * @return the index of the active button column.
     */
    int activeColumn();
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
    //AppMenuComponent has no asset files or colors.
    void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours){};
private:
    AppConfigFile& appConfig;
    AppLauncher appLauncher;
    //handle all AppMenuButton clicks
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    //if it loses visibility, stop waiting for apps to launch
    void visibilityChanged() override;


    void addButton(AppMenuButton::Ptr appButton);
    void selectIndex(int index);
    void scrollToSelected(bool animatedScroll=true);


    ScopedPointer<OverlaySpinner> loadingSpinner;
    DesktopEntries desktopEntries;
    //True iff desktopEntries are loading in another thread.
    std::atomic<bool> loadingAsync;

    //all buttons in each column
    std::vector<std::vector<AppMenuButton::Ptr>> buttonColumns;
    //current button selection(if any) for each open column
    std::vector<AppMenuButton::Ptr> selected;
    //top y-position of each open column
    std::vector<int> columnTops;
    std::map<String, AppMenuButton::Ptr> buttonNameMap;

    //base component position
    int x_origin;
    int y_origin;

    IconThread iconThread;

    /**
     * Display the spinner that indicates application loading. This will
     * also disable input.
     */
    void showLoadingSpinner();
    
    /**
     * Hide the loading spinner, re-enabling user input.
     */
    void hideLoadingSpinner();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};

#endif /* APPMENU_H */

