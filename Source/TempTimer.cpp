#include "TempTimer.h"

void TempTimer::initTimer(int runTimeMS, std::function<void() > callback)
{
    new TempTimer(runTimeMS, callback);
}

TempTimer::~TempTimer()
{
#    if JUCE_DEBUG
    DBG("TempTimer: deleting timer with ID " << String(timerID));
#    endif 
}

TempTimer::TempTimer(int runTimeMS, std::function<void() > callback) :
callback(callback)
{
    selfRef = this;
#    if JUCE_DEBUG
    timerID = Time::getMillisecondCounter();
    DBG("TempTimer: created timer with ID " << String(timerID));
#    endif 
    startTimer(runTimeMS);
}

void TempTimer::timerCallback()
{
    callback();
    stopTimer();
    //Can't delete this timer while running its callback, so schedule
    //it to delete asynchronously.
    MessageManager::callAsync([this]()
    {
        selfRef = nullptr;
    });
}
