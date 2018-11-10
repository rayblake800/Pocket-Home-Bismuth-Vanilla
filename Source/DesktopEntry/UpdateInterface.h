#pragma once
#include "DesktopEntry/DesktopEntry.h"

/**
 * @file DesktopEntry/UpdateInterface
 *
 * @brief  Defines the interface used by the DesktopEntry::LoadingThread to send
 *         updates to DesktopEntry::UpdateListener objects.
 */
class DesktopEntry::UpdateInterface
{
protected:
    UpdateInterface() { }

public:
    virtual ~UpdateInterface() { }

    virtual void entryAdded(const juce::String entryFileID) { }

    virtual void entryRemoved(const juce::String entryFileID) { }

    virtual void entryUpdated(const juce::String entryFileID) { }
};
