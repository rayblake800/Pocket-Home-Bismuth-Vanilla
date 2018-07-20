#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"

/**
 * @file WindowFocusedTimer.h
 * 
 * @brief  A Juce timer that is guaranteed to stop whenever the main application
 *         window loses focus, and resume whenever it regains focus.
 * 
 * This should reduce the amount that this application wastes CPU cycles while
 * it's running in the background.  
 * 
 * Suspended timers save their next scheduled runtime, not the amount of time 
 * left on the timer, so timers that would have ended while suspended will 
 * immediately call their timerCallback when resumed.
 */

class PocketHomeWindow;
class WindowFocusedTimer : public Timer, private WindowFocus::Listener {
public:
    /**
     * @param name  This identifies the timer for debug purposes.
     */
    WindowFocusedTimer(String name) : name(name) { }

    virtual ~WindowFocusedTimer() { }
    
    /**
     * Calling this on an active timer will stop the timer.  Calling it on 
     * a suspended timer will prevent it from resuming.
     */
    void stopTimer();

    /**
     * Starts the timer and sets the length of interval required. If the
     * window is currently unfocused, the timer will immediately be
     * suspended.
     *
     * @param intervalInMilliseconds  Milliseconds to wait before the 
     *                                timer callback function runs.
     */
    void startTimer(int intervalInMilliseconds);
    
private:
    /**
     * Runs whenever the timer is suspended. Override this to change
     * how timers behave when suspended.
     */
    virtual void onSuspend() {}
    
    /**
     * Suspends the timer when the window loses focus.
     */
    void windowFocusLost() override;

    /**
     * Resumes the timer when the window gains focus.
     */
    void windowFocusGained() override;

    /**
     * Next scheduled runtime for a suspended timer, relative to 
     * Time::getMillisecondCounter(). If set to zero, this indicates that 
     * the timer is not suspended.
     */
    uint32 suspendedEndTime = 0;
    
    //Timer name, set to better track the behavior of specific timers while
    //debugging.
    String name;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowFocusedTimer)
};
