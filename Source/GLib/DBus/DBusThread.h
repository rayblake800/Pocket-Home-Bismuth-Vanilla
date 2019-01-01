#ifndef DBUS_THREAD_IMPLEMENTATION
  #error File included outside of DBusProxy implementation.
#endif
#pragma once
/**
 * @file GLib/DBus/DBusThread.h
 *
 * @brief  Runs the GLib event loop thread used for DBus communication.
 */

#include "GLib/DBus/DBusProxy.h"
#include "GLib/Thread/ThreadResource.h"

namespace GLib { class DBusThread; }

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
