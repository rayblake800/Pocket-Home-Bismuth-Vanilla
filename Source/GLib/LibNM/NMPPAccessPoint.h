#pragma once
#include <nm-access-point.h>
#include "NMPPConnection.h"
#include "GPPObject.h"

/**
 * @file NMPPAccessPoint
 * 
 * @brief A RAII container and C++ interface for NMAccessPoint objects.
 * 
 * NMPPAccessPoint holds a LibNM object representing a wifi access point.  It
 * provides access to relevant access point data, and checks NMPPConnection 
 * objects for compatibility with this access point.
 * 
 * It also provides the NMPPAccessPoint::Listener class as an interface for 
 * receiving access point signals. After they have been added, listeners will
 * receive updates if signal strength changes or the access point is removed. 
 */

class NMPPAccessPoint : public GPPObject
{
public:
    /**
     * Create a NMPPAccessPoint sharing a GObject with an existing
     * NMPPAccessPoint.
     * 
     * @toCopy  An existing connection object.
     */
    NMPPAccessPoint(const NMPPAccessPoint& toCopy);

    /**
     * Create a NMPPAccessPoint to contain a NMAccessPoint object.
     * 
     * @toAssign  A valid NMAccessPoint for this NMPPAccessPoint to hold.
     */
    NMPPAccessPoint(NMAccessPoint* toAssign);
    
    /**
     * Create a void NMPPAccessPoint.
     */
    NMPPAccessPoint();
    
    /**
     * Gets the access point SSID as a byte array from the access point.  This 
     * may contain unprintable characters, and might not be null-terminated.
     * 
     * @return  the exact SSID value, packaged in a byte array. 
     */
    const GByteArray* getSSID();
    
    /**
     * Gets the access point SSID, converted into a printable string object.
     * This value should only be used for displaying the access point name to
     * the user, or for debug output.
     * 
     * @return  the SSID as printable text.
     */
    String getSSIDText();
    
    /**
     * Gets the MAC address of the wifi access point.
     * 
     * @return the access point's BSSID, or nullptr if the access point is void.
     */
    const char* getBSSID();
    
    /**
     * Gets the wifi access point frequency in (TODO: what format? MHz? 
     * documentation is unclear, do some tests and figure it out.)
     * 
     * @return the wifi access point frequency, or zero if the access point is
     *         void.
     */
    unsigned int getFrequency();
    
    /**
     * Gets the access point's maximum data transfer bit rate.
     * 
     * @return the maximum access point bit rate in kb/s, or zero if the access
     *         point is void.
     */
    unsigned int getMaxBitrate();
    
    /**
     * Gets the signal strength of the wifi access point.
     * 
     * @return the wifi signal strength, between 0 and 100, or zero if the
     *         access point is void.
     */
    unsigned int getSignalStrength();
    
    /**
     * Returns the timestamp (in CLOCK_BOOTTIME seconds) for the last time the 
     * access point was found in scan results. A value of -1 means the access 
     * point has not been found in a scan.
     * 
     * @return  the last time seen in seconds, or -1 if the access point is void
     *          or has not been found in a scan.
     */
    int getLastSeen();
    
    /**
     * Check the settings of a connection against the properties of this access
     * point to see if the connection could be activated with this access point.
     * 
     * @param connection  A connection object to check for compatibility with
     *                    this access point.
     * 
     * @return true  iff this access point and connection are compatible.
     */
    bool isValidConnection(const NMPPConnection& connection);
    
    /**
     * Get access point flags for this access point.
     * 
     * @return  basic Wifi security flags for this access point, or 
     *          NM_802_11_AP_FLAGS_NONE if the access point is void.
     */
    NM80211ApFlags getFlags();
    
    /**
     * Get WPA security flags for this access point.
     * 
     * @return all WPA security flags describing the access point, or
     *         NM_802_11_AP_SEC_NONE if the access point is void. 
     */
    NM80211ApSecurityFlags getWPAFlags();
    
    /**
     * Get RSN security flags for this access point.
     * 
     * @return all RSN security flags describing the access point, or
     *         NM_802_11_AP_SEC_NONE if the access point is void. 
     */
    NM80211ApSecurityFlags getRSNFlags();
    
    /**
     * Listener objects can subscribe to receive updates when the access point
     * signal strength changes, or the access point is removed.
     */
    class Listener : public GPPObject::SignalHandler
    {
    public:
        friend class NMPPAccessPoint;       
        Listener() { }
        virtual ~Listener() { }
    private:
        /**
         * Signals that an access point has been removed, and should no longer
         * be saved.
         * 
         * @param removed  The removed wifi access point object.
         */
        virtual void accessPointRemoved(NMPPAccessPoint* removed) = 0;
        
        /**
         * Signals that the access point's signal strength has been updated.
         * 
         * @param updatedAP    The updated wifi access point.
         * 
         * @param newStrength  The access point's new signal strength, ranging 
         *                     from zero to 100.
         */
        virtual void signalStrengthChanged(NMPPAccessPoint* updatedAP,
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
        void propertyChanged(GPPObject* source, String property) override;
    };
    
    /**
     * Add a new listener object to receive signals from this access point.
     * 
     * @param listener  An object that needs to act when connection signal
     *                  strength changes or the access point is removed.
     */
    void addListener(Listener* listener);
    
    /**
     * Remove a listener object from this access point.
     * 
     * @param listener  This object will no longer be notified when signal
     *                  strength changes or the access point is removed.
     */
    void removeListener(Listener* listener);
    
private:
    /**
     * Get the NMAccessPoint class GType
     * 
     * @return NM_TYPE_ACCESS_POINT
     */
    GType getType() const override;    
};