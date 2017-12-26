/* 
 * File:   AppMenuComponent.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 * A menu for launching applications, populated with .Desktop file info
 */

#ifndef APPMENU_H
#define APPMENU_H
#include <functional>
#include "../JuceLibraryCode/JuceHeader.h"
#include "AppMenuButton.h"
#include "DesktopEntries.h"
#include "PageStackComponent.h"
#include "OverlaySpinner.h"

class AppMenuComponent : public Component,
public Button::Listener {
public:
    AppMenuComponent(const var &configJson, Rectangle<int>clipBounds);
    virtual ~AppMenuComponent();
    
    void showLaunchSpinner();
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
     * @param categoryName the category to open
     */
    void openFolder(String categoryName);

    /**
     * close the topmost open folder, removing all contained buttons
     */
    void closeFolder();


    void checkRunningApps();
    bool getDebounce();
    void setDebounce(bool newState);



    /**
     * @return the index of the active button column.
     */
    int activeColumn();
private:
    //handle all AppMenuButton clicks
    void buttonClicked(Button* buttonClicked) override;
    void resized() override;
    
    void addButton(AppMenuButton* appButton);
    void selectIndex(int index);
    void scrollToSelected();

    
    ScopedPointer<OverlaySpinner> launchSpinner;
    DesktopEntries desktopEntries;
    OwnedArray<ChildProcess> runningApps;
    bool debounce = false;
    //all buttons in each column
    std::vector<std::vector<AppMenuButton*>> buttonColumns;
    //current button selection(if any) for each open column
    std::vector<AppMenuButton*> selected;
    //top y-position of each open column
    std::vector<int> columnTops;

    //appMenuButton dimensions, loaded in constructor
    int buttonWidth;
    int buttonHeight;
    //base component position/clip bounds
    Rectangle<int>clipBounds;

    //################## Application Launching #################################
    /**
     * Assigns a callback handler for launching applications.
     * @param launchFn when a selected app button is clicked, 
     * launchFn(shell command) will run.
     */
    using AppRunningMap = HashMap<AppMenuButton*, int>;
    AppRunningMap runningAppsByButton;
    class AppMenuTimer : public Timer {
    public:
        AppMenuTimer(AppMenuComponent* appMenu, std::function<void(AppMenuComponent*)> callback);
        virtual ~AppMenuTimer();
        virtual void timerCallback() override;
    private:
        AppMenuComponent* appMenu = NULL;
        std::function<void(AppMenuComponent*)> callback;
    };
    AppMenuTimer runningCheckTimer;
    AppMenuTimer debounceTimer;
    void startApp(AppMenuButton * appButton);
    void focusApp(AppMenuButton* appButton, const String& windowId);
    void startOrFocusApp(AppMenuButton* appButton);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};

#endif /* APPMENU_H */

