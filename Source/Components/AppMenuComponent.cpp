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
    Rectangle<int>bounds = menuSettings.getBounds();
    x_origin = bounds.getX();
    y_origin = bounds.getY();

    ConfigFile::ComponentSettings buttonSettings =
            configFile->getComponentSettings(ConfigFile::APP_MENU_BUTTON);
    Rectangle<int>buttonSize = AppMenuButton::getButtonSize();
    buttonWidth = buttonSize.getWidth();
    buttonHeight = buttonSize.getHeight();
    setWantsKeyboardFocus(false);
    launchSpinner = new OverlaySpinner();
    launchSpinner->setAlwaysOnTop(true);
    selected.push_back(nullptr);
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
    if (parentPage != nullptr) {
        parentPage->addAndMakeVisible(launchSpinner);
    }
}

void AppMenuComponent::hideLaunchSpinner() {
    DBG("Hide launch spinner");
    Component * parentPage = getParentComponent();
    if (parentPage != nullptr) {
        parentPage->removeChildComponent(launchSpinner);
    }
}

/**
 * Open an application category folder, creating AppMenuButtons for all
 * associated desktop applications.
 */
void AppMenuComponent::openFolder(std::vector<String> categoryNames) {
    ConfigFile * config = ConfigFile::getInstance();
    int folderIndex = selected[activeColumn()]->getIndex() -
            config->getFavorites().size();
    ConfigFile::AppFolder selectedFolder = config->getFolders()[folderIndex];
    std::set<DesktopEntry> folderItems =
            desktopEntries.getCategoryListEntries(categoryNames);
    if (folderItems.empty() && selectedFolder.pinnedApps.empty())return;
    int columnTop = y_origin;
    if (selected[activeColumn()] != nullptr) {
        columnTop = selected[activeColumn()]->getY();
    }
    selected.push_back(nullptr);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton::Ptr>());
    for (ConfigFile::AppItem item : selectedFolder.pinnedApps) {
        AppMenuButton::Ptr addedButton;
        if (buttonNameMap[item.name] != nullptr &&
                buttonNameMap[item.name]->getParentComponent() == nullptr) {
            addedButton = buttonNameMap[item.name];
            addedButton->setIndex(buttonColumns[activeColumn()].size());
            addedButton->setColumn(activeColumn());
        } else {
            addedButton = new AppMenuButton(DesktopEntry(item),
                    buttonColumns[activeColumn()].size(), activeColumn());
        }
        addButton(addedButton);
    }
    DBG(String("found ") + String(folderItems.size()) + " items in folder");
    for (DesktopEntry desktopEntry : folderItems) {
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay()) {
            String name = desktopEntry.getName();
            AppMenuButton::Ptr addedButton;
            if (buttonNameMap[name] != nullptr &&
                    buttonNameMap[name]->getParentComponent() == nullptr) {
                addedButton = buttonNameMap[name];
                addedButton->setIndex(buttonColumns[activeColumn()].size());
                addedButton->setColumn(activeColumn());
            } else {
                addedButton = new AppMenuButton(desktopEntry,
                        buttonColumns[activeColumn()].size(), activeColumn());
            }
            addButton(addedButton);
        }
    }
    selected[activeColumn()] = buttonColumns[activeColumn()][0];
    buttonColumns[activeColumn()][0]->setSelected(true);
    scrollToSelected();
}

//close the topmost open folder, removing all contained buttons

void AppMenuComponent::closeFolder() {
    if (debounce) {
        return;
    }
    for (int i = buttonColumns[activeColumn()].size() - 1; i >= 0; i--) {
        AppMenuButton::Ptr toRemove = buttonColumns[activeColumn()][i];
        toRemove->setVisible(false);
        toRemove->setSelected(false);
        Component * parent = toRemove->getParentComponent();
        if (parent != nullptr) {
            parent->removeChildComponent(toRemove);
        }
        buttonColumns[activeColumn()].pop_back();
    }
    selected.pop_back();
    buttonColumns.pop_back();
    columnTops.pop_back();
    scrollToSelected();
}


//handle all AppMenuButton clicks

void AppMenuComponent::buttonClicked(Button * buttonClicked) {
    if (debounce) {
        return;
    }
    AppMenuButton::Ptr appClicked = (AppMenuButton *) buttonClicked;
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
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings menuSettings = config->getComponentSettings(ConfigFile::APP_MENU);
    Rectangle<int> menuBounds = menuSettings.getBounds();
    x_origin = menuBounds.getX();
    y_origin = menuBounds.getY();
    //resize all buttons
    Rectangle<int>buttonSize = AppMenuButton::getButtonSize();
    buttonWidth = buttonSize.getWidth();
    buttonHeight = buttonSize.getHeight();
    int numColumns = selected.size();
    if (menuBounds.getWidth() < numColumns * buttonWidth) {
        menuBounds.setWidth(numColumns * buttonWidth);
    }
    for (int c = 0; c < numColumns; c++) {
        if (c > 0) {
            columnTops[c] = selected[c - 1]->getY();
        }
        int numRows = buttonColumns[c].size();
        if (menuBounds.getHeight() < numRows * buttonHeight + columnTops[c]) {
            menuBounds.setHeight(numRows * buttonHeight + columnTops[c]);
        }
        for (int i = 0; i < numRows; i++) {
            AppMenuButton * button = buttonColumns[c][i];
            button->setBounds(buttonSize.withPosition(c*buttonWidth,
                    i * buttonHeight + columnTops[c]));
        }
    }
    setBounds(menuBounds);
    if (activeColumn() >= 0 && selected[activeColumn()] != nullptr
            && !Desktop::getInstance().getAnimator().isAnimating()) {
        scrollToSelected();
    }
}

void AppMenuComponent::selectIndex(int index) {
    if (debounce) {
        return;
    }
    int column = activeColumn();
    //DBG(String("AppMenuComponent: selecting column ") + String(column) + String(" index ") + String(index));
    if (index >= buttonColumns[column].size()
            || index < 0
            || selected[column] == buttonColumns[column][index])return;
    if (selected[column] != nullptr) {
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
    if (selected[activeColumn()] == nullptr) {
        selectIndex(0);
    } else {
        selectIndex(selected[activeColumn()]->getIndex() + 1);
    }
}

//Select the previous appMenuButton in the active button column.

void AppMenuComponent::selectPrevious() {
    if (selected[activeColumn()] == nullptr) {
        selectIndex(0);
    } else {
        selectIndex(selected[activeColumn()]->getIndex() - 1);
    }
}

//Trigger a click for the selected button.

void AppMenuComponent::clickSelected() {
    if (selected[activeColumn()] != nullptr && !debounce) {
        selected[activeColumn()]->triggerClick();
    }
}

//return the index of the active button column.

int AppMenuComponent::activeColumn() {
    return selected.size() - 1;
}

void AppMenuComponent::scrollToSelected() {
    int column = activeColumn();
    if (column < 0) {
        return;
    }
    AppMenuButton::Ptr selectedButton = selected[column];
    Rectangle<int>dest = getBounds();
    if (selectedButton != nullptr && selectedButton->isVisible()) {
        int buttonPos = selectedButton->getY();
        int screenHeight = getWindowSize().getHeight();
        int distanceFromCenter = abs(buttonPos - getY() + screenHeight / 2);
        //only scroll vertically if selected button is outside the center 3/5 
        if (distanceFromCenter > screenHeight / 5 * 3) {
            dest.setY(y_origin - buttonPos + screenHeight / 2 - buttonHeight / 2);
        }
        if (dest.getY() > y_origin) {
            dest.setY(y_origin);
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

void AppMenuComponent::addButton(AppMenuButton::Ptr appButton) {
    String name = appButton->getAppName();
    if (buttonNameMap[name] == nullptr) {
        buttonNameMap[name] = appButton;
    }
    int index = appButton->getIndex();
    int column = appButton->getColumn();
    int x = column*buttonWidth;
    int y = columnTops[column] + buttonHeight * buttonColumns[column].size();
    appButton->setBounds(x, y, buttonWidth, buttonHeight);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addListener(this);
    this->buttonColumns[column].push_back(appButton);
    if ((x + buttonWidth) > getWidth()) {
        setBounds(getX(), getY(), x + buttonWidth, getHeight());
    }
    if ((y + buttonHeight) > getHeight()) {
        setBounds(getX(), getY(), getWidth(), y + buttonHeight);
    }
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

    appMenu = nullptr;
    this->stopTimer();
}

void AppMenuComponent::AppMenuTimer::timerCallback() {
    if (appMenu != nullptr) {

        callback(appMenu);
    }
}

void AppMenuComponent::startApp(AppMenuButton::Ptr appButton) {
    DBG("AppsPageComponent::startApp - " << appButton->getCommand());
    ChildProcess* launchApp = new ChildProcess();
    launchApp->start("xmodmap ${HOME}/.Xmodmap"); // Reload xmodmap to ensure it's running
#if JUCE_DEBUG
    File launchLog("launchLog.txt");
    if(!launchLog.existsAsFile()){
        launchLog.create();
    }
    launchLog.appendText(appButton->getCommand(),false,false);
#endif
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

void AppMenuComponent::focusApp(AppMenuButton::Ptr appButton, const String & windowId) {

    DBG("AppsPageComponent::focusApp - " << appButton->getCommand());
    String focusShell = "echo 'focus_client_by_window_id(" + windowId + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
}

void AppMenuComponent::startOrFocusApp(AppMenuButton::Ptr appButton) {
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