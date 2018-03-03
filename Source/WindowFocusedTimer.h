/**
 * @file WindowFocusedTimer.h
 * 
 * WindowFocusedTimer is a Juce timer that is guaranteed to stop whenever 
 * PocketHomeWindow loses focus, and resume whenever it regains focus.
 * This should reduce the amount that this application wastes CPU cycles while
 * it's running in the background.  Suspended timers save their next scheduled
 * runtime, not the amount of time left on the timer, so timers that would
 * have ended while suspended will immediately call their timerCallback when
 * resumed.
 * 
 * The PocketHomeWindow class is responsible for passing window state changes
 * on to this class by calling windowFocusGained() and windowFocusLost()
 */
#include "../JuceLibraryCode/JuceHeader.h"
#pragma once

class WindowFocusedTimer : public Timer {
    friend class PocketHomeWindow;
public:
    /**
     * Adds the timer to the list of all timers.
     */
    WindowFocusedTimer();

    /**
     * Removes the timer from the list of all timers.
     */
    virtual ~WindowFocusedTimer();
    
    /**
     * Calling this on an active timer will stop the timer.  Calling it on 
     * a suspended timer will prevent it from resuming.
     */
    void stopTimer();
private:
    /**
     * Runs whenever the timer is suspended, override this to change
     * how timers behave when suspended.
     */
    virtual void onSuspend() {}
    
    /**
     * Called whenever the window loses focus, suspends all active timers.
     */
    static void windowFocusLost();

    /**
     * Called whenever the window gains focus, resumes all suspended timers.
     */
    static void windowFocusGained();

    /**
     * Protects the time list from concurrent modification.
     */
    static CriticalSection timerListLock;

    /**
     * Tracks all WindowFocusedTimers to ensure each one is notified when 
     * window focus changes.
     */
    static Array<WindowFocusedTimer*> allTimers;

    /**
     * Next scheduled runtime for a suspended timer, relative to 
     * Time::getMillisecondCounter. If set to zero, this indicates that 
     * the timer is not suspended.
     */
    uint32 suspendedEndTime = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowFocusedTimer)
};
