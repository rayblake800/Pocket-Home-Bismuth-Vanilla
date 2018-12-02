#pragma once

/**
 * @file  LibNM/LibNM.h
 *
 * @brief  Provides a C++-style object interface and RAII memory management for
 *         the NetworkManager GLib library.
 *
 *  The LibNM module handles all direct interaction with the libnm-glib library.
 * Using the GLib module, it provides convenient C++ wrapper objects for all
 * relevant LibNM objects. These objects automatically manage reference counts 
 * and other memory management tasks, and provide methods used to call relevant
 * library functions.
 *
 *  This module supports any version of libnm-glib from 0.9.10 onwards. Not all
 * supported libnm-glib versions are threadsafe, so all interaction with
 * LibNM::Object and its subclasses must occur within a single 
 * GLib::ThreadResource using the global default GMainContext. The NMThread 
 * class creates this thread as a SharedResource object, and NMThreadHandler
 * objects provide threadsafe access to this thread.
 *
 *  Saved connection data cannot be reliably accessed in older versions of
 * libnm-glib within multithreaded programs, even if all interaction with libnm
 * is confined to a single thread. To circumvent this, saved connection data
 * is read directly from NetworkManager over DBus, using GLib::DBusProxy
 * subclasses. These classes should be fully threadsafe.
 */
namespace LibNM
{
    /**
     * @brief  The thread running a GMainLoop with the global default context,
     *         where all interaction with LibNM::Object subclasses should take
     *         place.
     *
     * @see  LibNM/NMThread.h
     */
    class NMThread;

    /**
     * @brief  Provides threadsafe access to the NMThread.
     *
     * @see  LibNM/NMThreadHandler.h
     */
    class NMThreadHandler;

    /* NMObjects: */
    /**
     * @brief  The basis for all classes managing LibNM's NMObject* types. This
     *         interface provides access to the object's DBus path.
     *
     * @see  LibNM/NMObjects/Object.h
     */
    class Object;

    /**
     * @brief  The NetworkManager client object, used to track and manage the
     *         current active network connection, and to initialize network
     *         device objects.
     *
     * @see  LibNM/NMObjects/Client.h
     */
    class Client;

    /**
     * @brief  Controls and reads data from a system wifi device managed by
     *         NetworkManager.
     *
     * @see  LibNM/NMObjects/DeviceWifi.h
     */
    class DeviceWifi;

    /**
     * @brief  Represents a single wifi access point visible to the active wifi
     *         device.
     *
     * @see  LibNM/NMObjects/AccessPoint.h
     */
    class AccessPoint;

    /**
     * @brief  Represents a single network connection known to NetworkManager.
     *
     * @see  LibNM/NMObjects/Connection.h
     */
    class Connection;

    /**
     * @brief  Represents an active network connection managed by the 
     *         NetworkManager.
     *
     * @see  LibNM/NMObjects/ActiveConnection.h
     */
    class ActiveConnection;
    
    /* DBus: */
    /**
     * @brief  Loads the list of saved network connections over DBus.
     *
     * @see  LibNM/DBus/SavedConnectionLoader.h
     */
    class SavedConnectionLoader;

    /**
     * @brief  Reads and manages a single saved network connection through DBus.
     *
     * @see  LibNM/DBus/SavedConnection.h
     */
    class SavedConnection;
}
