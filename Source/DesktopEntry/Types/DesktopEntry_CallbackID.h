#pragma once
/**
 * @file  DesktopEntry_CallbackID.h
 *
 * @brief  Defines the CallbackID type used to identify and cancel pending
 *         callback functions.
 */

#include "JuceHeader.h"

namespace DesktopEntry
{
    typedef juce::uint32 CallbackID;

    static inline bool isNullCallback(const CallbackID callback)
    {
        return callback == 0;
    }
}
