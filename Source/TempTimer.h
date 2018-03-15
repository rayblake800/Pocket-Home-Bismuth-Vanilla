/**
 * @file TempTimer.h
 * 
 * TempTimer starts a temporarily allocated timer that deletes itself after
 * it runs once.
 */
#pragma once
#include "JuceHeader.h"

class TempTimer : public ReferenceCountedObject, private Timer
{
public:
    
    /**
     * Start a new timer.
     * 
     * @param runTimeMS time in milliseconds before the timer executes.
     * @param callback function to run when the timer executes.
     */
    static void initTimer(int runTimeMS, std::function<void() > callback);
    
    virtual ~TempTimer();
private:
    typedef ReferenceCountedObjectPtr<TempTimer> Ptr;
    
    TempTimer(int runTimeMS, std::function<void() > callback);
    
    void timerCallback() override;
    
    Ptr selfRef = nullptr;
    std::function<void() > callback;
#if JUCE_DEBUG
    uint32 timerID;
#endif
}; 