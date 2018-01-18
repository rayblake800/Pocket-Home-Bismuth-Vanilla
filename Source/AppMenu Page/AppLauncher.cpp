/*
  ==============================================================================

    AppLauncher.cpp
    Created: 18 Jan 2018 3:21:50pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "AppLauncher.h"

AppLauncher::AppLauncher() :
launchFailureCallback([]()
{
})

,
launchTimer(this)
{
}

AppLauncher::~AppLauncher()
{
}

/**
 * Assigns a function to call if loading an application fails.
 */
void AppLauncher::setLaunchFailureCallback(std::function<void() > failureCallback)
{
    launchFailureCallback = failureCallback;
}

/**
 * Launch a new application, or focus its window if the application is
 * already running
 */
void AppLauncher::startOrFocusApp(String appTitle, String command)
{
    DBG(String("Attempting to launch ") + appTitle);
    //before adding another process to the list, clean out any old dead ones,
    //so they don't start piling up
    std::vector<ChildProcess*>toRemove;
    for (int i = 0; i < runningApps.size(); i++)
    {
        ChildProcess* appProcess = runningApps[i];
        if (appProcess != nullptr && !appProcess->isRunning())
        {
            toRemove.push_back(appProcess);
        }
    }
    ProcessInfo processInfo(appTitle, command);
    for (ChildProcess* appProcess : toRemove)
    {
        processMap.erase(processInfo);
        runningApps.removeObject(appProcess);
    }
    ChildProcess* appProcess = processMap[processInfo];
    if (appProcess != nullptr)
    {
        if (appProcess->isRunning())
        {
            DBG("app is already running, attempting to find the window id");
            String windowId = getWindowId(processInfo);

            if (!windowId.isEmpty())
            {
                DBG(String("Found window ") + windowId + String(", focusing app"));
                focusApp(windowId);

            } else
            {
                DBG("Process exists, but has no window to focus.");
            }
            return;
        } else
        {
            if (appProcess != nullptr)
            {
                DBG("Old process is dead, we're good to launch");
            }
        }
    }
    startApp(processInfo);
}

/**
 * If there is a timer tracking application launching, cancel it.
 */
void AppLauncher::stopTimer()
{
    launchTimer.stopTimer();
}

/**
 * Attempt to find an open window of a launched application
 */
String AppLauncher::getWindowId(ProcessInfo processInfo)
{
    std::function < String(String) > windowSearch =
            [this](String searchTerm)->String
            {
                StringArray findCmd{"xdotool", "search", "--all"
                    , "--limit", "1", "--class", searchTerm.toRawUTF8()};
                DBG(String("Running command:")
                        + findCmd.joinIntoString(" ", 0, -1));
                ChildProcess findWindow;
                findWindow.start(findCmd);
                String windowId = findWindow.readAllProcessOutput();
                DBG(String("Search result:") + windowId);
                return windowId.trimEnd();
            };
    String result = windowSearch(processInfo.title);
    //if no result on the title, try the launch command
    if (result.isEmpty())
    {
        result = windowSearch(processInfo.command
                .upToFirstOccurrenceOf(" ", false, true));
    }
    return result;
}

void AppLauncher::startApp(ProcessInfo processInfo)
{
    String command = processInfo.command;
    DBG("AppsPageComponent::startApp - " << processInfo.command);
    String testExistance = String("command -v ") + processInfo.command;
    if (system(testExistance.toRawUTF8()) != 0)
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                "Couldn't open application", String("\"") + processInfo.command
                + String("\" is not a valid command."));
        launchFailureCallback();
        return;

    }
    ChildProcess* launchApp = new ChildProcess();
    // Reload xmodmap to ensure it's running
    launchApp->start("xmodmap ${HOME}/.Xmodmap");
    if (launchApp->start(processInfo.command))
    {
        runningApps.add(launchApp);
        processMap[processInfo] = launchApp;
        launchTimer.setTrackedProcess(launchApp);
        launchTimer.startTimer(AppLaunchTimer::frequency);
    }
}

void AppLauncher::focusApp(const String & windowId)
{
    String focusShell = "echo 'focus_client_by_window_id(" + windowId
            + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
}

AppLauncher::ProcessInfo::ProcessInfo(String title, String command) :
title(title), command(command)
{
}

bool AppLauncher::ProcessInfo::operator==(const ProcessInfo& rhs) const
{
    return title == rhs.title && command == rhs.command;
}

bool AppLauncher::ProcessInfo::operator<(const ProcessInfo& rhs) const
{
    return title.compare(rhs.title)<0;
}

AppLauncher::AppLaunchTimer::AppLaunchTimer(AppLauncher* launcher) :
launcher(launcher)
{
}

AppLauncher::AppLaunchTimer::~AppLaunchTimer()
{
    launcher = nullptr;
    trackedProcess = nullptr;
    stopTimer();
}

void AppLauncher::AppLaunchTimer::setTrackedProcess(ChildProcess * trackedProcess)
{
    this->trackedProcess = trackedProcess;
}

void AppLauncher::AppLaunchTimer::stopTimer()
{
    trackedProcess = nullptr;
    this->Timer::stopTimer();
}

void AppLauncher::AppLaunchTimer::timerCallback()
{
    DBG("AppLaunchTimer callback");
    if (launcher != nullptr)
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
                Array<String> lines = split(output, "\n");
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
        launcher->launchFailureCallback();
    }
    stopTimer();

}