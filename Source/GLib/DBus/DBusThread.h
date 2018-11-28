#pragma once
#include "GLib/DBus/DBusProxy.h"
#include "GLib/ThreadResource.h"

/**
 * @file GLib/DBus/DBusThread.h
 *
 * @brief  Runs the GLib event loop thread used for DBus communication.
 */
class GLib::DBusThread : public ThreadResource
{
public:
    /* SharedResource unique instance key */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Create the DBusThread as a GLib::ThreadResource using its own
     *         context.
     */
    DBusThread();

    virtual ~DBusThread() { }
};
