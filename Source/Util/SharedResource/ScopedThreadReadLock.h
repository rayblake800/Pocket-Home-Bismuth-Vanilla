#pragma once
#include "ThreadLock.h"

/**
 * @file ScopedThreadReadLock
 *
 * @brief RAII ThreadLock control that takes a read-lock from a ThreadLock on
 *        construction, and releases it on destruction.
 */
class ScopedThreadReadLock
{
    private:
        ThreadLock& threadLock;
    public:
        /**
         * @param threadLock  A ThreadLock that will be locked for reading.
         */
        ScopedThreadReadLock(ThreadLock& threadLock);

        /**
         * Releases the read-lock claimed on construction.
         */
        virtual ~ScopedThreadReadLock();
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScopedThreadReadLock)
};
