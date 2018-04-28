#include "../Source/GLib/LibNM/NMPPClient.h"

/*
 * Create a NMPPClient holding a new NMClient object.
 */ NMPPClient::NMPPClient()
{
}

/*
 * Create a NMPPClient that shares a NMClient with another NMPPClient.
 */ NMPPClient::NMPPClient(const NMPPClient& toCopy)
{
}

/*
 * Get all wifi devices from Network Manager.
 */
Array<NMPPDeviceWifi> NMPPClient::getWifiDevices()
{
}

/*
 * Gets a specific wifi device using its interface name.
 */
NMPPDeviceWifi NMPPClient::getWifiDeviceByIface(const char* interface)
{
}

/*
 * Gets a specific wifi device using its DBus path.
 */
NMPPDeviceWifi NMPPClient::getWifiDeviceByPath(const char* path)
{
}

/*
 * Gets the list of all active connections known to the network manager.
 */
Array<NMPPConnection> NMPPClient::getActiveConnections()
{
}

/*
 * Gets the primary active network connection.
 */
NMPPConnection NMPPClient::getPrimaryConnection()
{
}

/*
 * Gets the connection being activated by the network manager.
 */
NMPPConnection NMPPClient::getActivatingConnection()
{
}

/*
 * Deactivates an active network connection.
 */
void NMPPClient::deactivateConnection(const NMPPConnection& activeCon)
{
}

/*
 * Checks if wireless connections are currently enabled.
 */
bool NMPPClient::wirelessEnabled()
{
}

/*
 * Sets whether wireless connections are enabled.
 */
void NMPPClient::setWirelessEnabled(bool enabled)
{
}
 NMPPClient::ConnectionHandler::~ConnectionHandler()
{
}

/*
 * Activate an existing network connection.
 */
void NMPPClient::ConnectionHandler::activateConnection(
const NMPPClient& client,
const NMPPConnection& connection,
const NMPPAccessPoint& accessPoint)
{
}

/*
 * Add and activate a new network connection.
 */
void NMPPClient::ConnectionHandler::addAndActivateConnection(
const NMPPClient& client,
const NMPPConnection& connection,
const NMPPDeviceWifi& wifiDevice,
const NMPPAccessPoint& accessPoint)
{
}

/*
 * The NMClientActivateFn called by LibNM when activating an existing
 * connection, used to create a call to openingConnection or to 
 * openingConnectionFailed.
 */
void NMPPClient::ConnectionHandler::activateCallback(NMClient* client,
NMActiveConnection* connection,
GError* error,
NMPPClient::Listener* listener)
{
}

/*
 * The NMClientAddActivateFn called by LibNM when adding and activating
 * a new connection, used to create a call to openingConnection or 
 * openingConnectionFailed.
 */
void NMPPClient::ConnectionHandler::addActivateCallback(NMClient* client,
NMConnection* connection,
const char* path,
GError* error,
NMPPClient::Listener* listener)
{
}

/*
 * Converts generic propertyChanged calls to class-specific 
 * wirelessStateChange calls.
 */
void NMPPClient::propertyChanged(GPPObject* source, String property)
{
}  

/*
 * Adds a listener to this network manager client.
 */
void addListener(Listener* listener)
{
}

/*
 * Removes a listener from this network manager client.
 */
void removeListener(Listener* listener)
{
}

/*
 * Gets the GType of this object's stored GObject class.
 */
GType getType() const
{
}

/*
 * Checks if a GObject's type allows it to be held by this object. 
 */
bool isValidType(GObject* toCheck) const
{
}

/*
 * Used to re-add a list of Listeners to new GObject data.
 */
void transferSignalHandlers(Array<SignalHandler*>& toTransfer)
{
}
}
{
}