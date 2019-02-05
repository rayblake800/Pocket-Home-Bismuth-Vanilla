#pragma once
/**
 * @file Wifi_LibNM_DeviceWifi.h
 *
 * @brief A C++ interface for LibNM NMDeviceWifi objects.
 */

#include "GLib_Borrowed_Object.h"
#include "GLib_Borrowed_ObjectLender.h"
#include "GLib_Signal_Handler.h"
#include <nm-device-wifi.h>

namespace Wifi 
{ 
    namespace LibNM
    {
        class DeviceWifi; 
        class Client;
        class AccessPoint;
        class Connection;
        class ActiveConnection;
        namespace Thread { class Module; }
        namespace Signal { class DeviceHandler; }
        class ObjectLenderInterface;
    } 
}

namespace GLib { namespace Signal { 
    template <class SourceType> class CallbackData; } }

/**
 * @brief  Inheriting this interface ensures that only the LibNM::Thread::Module
 *         may set the DeviceWifi's ObjectLender pointers.
 */
class Wifi::LibNM::ObjectLenderInterface
{
protected:
    /* More convenient names for ObjectLenders managed by the wifi device: */
    typedef GLib::Borrowed::ObjectLender<ActiveConnection> ConnectionLender;
    typedef GLib::Borrowed::ObjectLender<AccessPoint> APLender;

    /**
     * @brief  Gets a pointer to the container holding ActiveConnection objects
     *         managed through the wifi device.
     *
     * @return   The connection lender, or nullptr if the wifi device does not
     *           manage any connections.
     */
    ConnectionLender* getConnectionLender() const;

    /**
     * @brief  Gets a pointer to the container holding AccessPoint objects
     *         managed through the wifi device.
     *
     * @return  The access point lender, or nullptr if the wifi device does not
     *          manage any access points.
     */
    APLender* getAPLender() const;

private:
    friend class Thread::Module;
    friend class Signal::DeviceHandler;

    /**
     * @brief  Allows the thread module to connect object lenders to the active
     *         wifi device object.
     *
     * @param connectionLender  The address of the connection lender the wifi
     *                          wifi device should use.
     *
     * @param apLender          The address of the access point lender the wifi
     *                          device should use.
     */
    void setLenders(ConnectionLender* connectionLender, APLender* apLender);

    ConnectionLender* connectionLender = nullptr;
    APLender* apLender = nullptr;
};

/**
 *  @brief  Represents a system Wifi device recognized by NetworkManager. 
 *
 *  DeviceWifi provides information about the Wifi device's state, loads 
 * AccessPoint objects representing access points visible to the device, and 
 * provides limited methods for controlling the Wifi device.
 *
 *  DeviceWifi provides methods to close any connection active on the device,
 * and to make the device re-scan visible access points. Enabling or disabling
 * the device, and creating new connections using the device is handled by the
 * LibNM::Client class instead.
 *
 *  DeviceWifi also provides a Listener object interface that may be used to
 * track NMDeviceWifi signals. Listeners are notified when the active connection
 * changes, access points are discovered or lost, or the device's state changes. 
 */
class Wifi::LibNM::DeviceWifi : public GLib::Borrowed::Object,
        public ObjectLenderInterface
{
public:
    /**
     * @brief  Creates a null DeviceWifi.
     */
    DeviceWifi();

    virtual ~DeviceWifi() { }
    
    /**
     * @brief  Gets the current state of the wifi network device.
     * 
     * @return  The current device state, or NM_DEVICE_STATE_UNKNOWN if this
     *          object is null.
     */
    NMDeviceState getState() const;
    
    /**
     * @brief  Gets the reason for the current device state.
     * 
     * @return  An explanation for the value returned by getState(), or 
     *          NM_DEVICE_STATE_REASON_UNKNOWN if this object is null.
     */
    NMDeviceStateReason getStateReason() const;
    
    /**
     * @brief  Checks if NetworkManager manages this device.
     *
     * @return  Whether this device is managed by NetworkManager.
     */
    bool isManaged() const;
    
    /**
     * @brief  Gets the interface name of this wifi device.
     * 
     * @return  The interface name, or the empty string if this object is null.
     */
    const char* getInterface() const;

    /**
     * @brief  Gets the DBus path to this wifi device's remote object.
     *
     * @return  The device's NetworkManager DBus path.
     */
    const char* getPath() const;
    
    /**
     * @brief  Disconnects any connection that is active on this wifi device. 
     *         
     * If there is no active connection, or the object is null, nothing will 
     * happen.
     */
    void disconnect() const;
    
    /**
     * @brief  Gets the current active connection running on this device.
     * 
     * @return  The active connection object, or a null connection object if
     *          this object is not connected or is null.
     */
    ActiveConnection getActiveConnection() const;
    
    /**
     * @brief  Gets an access point object using the access point's path.
     * 
     * @param path  A valid wifi access point DBus path.
     *
     * @return      The matching access point object, or a null access point 
     *              if the access point was not found or this object is null.
     */
    AccessPoint getAccessPoint(const char* path) const;
    
    /**
     * @brief  Gets the active connection's access point.
     *
     * @return  The active access point object, or a null access point object
     *          if this object is disconnected or null.
     */
    AccessPoint getActiveAccessPoint() const;
    
    /**
     * @brief  Gets all access points visible to this device.
     * 
     * @return  An array containing one access point object for each nearby
     *          wifi access point visible to the device.
     */
    juce::Array<AccessPoint> getAccessPoints() const;
    
    /**
     * @brief  Sends a request to the wifi device asking it to scan visible 
     *         access points.
     */
    void requestScan() const;

private:
    /**
     * @brief  Gets this object's GLib object pointer cast as NMDeviceWifi* 
     *         data.
     *
     * @return  The object's LibNM Wifi device object data pointer.
     */
    NMDeviceWifi* getWifiDevicePtr() const;

    /**
     * @brief  Gets this object's GLib object pointer cast as NMDevice* data.
     *
     * @return  The object's LibNM device object data pointer.
     */
    NMDevice* getDevicePtr() const;
};
