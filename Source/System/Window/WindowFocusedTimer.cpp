#include "WindowFocusedTimer.h"

/**
 * Calling this on an active timer will stop the timer.  Calling it on 
 * a suspended timer will prevent it from resuming.
 */
void WindowFocusedTimer::stopTimer()
{
    using namespace juce;
    Timer::stopTimer();
    suspendedEndTime = 0;
}

/**
 * Starts the timer and sets the length of interval required. If the
 * window is currently unfocused, the timer will immediately be
 * suspended.
 */
void WindowFocusedTimer::startTimer(int intervalInMilliseconds)
{
    using namespace juce;
    if(WindowFocus::isFocused())
    {
        Timer::startTimer(intervalInMilliseconds);
    }
    else
    {   
        suspendedEndTime = Time::getMillisecondCounter() 
                + intervalInMilliseconds;
        onSuspend();
    }
}

/**
 * Suspends the timer when the window loses focus.
 */
void WindowFocusedTimer::windowFocusLost()
{
    using namespace juce;
    if (isTimerRunning())
    {
        uint32 endTime = Time::getMillisecondCounter() + getTimerInterval();
        stopTimer();
        suspendedEndTime = endTime;
        onSuspend();
    }
}

/**
 * Resumes the timer when the window gains focus.
 */
void WindowFocusedTimer::windowFocusGained()
{
    using namespace juce;
    if (suspendedEndTime > 0)
    {
        uint32 now = Time::getMillisecondCounter();
        if (now > suspendedEndTime)
        {
            DBG("WindowFocusedTimer::" << __func__ << ": Timer " << name
                    << " resumed and immediately finishes");
            timerCallback();
        }
        else
        {
            uint32 timeLeft = suspendedEndTime - now;
            DBG("WindowFocusedTimer::" << __func__ << ": Timer " << name
                    << " resumed at " << String(timeLeft) << " milliseconds");
            startTimer(timeLeft);
        }
        suspendedEndTime = 0;
    }
}


