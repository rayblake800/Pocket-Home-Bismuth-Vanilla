#pragma once
#include <functional>
#include <map>
#include "LaunchedApp.h"
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
     * Assigns a function to call if loading an application fails.
     * @param failureCallback will run if an app fails to launch.
     */
    void setLaunchFailureCallback(std::function<void() > failureCallback)
    {
        launchFailureCallback = failureCallback;
    }

    /**
     * Launch a new application, or focus its window if the application is
     * already running
     * 
     * @param appTitle  The application title.
     * 
     * @param command   The shell command that starts the application.
     */
    void startOrFocusApp(juce::String appTitle, juce::String command);
private:

    /**
     * Start a new instance of an application process.
     * 
     * @param command   The command used to launch the process.
     * 
     * @return   An object representing the application process.
     */
    LaunchedApp* startApp(const juce::String& command);

    /**
     * Focus the window of a running application.
     * 
     * @param ProcessInfo  The application's process information.
     */
    void focusApp(LaunchedApp* runningApp);
    

    /**
     * Track application launch success and respond appropriately.
     */
    virtual void timerCallback() override;

    /**
     * Stop process tracking if window focus changes and the timer is suspended.
     */
    virtual void onSuspend() override;

    //function to run if application launching fails
    std::function<void() > launchFailureCallback;

    //holds all running processes launched by this object.
    juce::OwnedArray<LaunchedApp> runningApps;

    //timer interval in milliseconds
    static const int timerFrequency = 2000;

    //milliseconds to wait before giving up on a launch
    static const int timeout = 15000;

    //last launch time from Time::getMillisecondCounter()
    juce::uint32 lastLaunch = 0;

    //Process to check up on when the timer finishes
    juce::ChildProcess * timedProcess = nullptr;

    //localized text keys;
    static const constexpr char * could_not_open = "could_not_open";
    static const constexpr char * not_valid_command = "not_valid_command";
};
