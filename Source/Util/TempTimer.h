/**
 * @file TempTimer.h
 * 
 * TempTimer starts a temporarily allocated timer that deletes itself after
 * it runs once.  Once started it cannot be canceled, so avoid using this to
 * handle actions for objects that may be unexpectedly destroyed.
 */
#pragma once
#include "JuceHeader.h"

class TempTimer : public ReferenceCountedObject, private Timer
{
public:

    /**
     * Start a new timer.
     * 
     * @param runTimeMS  The time in milliseconds before the timer executes.
     *
     * @param callback   A function to run when the timer executes.
     */
    static void initTimer(int runTimeMS, std::function<void() > callback);

    virtual ~TempTimer() { }

private:
    /**
     * This should only be called by initTimer().
     * 
     * @param runTimeMS
     * 
     * @param callback
     */
    TempTimer(int runTimeMS, std::function<void() > callback);


    /**
     * Runs the timer callback function, and asynchronously deletes the timer.
     */
    void timerCallback() override;

    //TempTimer uses a reference counted pointer to itself to ensure it is
    //deleted appropriately.
    ReferenceCountedObjectPtr<TempTimer> selfRef = nullptr;
    //callback function to run when the timer finishes.
    std::function<void() > callback;
}; 