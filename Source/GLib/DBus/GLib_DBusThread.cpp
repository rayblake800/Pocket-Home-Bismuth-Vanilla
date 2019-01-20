#define DBUS_THREAD_IMPLEMENTATION
#include "GLib_DBusThread.h"

const juce::Identifier GLib::DBusThread::resourceKey = "GLib_DBusThread";

/*
 * Create the DBusThread as a GLib::ThreadResource using its own context.
 */
GLib::DBusThread::DBusThread() : 
ThreadResource(resourceKey, SharedContextPtr(g_main_context_new())) { }
