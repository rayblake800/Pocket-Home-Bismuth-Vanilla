/* 
 * File:   AppMenuComponent.cpp
 * Author: anthony
 * Created on December 19, 2017, 1:37 PM
 */


#include <set>
#include "AppMenuComponent.h"


AppMenuComponent::AppMenuComponent() :
runningCheckTimer(this, [](AppMenuComponent* appMenu) {
    appMenu->checkRunningApps();
}),
debounceTimer(this, [](AppMenuComponent* appMenu) {

    appMenu->setDebounce(false);
}) {
    ConfigFile * configFile = ConfigFile::getInstance();
    Rectangle<int> screenSize = getWindowSize();
    ConfigFile::ComponentSettings menuSettings = configFile->getComponentSettings(ConfigFile::APP_MENU);
    x_origin=menuSettings.x * screenSize.getWidth();
    y_origin=menuSettings.y * screenSize.getHeight();

    ConfigFile::ComponentSettings buttonSettings =
            configFile->getComponentSettings(ConfigFile::APP_MENU_BUTTON);
    buttonWidth = screenSize.getWidth() * buttonSettings.width;
    buttonHeight = screenSize.getHeight() * buttonSettings.height;
    setWantsKeyboardFocus(false);
    launchSpinner = new OverlaySpinner();
    selected.push_back(NULL);
    columnTops.push_back(y_origin);
    buttonColumns.emplace(buttonColumns.begin());

    //read in main page apps from config
    std::vector<ConfigFile::AppItem> favorites = configFile->getFavorites();
    for (const ConfigFile::AppItem& favorite : favorites) {
        DBG(String("AppMenu:Found app in config:") + favorite.name);
        addButton(new AppMenuButton(DesktopEntry(favorite),
                buttonColumns[activeColumn()].size(), activeColumn()));
    }

    //add category buttons
    std::vector<ConfigFile::AppFolder> categories = configFile->getFolders();
    for (const ConfigFile::AppFolder& category : categories) {
        addButton(new AppMenuButton(DesktopEntry(category),
                buttonColumns[activeColumn()].size(), activeColumn()));
    }
    DBG(String("added ") + String(buttonColumns[activeColumn()].size()) + " buttons");
    scrollToSelected();
}

AppMenuComponent::~AppMenuComponent() {
    runningCheckTimer.stopTimer();
    debounceTimer.stopTimer();
    debounce = false;
    while (!buttonColumns.empty())closeFolder();
}

void AppMenuComponent::showLaunchSpinner() {
    DBG("Show launch spinner");
    Component * parentPage = getParentComponent();
    if (parentPage != NULL) {
        parentPage->addAndMakeVisible(launchSpinner);
    }
}

void AppMenuComponent::hideLaunchSpinner() {
    DBG("Hide launch spinner");
    Component * parentPage = getParentComponent();
    if (parentPage != NULL) {
        parentPage->removeChildComponent(launchSpinner);
    }
}

/**
 * Open an application category folder, creating AppMenuButtons for all
 * associated desktop applications.
 */
void AppMenuComponent::openFolder(std::vector<String> categoryNames) {
    std::set<DesktopEntry> folderItems = 
            desktopEntries.getCategoryListEntries(categoryNames);
    if (folderItems.empty())return;
    int columnTop = y_origin;
    if (selected[activeColumn()] != NULL) {
        columnTop = selected[activeColumn()]->getY();
    }
    selected.push_back(NULL);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton*>());
    DBG(String("found ") + String(folderItems.size()) + " items in folder");
    for (DesktopEntry desktopEntry : folderItems) {
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay()) {
            AppMenuButton* newButton = new AppMenuButton(desktopEntry,
                    buttonColumns[activeColumn()].size(), activeColumn());
            addButton(newButton);
        }
    }
    scrollToSelected();
}

//close the topmost open folder, removing all contained buttons

void AppMenuComponent::closeFolder() {
    if (debounce){
        return;
    }
    for (int i = buttonColumns[activeColumn()].size() - 1; i >= 0; i--) {
        AppMenuButton * toDelete = buttonColumns[activeColumn()][i];
        toDelete->removeFromDesktop();
        buttonColumns[activeColumn()].pop_back();
        delete toDelete;
    }
    buttonColumns.pop_back();
    selected.pop_back();
    columnTops.pop_back();
    scrollToSelected();
}


//handle all AppMenuButton clicks

void AppMenuComponent::buttonClicked(Button * buttonClicked) {
    if (debounce) {
        return;
    }
    AppMenuButton * appClicked = (AppMenuButton *) buttonClicked;
    if (appClicked->getColumn() < activeColumn()) {
        while (appClicked->getColumn() < activeColumn()) {
            closeFolder();
        }
        selectIndex(appClicked->getIndex());
        return;
    }
    if (selected[activeColumn()] == appClicked) {
        if (appClicked->isFolder()) {
            openFolder(appClicked->getCategories());
        } else {
            startApp(appClicked);
        }
    } else {
        selectIndex(appClicked->getIndex());
    }
}

void AppMenuComponent::resized() {
    launchSpinner->setBounds(getWindowSize());
    //if (selected[activeColumn()] != NULL){
    //    scrollToSelected();
    //}
}

void AppMenuComponent::selectIndex(int index) {
    if (debounce) {
        return;
    }
    int column = activeColumn();
    DBG(String("AppMenuComponent: selecting column ") + String(column) + String(" index ") + String(index));
    if (index >= buttonColumns[column].size()
            || index < 0
            || selected[column] == buttonColumns[column][index])return;
    if (selected[column] != NULL) {
        selected[column]->setSelected(false);
        selected[column]->repaint();
    }
    selected[column] = buttonColumns[column][index];
    selected[column]->setSelected(true);
    selected[column]->repaint();
    //move AppMenu to center the selected button, if it's not near an edge
    scrollToSelected();
}

//Select the next appMenuButton in the active button column.

void AppMenuComponent::selectNext() {
    if (selected[activeColumn()] == NULL) {
        selectIndex(0);
    } else {
        selectIndex(selected[activeColumn()]->getIndex() + 1);
    }
}

//Select the previous appMenuButton in the active button column.

void AppMenuComponent::selectPrevious() {
    if (selected[activeColumn()] == NULL) {
        selectIndex(0);
    } else {
        selectIndex(selected[activeColumn()]->getIndex() - 1);
    }
}

//Trigger a click for the selected button.

void AppMenuComponent::clickSelected() {
    if (selected[activeColumn()] != NULL && !debounce) {
        selected[activeColumn()]->triggerClick();
    }
}

//return the index of the active button column.

int AppMenuComponent::activeColumn() {
    return selected.size() - 1;
}

void AppMenuComponent::scrollToSelected() {
    int column = activeColumn();
    AppMenuButton* selectedButton = selected[column];
    Rectangle<int>dest = getBounds();
    if (selectedButton != NULL) {
        int buttonPos = selectedButton->getY();
        int screenHeight = getWindowSize().getHeight();
        int distanceFromCenter = abs(buttonPos - getY() + screenHeight / 2);
        //only scroll vertically if selected button is outside the center 3/5 
        if (distanceFromCenter > screenHeight / 5 * 3) {
            dest.setY(y_origin - buttonPos + screenHeight / 2 - buttonHeight / 2);
        }
        if (dest.getY() > y_origin) {
            dest.setY(y_origin);
        } else if (getHeight() > screenHeight && dest.getBottom() < screenHeight) {
            dest.setBottom(screenHeight);
        }
    } else dest.setY(y_origin - columnTops[column]);
    if (column == 0)dest.setX(x_origin);
    else {
        dest.setX(x_origin - column * buttonWidth + buttonHeight);
    }
    ComponentAnimator& animator = Desktop::getInstance().getAnimator();
    if (animator.isAnimating(this)) {
        animator.cancelAnimation(this, false);
    }
    animator.animateComponent(this, dest, 1, 100, false, 1, 1);
}

void AppMenuComponent::addButton(AppMenuButton * appButton) {
    int index = appButton->getIndex();
    int column = appButton->getColumn();
    int w = appButton->getWidth();
    int h = appButton->getHeight();
    int x = x_origin + column*w;
    int y = columnTops[column] + h * buttonColumns[column].size();
    appButton->setBounds(x, y, w, h);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addListener(this);
    this->buttonColumns[column].push_back(appButton);
    if ((x + w) > getWidth())setBounds(getX(), getY(), x + w, getHeight());
    if ((y + h) > getHeight())setBounds(getX(), getY(), getWidth(), y + h);
}

//################## Application Launching #################################

void AppMenuComponent::checkRunningApps() {
    Array<int> needsRemove{};

    // check list to mark any needing removal
    for (const auto& cp : runningApps) {
        if (!cp->isRunning()) {
            needsRemove.add(runningApps.indexOf(cp));
        }
    }

    // cleanup list
    for (const auto appIdx : needsRemove) {
        runningApps.remove(appIdx);
        runningAppsByButton.removeValue(appIdx);
    }

    if (!runningApps.size()) {
        // FIXME: uncomment when process running check works
        // runningCheckTimer.stopTimer();
        hideLaunchSpinner();
    }
}

void AppMenuComponent::setDebounce(bool newState) {
    debounce = newState;
}

AppMenuComponent::AppMenuTimer::AppMenuTimer(AppMenuComponent* appMenu,
        std::function<void(AppMenuComponent*) > callback) :
appMenu(appMenu), callback(callback) {
}

AppMenuComponent::AppMenuTimer::~AppMenuTimer() {

    appMenu = NULL;
    this->stopTimer();
}

void AppMenuComponent::AppMenuTimer::timerCallback() {
    if (appMenu != NULL) {

        callback(appMenu);
    }
}

void AppMenuComponent::startApp(AppMenuButton * appButton) {
    DBG("AppsPageComponent::startApp - " << appButton->getCommand());
    ChildProcess* launchApp = new ChildProcess();
    launchApp->start("xmodmap ${HOME}/.Xmodmap"); // Reload xmodmap to ensure it's running
    if (launchApp->start(appButton->getCommand())) {

        runningApps.add(launchApp);
        runningAppsByButton.set(appButton, runningApps.indexOf(launchApp));
        // FIXME: uncomment when process running check works
        // runningCheckTimer.startTimer(5 * 1000);

        debounce = true;
        debounceTimer.startTimer(2 * 1000);

        // TODO: should probably put app button clicking logic up into LauncherComponent
        // correct level for event handling needs more thought
        showLaunchSpinner();
    }
}

void AppMenuComponent::focusApp(AppMenuButton* appButton, const String & windowId) {

    DBG("AppsPageComponent::focusApp - " << appButton->getCommand());
    String focusShell = "echo 'focus_client_by_window_id(" + windowId + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
}

void AppMenuComponent::startOrFocusApp(AppMenuButton * appButton) {
    if (debounce) return;

    bool shouldStart = true;
    int appIdx = runningAppsByButton[appButton];
    bool hasLaunched = runningApps[appIdx] != nullptr;
    String windowId;

    if (hasLaunched) {
        const auto shellWords = split(appButton->getCommand(), " ");
        const auto& cmdName = shellWords[0];
        StringArray findCmd{"xdotool", "search", "--all", "--limit", "1", "--class", cmdName.toRawUTF8()};
        ChildProcess findWindow;
        findWindow.start(findCmd);
        findWindow.waitForProcessToFinish(1000);
        windowId = findWindow.readAllProcessOutput().trimEnd();

        // does xdotool find a window id? if so, we shouldn't start a new one
        shouldStart = (windowId.length() > 0) ? false : true;
    }

    if (shouldStart) {
        startApp(appButton);
    } else {
        focusApp(appButton, windowId);
    }

}