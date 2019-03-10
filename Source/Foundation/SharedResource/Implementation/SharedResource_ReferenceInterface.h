#pragma once
/**
 * @file  SharedResource_ReferenceInterface.h
 *
 * @brief  The interface shared by Reference and Instance objects to control
 *         Reference access.
 */
#include "JuceHeader.h"

namespace SharedResource { class ReferenceInterface; }

class SharedResource::ReferenceInterface
{
public:
    virtual ~ReferenceInterface() { }

protected:
    /* Only the Reference and its Instance may access the lock. */
    friend class Instance;

    /**
     * @brief  Gets the lock used to control this Reference.
     *
     *  This lock prevents the Reference from being destroyed while its resource
     * Instance is accessing it.
     *
     * @return  The reference lock.
     */
    juce::CriticalSection& getLock();

private: 
    juce::CriticalSection lock;
};
