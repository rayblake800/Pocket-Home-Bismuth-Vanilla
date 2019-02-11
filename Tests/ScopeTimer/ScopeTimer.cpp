#include "ScopeTimer.h"
#include "ScopeTimerRecords.h"


/*
 * Starts the timer on construction.
 */
ScopeTimer::ScopeTimer(const juce::String scopeName) :
scopeName(scopeName), 
threadID(juce::Thread::getCurrentThreadId()) 
{ 
    ScopeTimerRecords::addStartRecord(scopeName, threadID);
}

/*
 * Saves the total amount of time this object existed before it is destroyed.
 */
ScopeTimer::~ScopeTimer()
{
    ScopeTimerRecords::addStopRecord(scopeName, threadID);
}
