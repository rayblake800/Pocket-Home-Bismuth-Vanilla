/* 
 * File:   AppMenuComponent.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 */

#ifndef APPMENU_H
#define APPMENU_H
#include "../DesktopEntries.h"
#include "Basic/OverlaySpinner.h"
#include "AppMenuButton.h"
#include "../JuceLibraryCode/JuceHeader.h"

/**
 * 
 * A menu for launching applications, populated with .Desktop file info
 */
class AppMenuComponent : public Component,
public Button::Listener {
public:
    AppMenuComponent();
    virtual ~AppMenuComponent();
    
    /**
     * Display the spinner that indicates application launch. This will
     * also disable input.
     */
    void showLaunchSpinner();
    /**
     * Hide the launch spinner, re-enabling user input.
     */
    void hideLaunchSpinner();

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

    //#################### Folder Management  ############################
    /**
     * Open an application category folder, creating AppMenuButtons for all
     * associated desktop applications.
     * @param categoryName the categories include in the folder
     */
    void openFolder(std::vector<String> categoryNames);

    /**
     * close the topmost open folder, removing all contained buttons
     */
    void closeFolder();
    /**
     * @return the index of the active button column.
     */
    int activeColumn();

    void checkRunningApps();
    bool getDebounce();
    void setDebounce(bool newState);

private:
    //handle all AppMenuButton clicks
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    
    void addButton(AppMenuButton::Ptr appButton);
    void selectIndex(int index);
    void scrollToSelected();

    
    ScopedPointer<OverlaySpinner> launchSpinner;
    DesktopEntries desktopEntries;
    OwnedArray<ChildProcess> runningApps;
    //all buttons in each column
    std::vector<std::vector<AppMenuButton::Ptr>> buttonColumns;
    //current button selection(if any) for each open column
    std::vector<AppMenuButton::Ptr> selected;
    //top y-position of each open column
    std::vector<int> columnTops;
    std::map<String,AppMenuButton::Ptr> buttonNameMap;

    //appMenuButton dimensions
    int buttonWidth;
    int buttonHeight;
    //base component position
    int x_origin;
    int y_origin;


    using AppRunningMap = HashMap<AppMenuButton::Ptr, int>;
    AppRunningMap runningAppsByButton;
    class AppMenuTimer : public Timer {
    public:
        AppMenuTimer(AppMenuComponent* appMenu, std::function<void(AppMenuComponent*)> callback);
        virtual ~AppMenuTimer();
        virtual void timerCallback() override;
    private:
        AppMenuComponent* appMenu = nullptr;
        std::function<void(AppMenuComponent*)> callback;
    };
    AppMenuTimer runningCheckTimer;
    AppMenuTimer debounceTimer;
    bool debounce = false;
    void startApp(AppMenuButton::Ptr appButton);
    void focusApp(AppMenuButton::Ptr appButton, const String& windowId);
    void startOrFocusApp(AppMenuButton::Ptr appButton);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};

#endif /* APPMENU_H */

