#pragma once
#include "LibNM/NMObjects/Object.h"
#include "LibNM/NMObjects/Connection.h"
#include "GLib/SignalHandler.h"
#include <nm-access-point.h>

/**
 * @file LibNM/AccessPoint.h
 * 
 * @brief A RAII container and C++ interface for NMAccessPoint objects.
 * 
 * AccessPoint holds a LibNM object representing a wifi access point.  It
 * provides access to relevant access point data, and checks Connection 
 * objects for compatibility with this access point.
 * 
 * It also provides the AccessPoint::Listener class as an interface for 
 * receiving access point signals. After they have been added, listeners will
 * receive updates if signal strength changes or the access point is removed. 
 */

class LibNM::AccessPoint : public LibNM::Object
{
public:
    /**
     * Creates a AccessPoint sharing a GObject with an existing
     * AccessPoint.
     * 
     * @toCopy  An existing connection object.
     */
    AccessPoint(const AccessPoint& toCopy);

    /**
     * Creates a AccessPoint to contain a NMAccessPoint object.
     * 
     * @toAssign  A valid NMAccessPoint for this AccessPoint to hold.
     */
    AccessPoint(NMAccessPoint* toAssign);
    
    /**
     * Creates a null AccessPoint.
     */
    AccessPoint();
    
    /**
     * Gets the access point SSID as a byte array from the access point.  This 
     * may contain unprintable characters, and might not be null-terminated.
     * 
     * @return  the exact SSID value, packaged in a byte array. 
     */
    const GByteArray* getSSID() const;
    
    /**
     * Gets the access point SSID, converted into a printable string object.
     * This value should only be used for displaying the access point name to
     * the user, or for debug output.
     * 
     * @return  the SSID as printable text.
     */
    juce::String getSSIDText() const;
    
    /**
     * Gets the MAC address of the wifi access point.
     * 
     * @return the access point's BSSID, or the empty string if the access point
     *         is null.
     */
    const char* getBSSID() const;
    
    /**
     * Gets the wifi access point frequency in (TODO: what format? MHz? 
     * documentation is unclear, do some tests and figure it out. Or just remove
     * this method, it is currently unused)
     * 
     * @return the wifi access point frequency, or zero if the access point is
     *         null.
     */
    unsigned int getFrequency() const;
    
    /**
     * Gets the access point's maximum data transfer bit rate.
     * 
     * @return the maximum access point bit rate in kb/s, or zero if the access
     *         point is null.
     */
    unsigned int getMaxBitrate() const;
    
    /**
     * Gets the signal strength of the wifi access point.
     * 
     * @return the wifi signal strength, between 0 and 100, or zero if the
     *         access point is null.
     */
    unsigned int getSignalStrength() const;
    
    /**
     * Check the settings of a connection against the properties of this access
     * point to see if the connection could be activated with this access point.
     * 
     * @param connection  A connection object to check for compatibility with
     *                    this access point.
     * 
     * @return true  iff this access point and connection are compatible.
     */
    bool isValidConnection(const Connection& connection) const;
    
   
    /**
     * Gets the device mode of this access point.
     * 
     * @return  the access point's mode, or NM_802_11_MODE_UNKNOWN if the access
     *          point is null. 
     */
    NM80211Mode getMode() const;
   
    /**
     * Get access point flags for this access point.
     * 
     * @return  basic Wifi security flags for this access point, or 
     *          NM_802_11_AP_FLAGS_NONE if the access point is null.
     */
    NM80211ApFlags getFlags() const;
    
    /**
     * Get WPA security flags for this access point.
     * 
     * @return all WPA security flags describing the access point, or
     *         NM_802_11_AP_SEC_NONE if the access point is null. 
     */
    NM80211ApSecurityFlags getWPAFlags() const;
    
    /**
     * Get RSN security flags for this access point.
     * 
     * @return all RSN security flags describing the access point, or
     *         NM_802_11_AP_SEC_NONE if the access point is null. 
     */
    NM80211ApSecurityFlags getRSNFlags() const;
    
    /**
     * Listener objects can subscribe to receive updates when the access point
     * signal strength changes.
     */
    class Listener : public GLib::SignalHandler
    {
    public:     
        Listener() { }
        
        virtual ~Listener() { }
        
    private:   
        friend class AccessPoint;
        
        /**
         * Subscribe to signal strength signals from a single NMAccessPoint.
         * 
         * @param source  A NMAccessPoint accessed as a GObject pointer.
         */
        virtual void connectAllSignals(GObject* source) override;
        
        /**
         * Signals that the access point's signal strength has been updated.
         * 
         * @param updatedAP    The updated wifi access point.
         * 
         * @param newStrength  The access point's new signal strength, ranging 
         *                     from zero to 100.
         */
        virtual void signalStrengthChanged(AccessPoint& updatedAP,
                unsigned int newStrength) = 0;
        
        /**
         * Build signalStrengthChanged() calls from generic property change
         * notifications.
         * 
         * @param source    The unpackaged NMAccessPoint responsible for the
         *                  property change signal.
         * 
         * @param property  The updated property type.
         */
        virtual void propertyChanged
        (GObject* source, juce::String property) override;
    };
    
    
    /**
     * Add a new listener to receive updates from this access point.
     * 
     * @param listener  An object that will be notified when connection signal
     *                  strength changes.
     */
    void addListener(Listener& listener);
};
