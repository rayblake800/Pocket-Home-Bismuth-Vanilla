#pragma once
#include <map>
#include "JuceHeader.h"

/**
 * @file Utils.h
 * 
 * @brief Provides miscellaneous utility and debugging functions.
 */

#if JUCE_DEBUG
/**
 * Print debug info about the component tree.
 */
void componentTrace();


/**
 * Convert a pointer to a unique, fixed ID for debug output. Nullptr will always
 * have ID 0.
 * 
 * @param ptr  any pointer.
 * 
 * @return  an ID number unique to this pointer.
 */
int addressID(const void* ptr);

/**
 * Appends a line of text to the log of events occurring to a specific address.
 * 
 * @param ptr  any pointer.
 * 
 * @param event  Some event that occurred involving that pointer
 * 
 * @param ptr2   Some other pointer involved in the event, if non-null, it's ID
 *               will be appended onto the end of the event log.
 * 
 * @return  a reference to the full event log for that pointer.
 */
const String& addressLog(const void* ptr, String event, const void* ptr2 = nullptr);

#define ADDR_LOG(ptr1,...) addressLog(ptr1, __VA_ARGS__)

/**
 * Prints all logged events for a specific memory address
 *  
 * @param addressID  The address being debugged.
 */
void printLog(int addressID);

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

    virtual ~ScopedExecTimer()
    {
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
void confirmAction
(String title, String message, std::function<void() > onConfirm);

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
 * @param  checkIfC2  A pointer to an object of type C1.
 * 
 * @return true iff the object pointed to by checkIfC2 also has type C2.
 */
template<class C1, class C2>
bool isClass(C1* checkIfC2)
{
    C2* testPtr = dynamic_cast<C2*> (checkIfC2);
    return testPtr != nullptr;
}
 
/**
 * Gets the size of the active display.
 * 
 * @return the size of the display, measured in pixels.
 */
Rectangle<int> getDisplaySize();

/**
 * Gets the size of the single application window.
 * 
 * @return the bounds of the active window, measured in pixels.
 */
Rectangle<int> getWindowBounds();