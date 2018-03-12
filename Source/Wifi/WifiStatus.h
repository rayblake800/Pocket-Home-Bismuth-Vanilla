/**
 * @file WifiStatus.h
 * 
 * WifiStatus periodically checks for wifi state changes, notifying Listener
 * objects whenever
 */
#pragma once
#include <exception>
#include "../../JuceLibraryCode/JuceHeader.h"
#include "WifiAccessPoint.h"

class WifiStatus : public Thread {
public:
    virtual ~WifiStatus();

    /**
     * @return the WifiStatus thread, or nullptr if the instance hasn't been
     * set yet.
     */
    static WifiStatus* getInstance();

    /**
     * Sets the WifiStatus thread instance.  This should only ever happen once
     * per program instance, after it is determined which WifiStatus subclass 
     * should be used.
     * 
     * @param wifiStatus
     * @throw ExtraWifiInitException if the WifiStatus instance has already
     * been set.
     */
    static void setInstance(WifiStatus* wifiStatus);

    /**
     * @return the list of all Wifi access points close enough to detect.
     */
    virtual Array<WifiAccessPoint> nearbyAccessPoints() = 0;
    
    /**
     * @return the currently connected access point, or WifiAccessPoint::null()
     * if no access point is connected. 
     */
    virtual WifiAccessPoint connectedAccessPoint() const = 0;
      
    /**
     * @return true iff wifi is currently turned on.
     */
    virtual bool isEnabled() const = 0;
    
    /**
     * @return true iff the system is currently connected to a wifi access
     * point.
     */
    virtual bool isConnected() const = 0;
    
    /**
     * Turns on the wifi radio.
     */
    virtual void enableWifi() = 0;
    
    /**
     * Turns off the wifi radio.
     */
    virtual void disableWifi() = 0;

    /**
     * Attempts to connect to a wifi access point.
     * @param ap defines the wifi access point to connect to.
     * @param psk wifi access point key, if ap is a secure connection.
     */    
    virtual void setConnectedAccessPoint(const WifiAccessPoint& ap,
            String psk = String()) = 0;  

    /**
     * If connected to a wifi access point, this will close that connection.
     */
    virtual void disconnect() = 0;

    /**
     *registered Listener objects receive updates whenever wifi status changes.
     */
    class Listener {
    public:
        Listener();
        virtual ~Listener();
        virtual void handleWifiEnabled() { }
        virtual void handleWifiDisabled() { }
        virtual void handleWifiConnected() { }
        virtual void handleWifiDisconnected() { }
        virtual void handleWifiFailedConnect() { }
        virtual void handleWifiBusy() { }
    };
    
    /**
     * Add a listener to the list of objects receiving updates on wifi state
     * changes.
     * @param listener
     */
    void addListener(Listener* listener);
    
    
    /**
     * Remove a listener from the list of objects receiving updates on wifi state
     * changes.
     * @param listener
     */
    void removeListener(Listener* listener);
    
    
    /**
     * Remove all listeners from the list of objects receiving updates on wifi 
     * state changes.
     */
    void clearListeners();


    struct ExtraWifiInitException : public std::exception {

        const char * what() const noexcept {
            return "WifiStatus thread object already exists.";
        }
    };

protected:
    /**
     * @throw ExtraWifiInitException if the WifiStatus instance already exists
     */
    WifiStatus();

    /**
     * call listener->handleWifiEnabled() for each registered listener object.
     */
    void notifyListenersWifiEnabled();

    /**
     * call listener->handleWifiDisabled() for each registered listener object.
     */
    void notifyListenersWifiDisabled();

    /**
     * call listener->handleWifiConnected() for each registered listener object.
     */
    void notifyListenersWifiConnected();

    /**
     * call listener->handleWifiDisconnected() for each registered listener 
     * object.
     */
    void notifyListenersWifiDisconnected();

    /**
     * call listener->handleWifiFailedConnect() for each registered listener 
     * object.
     */
    void notifyListenersWifiFailedConnect();

    /**
     * call listener->handleWifiBusy() for each registered listener object.
     */
    void notifyListenersWifiBusy();
private:
    Array<WifiStatus::Listener*> listeners;
    static ScopedPointer<WifiStatus> instance;
};


