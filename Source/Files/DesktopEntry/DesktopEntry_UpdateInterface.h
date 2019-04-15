#pragma once
/**
 * @file  DesktopEntry_UpdateInterface
 *
 * @brief  Defines the interface used by the DesktopEntry::LoadingThread to
 *         send updates to DesktopEntry::UpdateListener objects.
 */

#include "JuceHeader.h"

namespace DesktopEntry { class UpdateInterface; }

/**
 * @brief  An abstract interface implemented by UpdateListener objects and used
 *         by the DesktopEntry::LoadingThread to interact with all update
 *         listeners.
 */
class DesktopEntry::UpdateInterface
{
public:
    UpdateInterface() { }

    virtual ~UpdateInterface() { }

    /**
     * @brief  Signals that new desktop entry files were found.
     *
     * @param entryFileIDs  The desktop file IDs of the newly discovered entry
     *                      files.
     */
    virtual void entriesAdded(const juce::StringArray entryFileIDs) = 0;

    /**
     * @brief  Signals that existing desktop entry files were removed.
     *
     * @param entryFileIDs  The desktop file IDs of the removed entry files.
     */
    virtual void entriesRemoved(const juce::StringArray entryFileIDs) = 0;

    /**
     * @brief  Signals that existing desktop entry files were changed.
     *
     * @param entryFileIDs  The desktop file IDs of the updated entry files.
     */
    virtual void entriesUpdated(const juce::StringArray entryFileIDs) = 0;
};
