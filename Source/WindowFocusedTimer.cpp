#include "WindowFocusedTimer.h"

/**
 * Adds the timer to the list of all timers.
 */
WindowFocusedTimer::WindowFocusedTimer()
{
    const ScopedLock listLock(timerListLock);
    allTimers.add(this);
}

/**
 * Removes the timer from the list of all timers.
 */
WindowFocusedTimer::~WindowFocusedTimer()
{
    const ScopedLock listLock(timerListLock);
    allTimers.removeFirstMatchingValue(this);
}

/**
 * Calling this on an active timer will stop the timer.  Calling it on 
 * a suspended timer will prevent it from resuming.
 */
void WindowFocusedTimer::stopTimer()
{
    Timer::stopTimer();
    suspendedTime = Time();
}

/**
 * Called whenever the window loses focus, suspends all active timers.
 */
void WindowFocusedTimer::windowFocusLost()
{
    const ScopedLock listLock(timerListLock);
    int suspended = 0;
    for (WindowFocusedTimer* timer : allTimers)
    {
        if (timer->isTimerRunning())
        {
            Time endTime = Time::getCurrentTime() + 
                    RelativeTime::milliseconds(timer->getTimerInterval());
            timer->stopTimer();
            timer->suspendedTime = endTime;
            suspended++;
        }
    }
    DBG(String("WindowFocusedTimer: Suspended ") + String(suspended) + String("/") +
            String(allTimers.size()) + String(" timers."));
}

/**
 * Called whenever the window gains focus, resumes all suspended timers.
 */
void WindowFocusedTimer::windowFocusGained()
{
    const ScopedLock listLock(timerListLock);
    int resumed = 0;
    for (WindowFocusedTimer* timer : allTimers)
    {
        if (timer->suspendedTime >= 0)
        {
            if (Time::currentTimeMillis() > timer->suspendedTime)
            {
                DBG("Timer resumed and immediately finishes");
                timer->timerCallback();
            }
            else
            {
                int64 timeLeft = Time::currentTimeMillis() - timer->suspendedTime;
                DBG(String("Timer resumed with ") + String(timeLeft) + " ms");
                timer->startTimer(timeLeft);
            }
            timer->suspendedTime = -1;
            resumed++;
        }
    }
    DBG(String("WindowFocusedTimer: Resumed ") + String(resumed) + String("/") +
            String(allTimers.size()) + String(" timers."));
}

/**
 * Protects the time list from concurrent modification.
 */
CriticalSection WindowFocusedTimer::timerListLock;

/**
 * Tracks all WindowFocusedTimers to ensure each one is notified when 
 * window focus changes.
 */
Array<WindowFocusedTimer*> WindowFocusedTimer::allTimers;

