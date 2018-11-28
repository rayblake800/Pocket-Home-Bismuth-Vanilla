#include "LibNM/NMThread.h"

/* Unique SharedResource object instance key: */
const juce::Identifier LibNM::NMThread::resourceKey = "LibNM::NMThread";

/*
 * Initializes the NMThread with the default GLib context.
 */
LibNM::NMThread::NMThread() :
GLib::ThreadResource(resourceKey, GLib::SharedContextPtr(nullptr)) { }
