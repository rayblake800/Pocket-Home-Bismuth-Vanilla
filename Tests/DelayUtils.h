#pragma once
#include "JuceHeader.h"

/**
 * @file  DelayUtils.h
 *
 * @brief  Handles tests that require waiting until some particular event is
 *         true.
 */
class DelayUtils
{
public:
    /**
     * @brief  Runs the juce dispatch loop until an expected condition is true. 
     *
     * @param waitCondition     Tests if the expected condition is true yet.
     *
     * @param testFrequencyMS   Milliseconds to wait between tests of the 
     *                          expected condition.
     *
     * @param timeoutMS         Milliseconds to wait before assuming the
     *                          condition will never be met.
     *
     * @return                  Whether waitCondition returned true before the
     *                          timeout period ended.
     */
    static bool idleUntil(const std::function<bool()> waitCondition,
            int testFrequencyMS, int timeoutMS);
};

