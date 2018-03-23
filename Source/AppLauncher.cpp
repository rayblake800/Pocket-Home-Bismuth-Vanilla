#include "Utils.h"
#include "AppLauncher.h"

/**
 * Launch a new application, or focus its window if the application is
 * already running
 */
void AppLauncher::startOrFocusApp(String appTitle, String command)
{
    DBG("AppLauncher::" << __func__ << ": title = " << appTitle
            << ", command = " << command);
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
            DBG("AppLauncher::" << __func__
                    << ": app is already running,"
                    << " attempting to find the window id");
            String windowId = getWindowId(processInfo);

            if (!windowId.isEmpty())
            {
                DBG("AppLauncher::" << __func__ << ": Found window "
                        << windowId << ", focusing app");
                focusApp(windowId);
            }
            else
            {
                DBG("AppLauncher::" << __func__
                        << ": Process exists, but has no window to focus.");
            }
            return;
        }
        else
        {
            if (appProcess != nullptr)
            {
                DBG("AppLauncher::" << __func__
                        << ": Old process is dead, re-launching");
            }
        }
    }
    startApp(processInfo);
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

/**
 * Start a new instance of an application process
 */
void AppLauncher::startApp(ProcessInfo processInfo)
{
    String command = processInfo.command;
    DBG("AppsPageComponent::startApp - " << processInfo.command);
    String testExistance = String("command -v ") + processInfo.command;
    if (system(testExistance.toRawUTF8()) != 0)
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                localeText(could_not_open),
                String("\"") + processInfo.command + String("\"")
                + localeText(not_valid_command));
        launchFailureCallback();
        return;

    }
    ChildProcess* launchApp = new ChildProcess();
    // Reload xmodmap to ensure it's running
    // Commented for testing, remember to clean this out if it turns out to
    // be unneeded
    //launchApp->start("xmodmap ${HOME}/.Xmodmap");
    if (launchApp->start(processInfo.command))
    {
        runningApps.add(launchApp);
        processMap[processInfo] = launchApp;
        timedProcess = launchApp;
        lastLaunch = Time::getMillisecondCounter();
        startTimer(timerFrequency);
    }
}

/**
 * Focus the window of a running app
 */
void AppLauncher::focusApp(const String & windowId)
{
    String focusShell = "echo 'focus_client_by_window_id(" + windowId
            + ")' | awesome-client";
    StringArray focusCmd{"sh", "-c", focusShell.toRawUTF8()};
    ChildProcess focusWindow;
    focusWindow.start(focusCmd);
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
                DBG("AppLauncher::" << __func__ << ": Running command:"
                        << findCmd.joinIntoString(" ", 0, -1));
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

void AppLauncher::timerCallback()
{
    if (timedProcess != nullptr)
    {
        if (timedProcess->isRunning())
        {
            //if the process is still going and we have yet to reach timeout,
            //wait longer
            if (Time::getMillisecondCounter() - lastLaunch < timeout)
            {
                return;
            }
            else
            {
                DBG("AppLauncher::" << __func__ << ": launch timed out");
            }
        }
        else
        {
            DBG("AppLauncher::" << __func__ << ": process died, show message");
            String output = timedProcess->readAllProcessOutput();
            StringArray lines = StringArray::fromLines(output);
            output = "";
            for (int i = lines.size() - 1;
                 i > lines.size() - 6 && i >= 0; i--)
            {
                output = lines[i] + String("\n") + output;
            }
            AlertWindow::showMessageBoxAsync
                    (AlertWindow::AlertIconType::WarningIcon,
                    localeText(could_not_open), output);
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