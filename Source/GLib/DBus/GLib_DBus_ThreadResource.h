#ifndef DBUS_THREAD_IMPLEMENTATION
  #error File included directly outside of DBusProxy implementation.
#endif
#pragma once
/**
 * @file GLib_DBus_Thread.h
 *
 * @brief  Runs the GLib event loop thread used for DBus communication.
 */

#include "GLib_DBus_Proxy.h"
#include "GLib_SharedThread.h"
#include "SharedResource_Resource.h"

namespace GLib { namespace DBus { class ThreadResource; } }

class GLib::DBus::ThreadResource : public SharedThread, 
        public SharedResource::Resource
{
public:
    /* SharedResource unique instance key */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Create the ThreadResource as a GLib::SharedThread resource using 
     *         its own context.
     */
    ThreadResource();

    virtual ~ThreadResource() { }

private:
    /**
     * @brief  Gets the thread resource's SharedResource instance key.
     *
     * @return  The resourceKey value.
     */
    virtual const juce::Identifier& getThreadResourceKey() const override;

    /**
     * @brief  Gets the number of references connected to this ThreadResource.
     *
     * @return   The ThreadResource object's reference count.
     */
    virtual int getThreadReferenceCount() const override;

    JUCE_LEAK_DETECTOR(GLib::DBus::ThreadResource);
};
