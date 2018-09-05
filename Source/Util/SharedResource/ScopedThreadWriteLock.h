#pragma once
#include "ThreadLock.h"

/**
 * @file ScopedThreadWriteLock
 *
 * @brief RAII ThreadLock control that takes a thread-lock from a ThreadLock on
 *        construction, and releases it on destruction.
 */
class ScopedThreadWriteLock
{
    private:
        ThreadLock& threadLock;
    public:
        /**
         * @param threadLock  A ThreadLock that will be locked for writing.
         */
        ScopedThreadWriteLock(ThreadLock& threadLock);

        /**
         * Releases the thread-lock claimed on construction.
         */
        virtual ~ScopedThreadWriteLock();
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopedThreadWriteLock)
};
