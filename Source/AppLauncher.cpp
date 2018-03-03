#include "Utils.h"
#include "AppLauncher.h"

AppLauncher::AppLauncher() :
launchFailureCallback([]()
{
}) { }

AppLauncher::~AppLauncher() { }

/**
 * Assigns a function to call if loading an application fails.
 */
void AppLauncher::setLaunchFailureCallback
(std::function<void() > failureCallback)
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

            }
            else
            {
                DBG("Process exists, but has no window to focus.");
            }
            return;
        }
        else
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
        timedProcess = launchApp;
        startTimer(timerFrequency);
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
title(title), command(command) { }

bool AppLauncher::ProcessInfo::operator==(const ProcessInfo& rhs) const
{
    return title == rhs.title && command == rhs.command;
}

bool AppLauncher::ProcessInfo::operator<(const ProcessInfo& rhs) const
{
    return title.compare(rhs.title) < 0;
}

void AppLauncher::timerCallback()
{
    DBG("AppLaunchTimer callback");
    if (timedProcess != nullptr)
    {
        DBG("tracked process is null");
        if (timedProcess->isRunning())
        {
            //if the process is still going, wait longer for it to take over
            //if not, stop waiting on it
            return;
        }
        else
        {
            DBG("process dies, show message");
            String output = timedProcess->readAllProcessOutput();
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
            timedProcess = nullptr;
        }
    }
    launchFailureCallback();
    stopTimer();

}

/**
 * Stop process tracking if window focus changes and the timer is suspended.
 */
void AppLauncher::onSuspend()
{
    stopTimer();
}