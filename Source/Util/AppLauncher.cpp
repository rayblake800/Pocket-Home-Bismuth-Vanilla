#include "Utils.h"
#include "AppLauncher.h"
#include "XWindowInterface.h"

//Ms to wait before forcibly terminating a window focus operation.
static const int windowFocusTimeout = 1000;

/**
 * Launch a new application, or focus its window if the application is
 * already running
 */
void AppLauncher::startOrFocusApp(juce::String appTitle, juce::String command)
{
    using namespace juce;
    DBG("AppLauncher::" << __func__ << ": title = " << appTitle
            << ", command = " << command);
    //before adding another process to the list, clean out any old dead ones,
    //so they don't start piling up
    std::vector<LaunchedApp*> toRemove;
    LaunchedApp* appInstance = nullptr;
    for (LaunchedApp* app : runningApps)
    {
        if (!app->isRunning())
        {
            toRemove.push_back(app);
        }
        else if(app->getLaunchCommand() == command)
        {
            appInstance = app;
        }
    }
    for (LaunchedApp* appProcess : toRemove)
    {
        runningApps.removeObject(appProcess);
    }
    if (appInstance != nullptr)
    {
        if (appInstance->isRunning())
        {
            DBG("AppLauncher::" << __func__
                    << ": app is already running,"
                    << " focusing the window");
            appInstance->activateWindow();
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
    LaunchedApp* newApp = startApp(processInfo);
    if(newApp != nullptr)
    {
        runningApps.add(newApp);
    }
    else
    {
        DBG("AppLauncher::" << __func__
                << ": Failed to launch " << appTitle);
    }
}

/**
 * Start a new instance of an application process
 */
LaunchedApp* AppLauncher::startApp(const juce::String& command)
{
    using namespace juce;
    DBG("AppsPageComponent::startApp - " << command);
    String testExistance = String("command -v ") + command;
    if (system(testExistance.toRawUTF8()) != 0)
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                localeText(could_not_open),
                String("\"") + processInfo.command + String("\"")
                + localeText(not_valid_command));
        launchFailureCallback();
        return nullptr;
    }
    
    LaunchedApp* newApp = new LaunchedApp(command);    
    timedProcess = newApp;
    lastLaunch = juce::Time::getMillisecondCounter();
    startTimer(timerFrequency);
    return launchedProcess;
}

void AppLauncher::timerCallback()
{
    using namespace juce;
    if (timedProcess != nullptr)
    {
        if (timedProcess->isRunning())
        {
            //if the process is still going and we have yet to reach timeout,
            //wait longer
            if (juce::Time::getMillisecondCounter() - lastLaunch < timeout)
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
