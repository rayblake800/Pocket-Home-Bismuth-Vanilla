/* 
 * File:   AppMenuComponent.cpp
 * Author: anthony
 * Created on December 19, 2017, 1:37 PM
 */


#include <set>
#include "../PocketHomeApplication.h"
#include "AppMenuComponent.h"

AppMenuComponent::AppMenuComponent() : launchTimer(this)
{

    MainConfigFile& configFile =
            PocketHomeApplication::getInstance()->getConfig();
    Rectangle<int> screenSize = getWindowSize();
    MainConfigFile::ComponentSettings menuSettings =
            configFile.getComponentSettings(APP_MENU);
    Rectangle<int>bounds = menuSettings.getBounds();
    x_origin = bounds.getX();
    y_origin = bounds.getY();

    MainConfigFile::ComponentSettings buttonSettings =
            configFile.getComponentSettings(APP_MENU_BUTTON);
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
    std::vector<MainConfigFile::AppItem> favorites = configFile.getFavorites();
    for (const MainConfigFile::AppItem& favorite : favorites)
    {
        DBG(String("AppMenu:Found app in config:") + favorite.name);
        addButton(new AppMenuButton(DesktopEntry(favorite),
                buttonColumns[activeColumn()].size(), activeColumn()));
    }

    //add category buttons
    std::vector<MainConfigFile::AppFolder> categories = configFile.getFolders();
    for (const MainConfigFile::AppFolder& category : categories)
    {
        addButton(new AppMenuButton(DesktopEntry(category),
                buttonColumns[activeColumn()].size(), activeColumn()));
    }
    DBG(String("added ") + String(buttonColumns[activeColumn()].size())
            + " buttons");
    scrollToSelected();
}

AppMenuComponent::~AppMenuComponent()
{
    stopWaitingOnLaunch();
    while (!buttonColumns.empty())closeFolder();
}

/**
 * Open an application category folder, creating AppMenuButtons for all
 * associated desktop applications.
 */
void AppMenuComponent::openFolder(std::vector<String> categoryNames)
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    int folderIndex = selected[activeColumn()]->getIndex() -
            config.getFavorites().size();
    MainConfigFile::AppFolder selectedFolder = config.getFolders()[folderIndex];
    std::set<DesktopEntry> folderItems =
            desktopEntries.getCategoryListEntries(categoryNames);
    if (folderItems.empty() && selectedFolder.pinnedApps.empty())return;
    int columnTop = y_origin;
    if (selected[activeColumn()] != nullptr)
    {
        columnTop = selected[activeColumn()]->getY();
    }
    selected.push_back(nullptr);
    columnTops.push_back(columnTop);
    buttonColumns.push_back(std::vector<AppMenuButton::Ptr>());
    for (MainConfigFile::AppItem item : selectedFolder.pinnedApps)
    {
        AppMenuButton::Ptr addedButton;
        if (buttonNameMap[item.name] != nullptr &&
                buttonNameMap[item.name]->getParentComponent() == nullptr)
        {
            addedButton = buttonNameMap[item.name];
            addedButton->setIndex(buttonColumns[activeColumn()].size());
            addedButton->setColumn(activeColumn());
        } else
        {
            addedButton = new AppMenuButton(DesktopEntry(item),
                    buttonColumns[activeColumn()].size(), activeColumn());
        }
        addButton(addedButton);
    }
    DBG(String("found ") + String(folderItems.size()) + " items in folder");
    for (DesktopEntry desktopEntry : folderItems)
    {
        if (!desktopEntry.hidden() && !desktopEntry.noDisplay())
        {
            String name = desktopEntry.getName();
            AppMenuButton::Ptr addedButton;
            if (buttonNameMap[name] != nullptr &&
                    buttonNameMap[name]->getParentComponent() == nullptr)
            {
                addedButton = buttonNameMap[name];
                addedButton->setIndex(buttonColumns[activeColumn()].size());
                addedButton->setColumn(activeColumn());
            } else
            {
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

void AppMenuComponent::closeFolder()
{
    if (waitingOnLaunch())
    {
        return;
    }
    for (int i = buttonColumns[activeColumn()].size() - 1; i >= 0; i--)
    {
        AppMenuButton::Ptr toRemove = buttonColumns[activeColumn()][i];
        toRemove->setVisible(false);
        toRemove->setSelected(false);
        Component * parent = toRemove->getParentComponent();
        if (parent != nullptr)
        {
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

void AppMenuComponent::buttonClicked(Button * buttonClicked)
{
    if (waitingOnLaunch())
    {
        return;
    }
    AppMenuButton::Ptr appClicked = (AppMenuButton *) buttonClicked;
    if (appClicked->getColumn() < activeColumn())
    {
        while (appClicked->getColumn() < activeColumn())
        {
            closeFolder();
        }
        selectIndex(appClicked->getIndex());
        return;
    }
    if (selected[activeColumn()] == appClicked)
    {
        if (appClicked->isFolder())
        {
            openFolder(appClicked->getCategories());
        } else
        {
            startOrFocusApp(appClicked);
        }
    } else
    {
        selectIndex(appClicked->getIndex());
    }
}

void AppMenuComponent::resized()
{
    launchSpinner->setBounds(getWindowSize());
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    MainConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(APP_MENU);
    Rectangle<int> menuBounds = menuSettings.getBounds();
    x_origin = menuBounds.getX();
    y_origin = menuBounds.getY();
    //resize all buttons
    Rectangle<int>buttonSize = AppMenuButton::getButtonSize();
    buttonWidth = buttonSize.getWidth();
    buttonHeight = buttonSize.getHeight();
    int numColumns = selected.size();
    if (menuBounds.getWidth() < numColumns * buttonWidth)
    {
        menuBounds.setWidth(numColumns * buttonWidth);
    }
    for (int c = 0; c < numColumns; c++)
    {
        if (c > 0)
        {
            columnTops[c] = selected[c - 1]->getY();
        }
        int numRows = buttonColumns[c].size();
        if (menuBounds.getHeight() < numRows * buttonHeight + columnTops[c])
        {
            menuBounds.setHeight(numRows * buttonHeight + columnTops[c]);
        }
        for (int i = 0; i < numRows; i++)
        {
            AppMenuButton * button = buttonColumns[c][i];
            button->setBounds(buttonSize.withPosition(c*buttonWidth,
                    i * buttonHeight + columnTops[c]));
        }
    }
    setBounds(menuBounds);
    if (activeColumn() >= 0 && selected[activeColumn()] != nullptr
            && !Desktop::getInstance().getAnimator().isAnimating())
    {
        scrollToSelected();
    }
}


//if it loses visibility, stop waiting for apps to launch

void AppMenuComponent::visibilityChanged()
{
    if (!isVisible())
    {
        stopWaitingOnLaunch();
    }
}

void AppMenuComponent::selectIndex(int index)
{
    if (waitingOnLaunch())
    {
        return;
    }
    int column = activeColumn();
    if (index >= buttonColumns[column].size()
            || index < 0
            || selected[column] == buttonColumns[column][index])return;
    if (selected[column] != nullptr)
    {
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

void AppMenuComponent::selectNext()
{
    if (selected[activeColumn()] == nullptr)
    {
        selectIndex(0);
    } else
    {
        selectIndex(selected[activeColumn()]->getIndex() + 1);
    }
}

//Select the previous appMenuButton in the active button column.

void AppMenuComponent::selectPrevious()
{
    if (selected[activeColumn()] == nullptr)
    {
        selectIndex(0);
    } else
    {
        selectIndex(selected[activeColumn()]->getIndex() - 1);
    }
}

//Trigger a click for the selected button.

void AppMenuComponent::clickSelected()
{
    if (selected[activeColumn()] != nullptr && !waitingOnLaunch())
    {
        selected[activeColumn()]->triggerClick();
    }
}

//return the index of the active button column.

int AppMenuComponent::activeColumn()
{
    return selected.size() - 1;
}

void AppMenuComponent::scrollToSelected()
{
    int column = activeColumn();
    if (column < 0)
    {
        return;
    }
    AppMenuButton::Ptr selectedButton = selected[column];
    Rectangle<int>dest = getBounds();
    if (selectedButton != nullptr && selectedButton->isVisible())
    {
        int buttonPos = selectedButton->getY();
        int screenHeight = getWindowSize().getHeight();
        int distanceFromCenter = abs(buttonPos - getY() + screenHeight / 2);
        //only scroll vertically if selected button is outside the center 3/5 
        if (distanceFromCenter > screenHeight / 5 * 3)
        {
            dest.setY(y_origin - buttonPos + screenHeight / 2 - buttonHeight / 2);
        }
        if (dest.getY() > y_origin)
        {
            dest.setY(y_origin);
        }
    } else dest.setY(y_origin - columnTops[column]);
    if (column == 0)dest.setX(x_origin);
    else
    {
        dest.setX(x_origin - column * buttonWidth + buttonHeight);
    }
    ComponentAnimator& animator = Desktop::getInstance().getAnimator();
    if (animator.isAnimating(this))
    {
        animator.cancelAnimation(this, false);
    }
    animator.animateComponent(this, dest, 1, 100, false, 1, 1);
}

void AppMenuComponent::addButton(AppMenuButton::Ptr appButton)
{
    String name = appButton->getAppName();
    if (buttonNameMap[name] == nullptr)
    {
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
    if ((x + buttonWidth) > getWidth())
    {
        setBounds(getX(), getY(), x + buttonWidth, getHeight());
    }
    if ((y + buttonHeight) > getHeight())
    {
        setBounds(getX(), getY(), getWidth(), y + buttonHeight);
    }
}

//################## Application Launching #################################

/**
 * @return true if currently waiting on an application to launch.
 */
bool AppMenuComponent::waitingOnLaunch()
{
    return launchSpinner->isShowing();
}

/**
 * Makes the menu stop waiting on an application to launch, re-enabling
 * user input.
 */
void AppMenuComponent::stopWaitingOnLaunch()
{
    hideLaunchSpinner();
    launchTimer.stopTimer();
}

AppMenuComponent::AppLaunchTimer::AppLaunchTimer(AppMenuComponent* appMenu) :
appMenu(appMenu)
{
}

AppMenuComponent::AppLaunchTimer::~AppLaunchTimer()
{
    appMenu = nullptr;
    trackedProcess = nullptr;
    stopTimer();
}

void AppMenuComponent::AppLaunchTimer::setTrackedProcess(ChildProcess * trackedProcess)
{
    this->trackedProcess = trackedProcess;
}

void AppMenuComponent::AppLaunchTimer::stopTimer()
{
    trackedProcess = nullptr;
    ((Timer*)this)->stopTimer();
}

void AppMenuComponent::AppLaunchTimer::timerCallback()
{
    DBG("timer callback");
    if (appMenu != nullptr)
    {
        if (trackedProcess != nullptr)
        {
            DBG("tracked process is null");
            if (trackedProcess->isRunning())
            {
                //if the process is still going, wait longer for it to take over
                //if not, stop waiting on it
                return;
            } else
            {
                DBG("process dies, show message");
                String output = trackedProcess->readAllProcessOutput();
                std::vector<String> lines = split(output, "\n");
                output = "";
                for (int i = lines.size() - 1;
                        i > lines.size() - 6 && i >= 0; i--)
                {
                    output = lines[i] + String("\n") + output;
                }
                AlertWindow::showMessageBoxAsync
                        (AlertWindow::AlertIconType::WarningIcon,
                        "Couldn't open application", output);
                trackedProcess = nullptr;
            }
        }
        appMenu->hideLaunchSpinner();
    }
    stopTimer();

}

void AppMenuComponent::startApp(AppMenuButton::Ptr appButton)
{
    String command = appButton->getCommand();
    DBG("AppsPageComponent::startApp - " << command);
    String testExistance = String("command -v ") + command;
    if (system(testExistance.toRawUTF8()) != 0)
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                "Couldn't open application", String("\"") + command
                + String("\" is not a valid command."));
        return;

    }
    ChildProcess* launchApp = new ChildProcess();
    launchApp->start("xmodmap ${HOME}/.Xmodmap"); // Reload xmodmap to ensure it's running
#if JUCE_DEBUG
    File launchLog("launchLog.txt");
    if (!launchLog.existsAsFile())
    {
        launchLog.create();
    }
    launchLog.appendText(appButton->getCommand() + String("\n"), false, false);
#endif
    if (launchApp->start(appButton->getCommand()))
    {

        runningApps.add(launchApp);
        runningAppsByButton.set(appButton, runningApps.indexOf(launchApp));
        launchTimer.setTrackedProcess(launchApp);
        launchTimer.startTimer(2 * 1000);
        showLaunchSpinner();
    }
}

void AppMenuComponent::focusApp(AppMenuButton::Ptr appButton, const String & windowId)
{
    DBG("AppsPageComponent::focusApp - " << appButton->getCommand());
    String focusShell = "echo 'focus_client_by_window_id(" + windowId + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
}

String AppMenuComponent::getWindowId(AppMenuButton::Ptr appButton)
{
    std::function < String(String) > windowSearch = [this](String searchTerm)->String
    {
        StringArray findCmd{"xdotool", "search", "--all", "--limit", "1", "--class", searchTerm.toRawUTF8()};
        DBG(String("Running command:") + findCmd.joinIntoString(" ", 0, -1));
        ChildProcess findWindow;
        findWindow.start(findCmd);
        String windowId = findWindow.readAllProcessOutput();
        DBG(String("Search result:") + windowId);
        return windowId.trimEnd();
    };
    String result = windowSearch(appButton->getAppName());
    //if no result on the title, try the launch command
    if (result.isEmpty())
    {
        result = windowSearch(appButton->getCommand().upToFirstOccurrenceOf(" ", false, true));
    }
    return result;
}

void AppMenuComponent::startOrFocusApp(AppMenuButton::Ptr appButton)
{
    if (waitingOnLaunch())
    {
        return;
    }
    DBG(String("Attempting to launch ") + appButton->getAppName());
    //before adding another process to the list, clean out any old dead ones,
    //so they don't start piling up
    std::vector<int>toRemove;
    for (int i = 0; i < runningApps.size(); i++)
    {
        if (runningApps[i] != nullptr && !runningApps[i]->isRunning())
        {
            toRemove.push_back(i);
        }
    }
    for (int index : toRemove)
    {
        runningApps.remove(index, true);
        runningAppsByButton.removeValue(index);
    }
    if (runningAppsByButton.contains(appButton))
    {
        int appId = runningAppsByButton[appButton];
        if (runningApps[appId] != nullptr && runningApps[appId]->isRunning())
        {
            DBG("app is already running, attempting to find the window id");
            String windowId = getWindowId(appButton);

            if (!windowId.isEmpty())
            {
                DBG(String("Found window ") + windowId + String(", focusing app"));
                focusApp(appButton, windowId);

            } else
            {
                DBG("Process exists, but has no window to focus. Leave it alone for now.");
            }
            return;
        } else
        {
            if (runningApps[appId] != nullptr)
            {
                DBG("Old process is dead, we're good to launch");
            }
        }
    }
    startApp(appButton);
}

void AppMenuComponent::showLaunchSpinner()
{
    DBG("Show launch spinner");
    Component * parentPage = getParentComponent();
    if (parentPage != nullptr)
    {
        parentPage->addAndMakeVisible(launchSpinner);
    }
}

void AppMenuComponent::hideLaunchSpinner()
{
    DBG("Hide launch spinner");
    Component * parentPage = getParentComponent();
    if (parentPage != nullptr)
    {
        parentPage->removeChildComponent(launchSpinner);
    }
}
