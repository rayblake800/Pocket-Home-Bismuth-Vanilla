#pragma once
#include "JuceHeader.h"

/**
 * @file  DesktopEntry/DesktopEntry.h
 *
 * @brief  Defines the public interface for the DesktopEntry module, which 
 *         handles desktop entry files.
 *
 * Desktop entry files are a standardized form of shared application shortcut
 * used on Linux desktops. The DesktopEntry module loads these files into data
 * objects, and provides an interface for reading, updating, and changing
 * desktop entry data.
 * 
 * @see  https://specifications.freedesktop.org/desktop-entry-spec/
 *       desktop-entry-spec-latest.html
 */
namespace DesktopEntry
{
    /**
     * @brief  Represents a single desktop entry file's data.
     *
     * @see  DesktopEntry/EntryFile.h
     */
    class EntryFile;

    /**
     * @brief  An exception type thrown if reading or writing desktop entry
     *         files fails.
     *
     * @see  DesktopEntry/FileError.h
     */
    class FileError;

    /**
     * @brief  An exception type thrown if improperly formatted data is found in
     *         a desktop entry file, or when attempting to add improperly
     *         formatted data to an EntryFile object.
     *
     * @see  DesktopEntry/FormatError.h
     */
    class FormatError;

    /**
     * @brief  A resource handler used to access cached desktop entries, and
     *         to call for desktop entry files to be loaded or updated.
     *
     * @see  DesktopEntry/Loader.h
     */
    class Loader;

    /**
     * @brief  A listener class that is notified whenever new desktop entries
     *         are loaded, or existing entries are updated or removed.
     *
     * @see  DesktopEntry/UpdateListener.h
     */
    class UpdateListener;

    /**
     * @brief  Identifies callback functions added to the Loader, so that they
     *         can be cancelled if necessary.
     */
    typedef juce::uint32 CallbackID;
}
