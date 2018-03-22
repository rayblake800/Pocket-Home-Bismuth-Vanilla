/**
 * @file Utils.h
 * 
 * TODO: work as much of this as possible into distinct modules, having a
 * random pile of assorted global functions is less than ideal.
 * also, documentation
 */

#pragma once
#include "JuceHeader.h"

#if JUCE_DEBUG

/**
 * Print debug info about the component tree.
 */
void componentTrace();

/**
 * Measures program execution time by printing how long it existed 
 * (in milliseconds) when it is destroyed.
 */
class ScopedExecTimer
{
public:

    ScopedExecTimer(String name) :
    name(name),
    startTime(Time::getMillisecondCounter()) { }
    
    virtual ~ScopedExecTimer() {
        uint32 execTime = Time::getMillisecondCounter() - startTime;
        DBG(name << ": Executed in " << String(execTime));
    }
private:
    String name;
    uint32 startTime;
};

#endif

/**
 * Requests user confirmation before performing some action.
 * 
 * @param title      The text to print at the top of the confirmation box.
 * 
 * @param message    Detailed information on the action being confirmed.
 * 
 * @param onConfirm  If the user presses the "OK" button, this callback
 *                    function will run.
 */
void confirmAction(String title, String message, std::function<void() > onConfirm);

/**
 * Find the median of three values.
 * 
 * @param a
 * 
 * @param b
 * 
 * @param c
 * 
 * @return the median of a, b, and c
 */
template<typename T>
T median(T a, T b, T c)
{
    if (a > b)
    {
        if (b > c)
        {
            return b;
        }
        if (a < c)
        {
            return a;
        }
        return c;
    }
    if (a > c)
    {
        return a;
    }
    if (b < c)
    {
        return b;
    }
    return c;
}

/**
 * Determine if a pointer has a particular type.
 * 
 * @param  checkIfC2 a pointer to an object of type C1
 * @return true iff the object pointed to by checkIfC2 also has type C2.
 */
template<class C1, class C2>
bool isClass(C1* checkIfC2)
{
    C2* testPtr = dynamic_cast<C2*> (checkIfC2);
    return testPtr != nullptr;
}