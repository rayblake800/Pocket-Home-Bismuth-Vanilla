#pragma once
/**
 * @file  ScopeTimer.h
 *
 * @brief  Measures execution time of some scope within this program.
 */

#include "JuceHeader.h"

class ScopeTimer
{
public:
    /**
     * @brief  Starts the timer on construction.
     *
     * @param scopeName  A name identifying the portion of the program the timer
     *                   is tracking.
     */
    ScopeTimer(const juce::String scopeName);

    /**
     * @brief  Saves the total amount of time this object existed before it is
     *         destroyed.
     */
    virtual ~ScopeTimer();

private:
    /* The timer name saved on construction: */
    const juce::String scopeName;

    /* The ID of the thread where this timer was created: */
    const juce::Thread::ThreadID threadID;
};
