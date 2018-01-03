/* 
 * File:   AppMenuComponent.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 */

#ifndef APPMENU_H
#define APPMENU_H
#include "../Basic Components/OverlaySpinner.h"
#include "DesktopEntries.h"
#include "AppMenuButton.h"

/**
 * 
 * A menu for launching applications, populated with .Desktop file info
 */
class AppMenuComponent : public Component,
public Button::Listener {
public:
    AppMenuComponent();
    virtual ~AppMenuComponent();

    

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
    //######################## App Launching #################################
    /**
     * @return true if currently waiting on an application to launch.
     */
    bool waitingOnLaunch();
    /**
     * Makes the menu stop waiting on an application to launch, re-enabling
     * user input.
     */
    void stopWaitingOnLaunch();

private:
    //handle all AppMenuButton clicks
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    //if it loses visibility, stop waiting for apps to launch
    void visibilityChanged() override;

    
    void addButton(AppMenuButton::Ptr appButton);
    void selectIndex(int index);
    void scrollToSelected();

    
    ScopedPointer<OverlaySpinner> launchSpinner;
    DesktopEntries desktopEntries;
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

    //store active processes with a pointer to their button index
    struct ButtonProcess{
        ButtonProcess(AppMenuButton::Ptr b,ChildProcess* p):
        menuButton(b),process(p){}
        AppMenuButton::Ptr menuButton;
        ScopedPointer<ChildProcess> process;
    };
    HashMap<AppMenuButton::Ptr, int> runningAppsByButton;
    OwnedArray<ChildProcess> runningApps;
    
    class AppLaunchTimer : public Timer {
    public:
        AppLaunchTimer(AppMenuComponent* appMenu);
        virtual ~AppLaunchTimer();
        virtual void timerCallback() override;
        void setTrackedProcess(ChildProcess * trackedProcess);
        //does the same thing as the base class method, but it also
        //gets rid of its launchButton pointer.
        void stopTimer();
    private:
        AppMenuComponent* appMenu = nullptr;
        ChildProcess * trackedProcess = nullptr;
    };
    AppLaunchTimer launchTimer;
    /**
     * Attempt to find an open window of a launched application
     * @param appButton a button that launched a running process
     * @return the window ID, or the empty string if none was found
     */
    String getWindowId(AppMenuButton::Ptr appButton);
    void startApp(AppMenuButton::Ptr appButton);
    void focusApp(AppMenuButton::Ptr appButton, const String& windowId);
    void startOrFocusApp(AppMenuButton::Ptr appButton);
    /**
     * Display the spinner that indicates application launch. This will
     * also disable input.
     */
    void showLaunchSpinner();
    /**
     * Hide the launch spinner, re-enabling user input.
     */
    void hideLaunchSpinner();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};

#endif /* APPMENU_H */

