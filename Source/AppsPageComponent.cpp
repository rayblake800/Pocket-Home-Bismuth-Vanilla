#include "AppsPageComponent.h"
#include "LauncherComponent.h"
#include "PokeLookAndFeel.h"
#include "Main.h"
#include "Utils.h"

using namespace std;

void AppCheckTimer::timerCallback() {
    DBG("AppCheckTimer::timerCallback - check running apps");
    if (appsPage) {
        appsPage->checkRunningApps();
    }
}

void AppDebounceTimer::timerCallback() {
    DBG("AppDebounceTimer::timerCallback - check launch debounce");
    if (appsPage) {
        appsPage->debounce = false;
    }
    stopTimer();
}



AppListComponent::AppListComponent(Component* parent, bool ishorizontal) {
    var config = getConfigJSON();
    Rectangle<int> appMenuBorder = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    appMenuBorder.reduce(0,50);
    appMenu=new AppMenu(config,appMenuBorder);
    addAndMakeVisible(appMenu);
}

AppListComponent::~AppListComponent() {}


void AppListComponent::resized() {}//TODO:update or remove

AppsPageComponent::AppsPageComponent(LauncherComponent* launcherComponent, bool horizontal) :
AppListComponent(launcherComponent, horizontal),
launcherComponent(launcherComponent),
runningCheckTimer(),
debounceTimer(),
x(-1), y(-1), shouldMove(false) {
    runningCheckTimer.appsPage = this;
    debounceTimer.appsPage = this;
    cpy = nullptr;

    //Trash Icon
    trashButton = createImageButton(
            "Trash", createImageFromFile(assetFile("trash.png")));
    trashButton->setName("Trash");
    trashButton->setAlwaysOnTop(true);
    addAndMakeVisible(trashButton, 100);
    trashButton->setBounds(170, 15, 40, 20);
    trashButton->setVisible(false);
    // Focus keyboard needed for the key listener
    appMenu->setLaunchFunction([this](AppMenuButton * appButton){
        this->startApp(appButton);
    });
    setWantsKeyboardFocus(true);
}

AppsPageComponent::~AppsPageComponent() {
}


void AppsPageComponent::startApp(AppMenuButton * appButton) {
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
        launcherComponent->showLaunchSpinner();
    }
};

void AppsPageComponent::focusApp(AppMenuButton* appButton, const String& windowId) {
    DBG("AppsPageComponent::focusApp - " << appButton->getCommand());
    String focusShell = "echo 'focus_client_by_window_id(" + windowId + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
};

void AppsPageComponent::startOrFocusApp(AppMenuButton* appButton) {
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

};

void AppsPageComponent::checkRunningApps() {
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
        launcherComponent->hideLaunchSpinner();
    }
};

bool AppsPageComponent::keyPressed(const KeyPress& key) {
    //don't interrupt animation
    if(Desktop::getInstance().getAnimator().isAnimating(appMenu))return false;
    int keyCode = key.getKeyCode();
    if(keyCode==KeyPress::upKey || keyCode==KeyPress::downKey){
        if(keyCode==KeyPress::upKey)appMenu->selectPrevious();
        else appMenu->selectNext();
        grabKeyboardFocus();
        return true;
    }
    else if(keyCode==KeyPress::leftKey || keyCode==KeyPress::escapeKey){
        if(appMenu->activeColumn()>0){
            appMenu->closeFolder();
            grabKeyboardFocus();
        }
        return true;
    }
    else if(keyCode==KeyPress::returnKey || 
            keyCode==KeyPress::spaceKey || 
            keyCode==KeyPress::rightKey){
        appMenu->clickSelected();
        grabKeyboardFocus();
        return true;
    }
    return false;
}


/* left is true if the current icon has to be switched 
 * with the one on its left */
void AppsPageComponent::moveInConfig(AppMenuButton* icon, bool left) {
    //Get the global configuration
    var json = getConfigJSON();
    Array<var>* pages_arr = (json["pages"].getArray());
    const var& pages = ((*pages_arr)[0]);
    Array<var>* items_arr = pages["items"].getArray();

    //Offset of the icon to switch with
    int nxtoffset = left ? -1 : 1;
    //Searching for the element in the Array
    for (int i = 0; i < items_arr->size(); i++) {
        const var& elt = (*items_arr)[i];
        if (elt["name"] == icon->getName() && elt["shell"] == icon->getCommand()) {
            items_arr->swap(i, i + nxtoffset);
            break;
        }
    }

    //Write the config object to the file
    File config = getConfigFile();
    DynamicObject* obj = json.getDynamicObject();
    String s = JSON::toString(json);
    config.replaceWithText(s);
}

