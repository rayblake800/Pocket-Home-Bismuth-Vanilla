/**
 * @file AppLauncher.h
 * 
 * AppLauncher launches and tracks new application processes. 
 */
#pragma once
#include <functional>
#include <map>
#include "../JuceLibraryCode/JuceHeader.h"

/**
 * TODO:
 *  -Test to see if it's really necessary to reload xmodmap every time an 
 *   application launches
 *  -Instead of holding a single instance of a custom Timer subclass, this
 *   class should just inherit from Timer
 */

class AppLauncher{
public:
    AppLauncher();
    virtual ~AppLauncher();
    
    /**
     * Assigns a function to call if loading an application fails.
     * @param failureCallback will run if an app fails to launch.
     */
    void setLaunchFailureCallback(std::function<void()> failureCallback);
    
    /**
     * Launch a new application, or focus its window if the application is
     * already running
     * @param appTitle the application title
     * @param command the shell command that starts the application
     */
    void startOrFocusApp(String appTitle, String command);
    
    /**
     * If there is a timer tracking application launching, cancel it.
     */
    void stopTimer();
private:
    //Stores information on launched processes
    struct ProcessInfo{
        ProcessInfo(String title,String command);
        //Application title
        String title;
        //Application launch command
        String command;
        bool operator==(const ProcessInfo& rhs) const;
        bool operator<(const ProcessInfo& rhs) const;
    };
    
    /**
     * Start a new instance of an application process
     * @param processInfo defines the new application title and
     * launch command.
     */
    void startApp(ProcessInfo processInfo);
    
    /**
     * Focus the window of a running app
     * @param windowId the app window's id
     */
    void focusApp(const String& windowId);
    
    /**
     * Attempt to find an open window of a launched application
     * @param processInfo running application process info
     * @return the window ID, or the empty string if none was found
     */
    String getWindowId(ProcessInfo processInfo);
    
    //function to run if application launching fails
    std::function<void()> launchFailureCallback;
    
    //holds all running processes launched by this object.
    OwnedArray<ChildProcess> runningApps;
    
    //store child process launch information
    std::map<ProcessInfo,ChildProcess*> processMap;
    
    //Track application launch success and respond appropriately.
    class AppLaunchTimer : public Timer {
    public:
        AppLaunchTimer(AppLauncher* launcher);
        virtual ~AppLaunchTimer();
        /**
        * Focus successfully launched apps,
        * stop waiting for failed ones.
         * */
        virtual void timerCallback() override;
        void setTrackedProcess(ChildProcess * trackedProcess);
        //does the same thing as the base class method, but it also
        //gets rid of its launcher pointer.
        void stopTimer();
        static const int frequency=2000;
    private:
        AppLauncher* launcher = nullptr;
        ChildProcess * trackedProcess = nullptr;
    };
    AppLaunchTimer launchTimer;
    
};
