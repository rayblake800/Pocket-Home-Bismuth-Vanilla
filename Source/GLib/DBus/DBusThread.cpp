#include "GLib/DBus/DBusThread.h"

const juce::Identifier GLib::DBusThread::resourceKey = "GLib::DBusThread";

/*
 * Create the DBusThread as a GLib::ThreadResource using its own context.
 */
GLib::DBusThread::DBusThread() : 
ThreadResource(resourceKey, SharedContextPtr(g_main_context_new())) { }
