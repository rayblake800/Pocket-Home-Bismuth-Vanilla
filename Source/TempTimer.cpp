#include "TempTimer.h"

void TempTimer::initTimer(int runTimeMS, std::function<void() > callback)
{
    new TempTimer(runTimeMS, callback);
}

TempTimer::~TempTimer()
{
}

TempTimer::TempTimer(int runTimeMS, std::function<void() > callback) :
callback(callback)
{
    selfRef = this;
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
