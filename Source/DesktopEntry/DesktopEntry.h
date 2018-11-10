#pragma once
#include <map>
#include "JuceHeader.h"

namespace DesktopEntry
{
    class EntryFile;

    class FileError;

    class FormatError;

    class Loader;

    class Listener;
    
    typedef juce::uint32 CallbackID;

    class UpdateInterface;

    enum class UpdateType
    {
        entryAdded,
        entryChanged,
        entryRemoved
    };

    typedef struct
    {
        juce::String desktopFileId;
        UpdateType updateType;
    } EntryUpdate;

    class LoadingThread;

    namespace FileUtils {}
}
