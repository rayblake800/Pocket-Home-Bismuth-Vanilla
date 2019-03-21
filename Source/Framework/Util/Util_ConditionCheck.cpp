#include "Util_ConditionCheck.h"
#include "Util_Math.h"

/* Default condition checking interval in milliseconds: */
static const constexpr int defaultInterval = 300;

/*
 * Initializes the internal timer and loads the default interval value on
 * construction.
 */
Util::ConditionCheck::ConditionCheck() : timer(*this),
    checkInterval(defaultInterval) { }

/*
 * Starts checking for a condition, unless already checking for another
 * condition.
 */
bool Util::ConditionCheck::startCheck(
        const std::function<bool()> check,
        const std::function<void()> callback,
        const int timeoutPeriod,
        const std::function<void()> onFailure)
{
    // Check and callback functions must be valid:
    jassert(check);
    jassert(callback);
    const juce::ScopedLock updateLock(conditionLock);
    if(!conditionCheck && check && callback)
    {
        conditionCheck = check;
        conditionCallback = callback;
        if(timeoutPeriod < 0)
        {
            timeout = -1;
        }
        else
        {
            timeout = juce::Time::currentTimeMillis() + timeoutPeriod; 
        }
        if(!checkCondition())
        {
            timer.startTimer(checkInterval);
        }
        return true;
    }
    return false;
}

/*
 * Checks if the object is currently waiting for a condition to occur.
 */
bool Util::ConditionCheck::isChecking() const
{
    const juce::ScopedLock updateLock(conditionLock);
    // The connection checking function will be valid if and only if the
    // object is currently checking a condition.
    return (bool) conditionCheck;
}

/*
 * Sets how frequently the object should check for its condition to be met.
 */
void Util::ConditionCheck::setCheckInterval
(const int interval, const float multiplier)
{
    const juce::ScopedLock updateLock(conditionLock);
    checkInterval = interval;
    intervalMultiplier = multiplier;
}

/*
 * Cancels any ongoing condition checking. This takes no action if no condition
 * is being checked.
 */
void Util::ConditionCheck::cancelCheck
(const bool runFailureCallback, const bool runFinalTest)
{
    const juce::ScopedLock updateLock(conditionLock);
    if(conditionCheck)
    {
        if(runFinalTest)
        {
            checkCondition();
        }
        if(runFailureCallback && failureCallback)
        {
            failureCallback();
        }
    }
    clearCheckValues();
}

/*
 * Checks the condition, running the condition callback if the condition is met.
 */
bool Util::ConditionCheck::checkCondition()
{
    if(conditionCheck())
    {
        conditionCallback();
        clearCheckValues();
        return true;
    }
    return false; 
}

/*
 * Removes the conditionCheck test and all callback functions and stops and 
 * resets the timer.
 */
void Util::ConditionCheck::clearCheckValues()
{
    conditionCheck = std::function<bool()>();
    conditionCallback = std::function<void()>();
    failureCallback = std::function<void()>();
    timeout= 0;
    timer.stopChecking();
}

/*
 * Connects the timer to the ConditionCheck object that owns it.
 */
Util::ConditionCheck::CheckTimer::CheckTimer(ConditionCheck& owner) : 
    owner(owner) { }


/*
 * Stops the timer and resets the timer's interval.
 */
void Util::ConditionCheck::CheckTimer::stopChecking()
{
    stopTimer();
    nextInterval = owner.checkInterval;
}

/*
 * Checks the condition, running callbacks or scheduling future checks as
 * necessary.
 */
void Util::ConditionCheck::CheckTimer::timerCallback()
{
    const juce::ScopedLock timerLock(owner.conditionLock);
    jassert(owner.conditionCheck);
    if(!owner.checkCondition())
    {
        if(owner.timeout >= 0 
                && juce::Time::currentTimeMillis() >= owner.timeout)
        {
            if(owner.failureCallback)
            {
                owner.failureCallback();
            }
            owner.clearCheckValues();
        }
        else
        {
            nextInterval *= owner.intervalMultiplier;
            if(owner.timeout >= 0)
            {
                nextInterval = Util::Math::median<int>(nextInterval,
                        owner.timeout - juce::Time::currentTimeMillis(), 1);
            }
            startTimer(nextInterval);
        }
    }
}
