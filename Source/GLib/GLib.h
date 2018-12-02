#pragma once

/**
 * @file  GLib/GLib.h
 *
 * @brief  Provides a C++ object interface and RAII memory management for GLib's 
 *         C-based object system. 
 */
namespace GLib
{
    /**
     * @brief  A C++ interface and RAII container for GLib GObject data.
     *
     * @see  GLib/Object.h
     */
    class Object;
    
    /**
     * @brief  Manages a GLib GWeakRef, preventing access to GObject data after
     *         it is destroyed.
     *
     * @see  GLib/WeakRef.h
     */
    class WeakRef;

    /**
     * @brief  Manages a GLib event loop.
     *
     * @see  GLib/EventLoop.h
     */
    class EventLoop;

    /**
     * @brief  Runs a GLib event loop on a SharedResource::ThreadResource.
     *
     * @see  GLib/ThreadResource.h
     */
    class ThreadResource;

    /**
     * @brief  Schedules functions to run on a ThreadResource's EventLoop,
     *         selected using its GMainContext.
     */
    class ContextCaller;

    /**
     * @brief  Safely accesses a GLib event loop thread resource.
     *
     * @see  GLib/ThreadHandler.h
     *
     * @tparam GLibThreadType  The GLib::ThreadResource instance subclass 
     *                         the ThreadHandler will access.
     */
    template<class GLibThreadType>
    class ThreadHandler;

    /**
     * @brief  Handles signals emitted by Objects on the GLib event loop. 
     *
     * @see  GLib/SignalHandler.h
     */
    class SignalHandler;

    /**
     * @brief  Manages a GDBusProxy, accessing the interface of a DBus object
     *         created by another application.
     *
     * @see  GLib/DBus/DBusProxy.h
     */
    class DBusProxy;

    /**
     * @brief  Creates a GLib ThreadResource to use exclusively for DBus 
     *         signals.
     *
     * @see  GLib/DBus/DBusThread.h
     */
    class DBusThread;

    /**
     * @brief  Manages GVariant* values, fixed, strongly typed data values with
     *         many possible types. These are used to send and receive data over
     *         DBus
     *
     * @see  GLib/Variant/Variant.h
     */
    class Variant;

    /**
     * @brief  A smart pointer class used to temporarily hold gpointer values,
     *         automatically unreferencing them on destruction.
     */
    template <typename GPointerType, typename Unreferencer> 
    class ScopedGPointer;

}
