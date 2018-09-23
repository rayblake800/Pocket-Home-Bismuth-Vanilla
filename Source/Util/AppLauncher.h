#pragma once
#include <functional>
#include <map>
#include "LaunchedProcess.h"
#include "Localized.h"
#include "JuceHeader.h"
#include "WindowFocusedTimer.h"

/**
 * @file AppLauncher.h
 * 
 * @brief Launches and tracks new application processes, automatically
 *        switching window focus to the launched application.
 */

class AppLauncher : public WindowFocusedTimer, private Localized
{
public:
    AppLauncher() : WindowFocusedTimer("AppLauncher"),
    Localized("AppLauncher"),
    launchFailureCallback([]()
    {
    }) { }

    virtual ~AppLauncher() { }

    /**
     * @brief  Assigns a function to call if loading an application fails.
     * 
     * @param failureCallback   A function to run if an app fails to launch.
     */
    void setLaunchFailureCallback(const std::function<void() > failureCallback)
    {
        launchFailureCallback = failureCallback;
    }

    /**
     * @brief  Launches a new application, or focuses its window if the 
     *         application is already running
     * 
     * @param command   The shell command that starts the application.
     */
    void startOrFocusApp(const juce::String& command);

    /**
     * @brief  Checks a string to see if it is a valid shell command.
     *
     * @param command  The command string to test.
     *
     * @return         True if and only if the command string is a valid shell 
     *                 command.
     */
    static bool testCommand(const juce::String& command);

private:
    /**
     * @brief  Starts a new instance of an application process.
     * 
     * @param command   The command used to launch the process.
     * 
     * @return          An object representing the application process.
     */
    LaunchedProcess* startApp(const juce::String& command);

    /**
     * @brief  Tracks application launch success and responds appropriately.
     */
    virtual void timerCallback() override;

    /**
     * Stop process tracking if window focus changes and the timer is suspended.
     */
    virtual void onSuspend() override;

    /* The function to run if application launching fails */
    std::function<void() > launchFailureCallback;

    /* Holds all running processes launched by this object. */
    juce::OwnedArray<LaunchedProcess> runningApps;

    /* Timer interval in milliseconds. */
    static const int timerFrequency = 2000;

    /* Milliseconds to wait before giving up on a launch. */
    static const int timeout = 15000;

    /* Last launch time from Time::getMillisecondCounter() */
    juce::uint32 lastLaunch = 0;

    /* Process to check up on when the timer finishes. */
    LaunchedProcess * timedProcess = nullptr;

    //localized text keys;
    static const constexpr char * could_not_open = "could_not_open";
    static const constexpr char * not_valid_command = "not_valid_command";
};
