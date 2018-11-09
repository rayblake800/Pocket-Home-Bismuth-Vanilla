#pragma once
#include "JuceHeader.h"

namespace DesktopEntry
{
    class EntryFile;

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

    class LoadingThread;

    namespace FileUtils {}
}
