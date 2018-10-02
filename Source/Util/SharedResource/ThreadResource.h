#pragma once
#include "SharedResource.h"
#include "JuceHeader.h"

class ThreadResource : public SharedResource, public juce::Thread
{
public:
    ThreadResource(const juce::Identifier& resourceKey);

    virtual ~ThreadResource() { }

protected:
    void takeReadLock();

    void takeWriteLock();

    void releaseLock();

private:
    virtual void runThread() = 0;

    virtual void run() final override;
    
    class ThreadLock : public SharedResource::Handler
    {
    public:
        ThreadLock();

        virtual ~ThreadLock() { }
            
        void takeReadLock();

        void takeWriteLock();

        void releaseLock();
    };

    juce::ScopedPointer<ThreadLock> threadLock;
};
