/* 
 * File:   AppMenuComponent.h
 * Author: anthony
 *
 * Created on December 19, 2017, 1:37 PM
 * A menu for launching applications, populated with .Desktop file info
 */

#ifndef APPMENU_H
#define APPMENU_H
#include "DesktopEntries.h"
#include "OverlaySpinner.h"
#include "AppMenuButton.h"
#include "../JuceLibraryCode/JuceHeader.h"

class AppMenuComponent : public Component,
public Button::Listener {
public:
    AppMenuComponent();
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
    
    void addButton(AppMenuButton* appButton);
    void selectIndex(int index);
    void scrollToSelected();

    
    ScopedPointer<OverlaySpinner> launchSpinner;
    DesktopEntries desktopEntries;
    OwnedArray<ChildProcess> runningApps;
    //all buttons in each column
    std::vector<std::vector<AppMenuButton*>> buttonColumns;
    //current button selection(if any) for each open column
    std::vector<AppMenuButton*> selected;
    //top y-position of each open column
    std::vector<int> columnTops;

    //appMenuButton dimensions
    int buttonWidth;
    int buttonHeight;
    //base component position
    int x_origin;
    int y_origin;


    using AppRunningMap = HashMap<AppMenuButton*, int>;
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
    void startApp(AppMenuButton * appButton);
    void focusApp(AppMenuButton* appButton, const String& windowId);
    void startOrFocusApp(AppMenuButton* appButton);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuComponent);
};

#endif /* APPMENU_H */

