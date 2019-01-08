#ifndef DBUS_THREAD_IMPLEMENTATION
  #error File included outside of DBusProxy implementation.
#endif
#pragma once
/**
 * @file GLib_DBusThread.h
 *
 * @brief  Runs the GLib event loop thread used for DBus communication.
 */

#include "GLib_DBusProxy.h"
#include "GLib_ThreadResource.h"

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
