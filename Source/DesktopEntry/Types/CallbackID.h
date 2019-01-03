#pragma once
#include "JuceHeader.h"

namespace DesktopEntry
{
    typedef juce::uint32 CallbackID;

    static inline bool isNullCallback(const CallbackID callback)
    {
        return callback == 0;
    }
}
