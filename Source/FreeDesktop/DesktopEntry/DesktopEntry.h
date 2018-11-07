#pragma once
#include "JuceHeader.h"

namespace DesktopEntry
{
    class Data;
    class FileError;
    class FormatError;
    class Loader;
    
    typedef juce::uint32 CallbackID;

    enum class UpdateType
    {
        entryAdded,
        entryChanged,
        entryRemoved
    };

    class Thread;
    class Utils;
}
