/**
 * @file WifiStatus.h
 * 
 * WifiStatus periodically checks for wifi state changes, notifying Listener
 * objects whenever
 */
#pragma once
#include <exception>
#include "../../JuceLibraryCode/JuceHeader.h"
#include "../WindowFocus.h"
#include "WifiAccessPoint.h"

class PocketHomeWindow;
class WifiStatus : protected Thread, private WindowFocus::Listener {
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
     * If the wifi status thread is not running, it will be started, as long
     * as NetworkManager and wlan0 are functioning normally and wifi is not
     * disabled.
     */
    void startWifiThread();

    /**
     * If the wifi status thread is running, this will stop it.
     */
    void stopWifiThread();
    
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
     * All wifi state changes that should be shared with listeners
     */
    enum WifiEvent{
        wifiEnabled,
        wifiDisabled,
        wifiConnected,
        wifiDisconnected,
        wifiConnectionFailed,
        wifiBusy
    };
    
#if JUCE_DEBUG
    /**
     * @param event
     * @return a string representation of event for debugging purposes
     */
    String wifiEventString(WifiEvent event);
#endif
    /**
     *registered Listener objects receive updates whenever wifi status changes.
     */
    class Listener {
    public:
        Listener();
        virtual ~Listener();
        virtual void handleWifiEvent(WifiEvent event) { }
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
     * call listener->handleWifiEvent(event) for each registered listener 
     * object.
     */
    void notifyListeners(WifiEvent event);

private:
    
    /**
     * Shut down the WifiStatus thread when the application window loses
     * focus.
     */
    void windowFocusLost() override;

    /**
     * If wifi is enabled, restart the WifiStatus thread.
     */
    void windowFocusGained() override;

    CriticalSection listenerLock;
    Array<WifiStatus::Listener*> listeners;
    static ScopedPointer<WifiStatus> instance;
};


