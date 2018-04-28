#include "NMPPAccessPoint.h"
/*
 * Create a NMPPAccessPoint sharing a GObject with an existing
 * NMPPAccessPoint.
 */
 NMPPAccessPoint::NMPPAccessPoint(const NMPPAccessPoint& toCopy)
{
}

/*
 * Create a NMPPAccessPoint to contain a NMAccessPoint object.
 */
 NMPPAccessPoint::NMPPAccessPoint(NMAccessPoint* toAssign)
{
}

/*
 * Create a void NMPPAccessPoint.
 */
 NMPPAccessPoint::NMPPAccessPoint()
{
}

/*
 * Gets the access point SSID as a byte array from the access point.  This 
 * may contain unprintable characters, and might not be null-terminated.
 */
const GByteArray* NMPPAccessPoint::getSSID()
{
}

/*
 * Gets the access point SSID, converted into a printable string object.
 * This value should only be used for displaying the access point name to
 * the user, or for debug output.
 */
String NMPPAccessPoint::getSSIDText()
{
}

/*
 * Gets the MAC address of the wifi access point.
 */
const char* NMPPAccessPoint::getBSSID()
{
}

/*
 * Gets the wifi access point frequency in (TODO: what format? MHz? 
 * documentation is unclear, do some tests and figure it out.)
 */
unsigned int NMPPAccessPoint::getFrequency()
{
}

/*
 * Gets the access point's maximum data transfer bit rate.
 */
unsigned int NMPPAccessPoint::getMaxBitrate()
{
}

/*
 * Gets the signal strength of the wifi access point.
 */
unsigned int NMPPAccessPoint::getSignalStrength()
{
}

/*
 * Returns the timestamp(in CLOCK_BOOTTIME seconds) for the last time the 
 * access point was found in scan results. A value of -1 means the access 
 * point has not been found in a scan.
 */
int NMPPAccessPoint::getLastSeen()
{
}

/*
 * Check the settings of a connection against the properties of this access
 * point to see if the connection could be activated with this access point.
 */
bool NMPPAccessPoint::isValidConnection(const NMPPConnection& connection)
{
}

/*
 * Get access point flags for this access point.
 */
NM80211ApFlags NMPPAccessPoint::getFlags()
{
}

/*
 * Get WPA security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getWPAFlags()
{
}

/*
 * Get RSN security flags for this access point.
 */
NM80211ApSecurityFlags NMPPAccessPoint::getRSNFlags()
{
}

/*
 * Build NMPPAccessPoint::Listener::signalStrengthChanged() calls from generic property change
 * notifications.
 */
void NMPPAccessPoint::Listener::propertyChanged(GPPObject* source, String property)
{
}

/*
 * Add a new listener object to receive signals from this access point.
 */
void NMPPAccessPoint::addListener(Listener* listener)
{
}

/*
 * Remove a listener object from this access point.
 */
void NMPPAccessPoint::removeListener(Listener* listener)
{
}

/*
 * Get the NMAccessPoint class GType
 */
GType NMPPAccessPoint::getType() const
{
}    

/*
 * Check if a GObject's type allows it to be held by this object. 
 */
bool NMPPAccessPoint::isValidType(GObject* toCheck) const
{
}

/*
 * Used to re-add a list of signal handlers to new GObject data.
 */
void NMPPAccessPoint::transferSignalHandlers
(Array<SignalHandler*>& toTransfer)
{
}