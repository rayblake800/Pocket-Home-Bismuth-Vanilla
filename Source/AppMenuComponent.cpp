/* 
 * File:   AppMenuComponent.cpp
 * Author: anthony
 * Created on December 19, 2017, 1:37 PM
 */


#include <sstream>
#include <functional>
#include <stdlib.h>
#include <vector>
#include "Utils.h"
#include "AppMenuComponent.h"


AppMenuComponent::AppMenuComponent(const var &configJson, Rectangle<int>drawRegion) :
clipBounds(drawRegion),
runningCheckTimer(this, [](AppMenuComponent* appMenu) {
    appMenu->checkRunningApps();
}),
debounceTimer(this, [](AppMenuComponent* appMenu) {

    appMenu->setDebounce(false);
}),
buttonWidth(configJson["menuButtonWidth"]),
buttonHeight(configJson["menuButtonHeight"]) {

    launchSpinner = new OverlaySpinner();
    selected.push_back(NULL);
    columnTops.push_back(0);
    buttonColumns.emplace(buttonColumns.begin());

    //read in main page apps from config
    Array<var>* pagesData = configJson["pages"].getArray();
    if (pagesData) {
        for (const var &page : *pagesData) {
            String name = page["name"].toString();
            if (name == "Apps") {
                var items = page["items"];
                for (const var &item : *items.getArray()) {
                    if (item["name"].isString()
                            && item["shell"].isString()
                            && item["icon"].isString()) {
                        DBG(String("AppMenu:Found app in config:") + item["name"].toString());
                        addButton(new AppMenuButton(DesktopEntry(item),
                                buttonColumns[activeColumn()].size(), activeColumn(),
                                buttonWidth, buttonHeight));
                    }
                }
            }
        }
    }
    //add category buttons
    std::vector<DesktopEntry> categories = desktopEntries.getMainCategories(true);
    for (int i = 0; i < categories.size(); i++) {
        addButton(new AppMenuButton(categories[i],
                buttonColumns[activeColumn()].size(), activeColumn(),
                buttonWidth, buttonHeight));
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
void AppMenuComponent::openFolder(String categoryName) {
    std::vector<DesktopEntry> folderItems = desktopEntries.getCategoryEntries(categoryName);
    if (folderItems.empty())return;
    int columnTop = clipBounds.getY();
    if (selected[activeColumn()] != NULL) {
        columnTop = selected[activeColumn()]->getY();
    }
    selected.push_back(NULL);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton*>());
    DBG(String("found ") + String(folderItems.size()) + " items in " + categoryName);
    for (int i = 0; i < folderItems.size(); i++) {
        DesktopEntry desktopEntry = folderItems[i];
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay()) {
            AppMenuButton* newButton = new AppMenuButton(desktopEntry,
                    buttonColumns[activeColumn()].size(), activeColumn(),
                    buttonWidth, buttonHeight);
            addButton(newButton);
        }
    }
    scrollToSelected();
}

//close the topmost open folder, removing all contained buttons

void AppMenuComponent::closeFolder() {
    if (debounce)return;
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
            openFolder(appClicked->getAppName());
        } else {
            startApp(appClicked);
        }
    } else {
        selectIndex(appClicked->getIndex());
    }
}

void AppMenuComponent::resized() {
    launchSpinner->setBounds(Desktop::getInstance().getDisplays().getMainDisplay().userArea);
    //if (selected[activeColumn()] != NULL){
    //    scrollToSelected();
    //}
}

void AppMenuComponent::selectIndex(int index) {
    if (debounce) {
        return;
    }
    int column = activeColumn();
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
        int screenHeight = Desktop::getInstance().getDisplays().getMainDisplay().userArea.getHeight();
        int distanceFromCenter = abs(buttonPos - getY() + screenHeight / 2);
        //only scroll vertically if selected button is outside the center 3/5 
        if (distanceFromCenter > screenHeight / 5 * 3) {
            dest.setY(clipBounds.getY() - buttonPos + screenHeight / 2 - buttonHeight / 2);
        }
        if (dest.getY() > clipBounds.getY()) {
            dest.setY(clipBounds.getY());
        } else if (getHeight() > screenHeight && dest.getBottom() < screenHeight) {
            dest.setBottom(screenHeight);
        }
    } else dest.setY(clipBounds.getY() - columnTops[column]);
    if (column == 0)dest.setX(clipBounds.getX());
    else {
        dest.setX(clipBounds.getX() - column * buttonWidth + buttonHeight);
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
    int x = clipBounds.getX() + column*w;
    int y = columnTops[column] + h * buttonColumns[column].size();
    appButton->setBounds(x, y, w, h);
    addAndMakeVisible(appButton);
    appButton->setEnabled(true);
    appButton->setVisible(true);
    appButton->addListener(this);
    appButton->setClipRegion(clipBounds);
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

void AppMenuComponent::focusApp(AppMenuButton* appButton, const String& windowId) {

    DBG("AppsPageComponent::focusApp - " << appButton->getCommand());
    String focusShell = "echo 'focus_client_by_window_id(" + windowId + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
}

void AppMenuComponent::startOrFocusApp(AppMenuButton* appButton) {
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