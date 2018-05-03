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

/*
 * Some shared resources are too expensive to load more than once, or having
 * multiples creates tricky problems with concurrent resource access that are
 * best avoided.  Singleton design patterns are a possible solution, but that
 * makes it much harder to manage their life cycles with RAII methods. 
 * 
 * Instead of making explicit singleton classes, I'm just going to make each
 * shared resource once, and pass them to any object that needs them.
 * 
 * The ASSERT_SINGULAR macro should be added to the constructor of all of these
 * classes.  In debug builds it will throw an exception and print an error if
 * any of these classes is initialized more than once.  
 */

struct DuplicateInstanceException: public std::exception{
    DuplicateInstanceException(const char* function, int num)
    {
        DBG(function<<": this class was instantiated " << (num+1) 
                << " times, but there should only be one!");
    }         
};
#define ASSERT_SINGULAR static int n = 0; if(n){throw DuplicateInstanceException(__func__,n);} n++
#else
#define ASSERT_SINGULAR
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