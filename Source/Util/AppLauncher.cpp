#include "Utils.h"
#include "AppLauncher.h"
#include "XWindowInterface.h"

//Ms to wait before forcibly terminating a window focus operation.
static const int windowFocusTimeout = 1000;

/**
 * Launch a new application, or focus its window if the application is
 * already running
 */
void AppLauncher::startOrFocusApp(const juce::String appTitle,
        const juce::String command)
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
        DBG("AppLauncher::" << __func__ << ": removing terminated app "
                << appProcess->getLaunchCommand());
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
            if (appInstance != nullptr)
            {
                DBG("AppLauncher::" << __func__
                        << ": Old process is dead, re-launching");
            }
        }
    }
    LaunchedApp* newApp = startApp(command);
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

/*
 * Checks a string to see if it is a valid shell command.
 */
bool AppLauncher::testCommand(const juce::String command)
{
    using namespace juce;
    String testResult = String("command -v ") + command;
    return system(testResult.toRawUTF8()) == 0;
}


/**
 * Start a new instance of an application process
 */
LaunchedApp* AppLauncher::startApp(const juce::String command)
{
    using namespace juce;
    DBG("AppsPageComponent::startApp - " << command);
    if (!testCommand(command))
    {
        AlertWindow::showMessageBoxAsync
                (AlertWindow::AlertIconType::WarningIcon,
                localeText(could_not_open),
                String("\"") + command + String("\"")
                + localeText(not_valid_command));
        launchFailureCallback();
        return nullptr;
    }
    
    LaunchedApp* newApp = new LaunchedApp(command);    
    timedProcess = newApp;
    lastLaunch = juce::Time::getMillisecondCounter();
    startTimer(timerFrequency);
    return newApp;
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
            String output = timedProcess->getProcessOutput();
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
