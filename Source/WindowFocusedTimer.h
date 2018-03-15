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
 */
#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"

class PocketHomeWindow;
class WindowFocusedTimer : public Timer, private WindowFocus::Listener {
public:
    /**
     * @param name identifies the timer for debug purposes
     */
    WindowFocusedTimer(String name);

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
     * Suspends the timer when the window loses focus.
     */
    void windowFocusLost() override;

    /**
     * Resumes the timer when the window gains focus.
     */
    void windowFocusGained() override;

    /**
     * Next scheduled runtime for a suspended timer, relative to 
     * Time::getMillisecondCounter. If set to zero, this indicates that 
     * the timer is not suspended.
     */
    uint32 suspendedEndTime = 0;
    
    String name;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WindowFocusedTimer)
};
