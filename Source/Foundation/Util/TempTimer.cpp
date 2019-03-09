#include "TempTimer.h"

/**
 * Start a new timer.
 */
void TempTimer::initTimer(int runTimeMS, std::function<void() > callback)
{
    new TempTimer(runTimeMS, callback);
}

/**
 * This should only be called by initTimer().
 */
TempTimer::TempTimer(int runTimeMS, std::function<void() > callback) :
callback(callback)
{
    selfRef = this;
    startTimer(runTimeMS);
}

/**
 * Runs the timer callback function, and asynchronously deletes the timer.
 */
void TempTimer::timerCallback()
{
    using namespace juce;
    stopTimer();
    callback();
    //Can't delete this timer while running its callback, so schedule
    //it to delete asynchronously.
    MessageManager::callAsync([this]()
    {
        selfRef = nullptr;
    });
}
