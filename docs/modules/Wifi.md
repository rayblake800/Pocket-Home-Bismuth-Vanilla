# Wifi Module Documentation
The Wifi module allows the application to provide Wifi controls and information. It provides a modular [SharedResource](./SharedResource.md) that monitors and controls the system Wifi device and any active Wifi connection. This is used to implement the Wifi controls provided by [Page\::WifiConnection](../../Source/GUI/Page/PageTypes/Page_WifiConnection.h).

#### [Wifi\::Resource](../../Source/System/Wifi/Wifi_Resource.h)
Resource defines the SharedResource object instance that controls access to the Wifi device and cached Wifi data. The Resource object initializes, manages, and destroys the [SharedResource\::Modular\::Module](../../Source/Framework/SharedResource/Modular/SharedResource_Modular_Module.h) subclasses that handle the Wifi module's responsibilities.

#### [Wifi\::Module](../../Source/System/Wifi/Wifi_Module.h)
Module is the shared basis for all resource modules managed by the Wifi\::Resource. It provides additional functions allowing its subclasses to safely run asynchronous actions and asynchronously interact with connected Module Handler objects.

#### [Wifi\::FocusUpdater](../../Source/System/Wifi/Wifi_FocusUpdater.h)
FocusUpdater ensures that Wifi connection status is up to date when the main application window gains focus and the Wifi thread restarts.

#### [Wifi\::DebugOutput](../../Source/System/Wifi/Wifi_DebugOutput.h)
DebugOutput converts Wifi enum types to text in order to construct debug output. DebugOutput functions are not included in release builds of the application.

## Wifi Device Submodule
The Device submodule controls basic access to the system's Wifi device.

#### [Wifi\::Device\::Module](../../Source/System/Wifi/Device/Wifi_Device_Module.h)
The Device\::Module object tracks whether the system has a Wifi device managed by NetworkManager. If a valid Wifi device is found, the Module will track whether that device is enabled or disabled, and whether the device is currently turning on or off.

#### [Wifi\::Device\::Reader](../../Source/System/Wifi/Device/Wifi_Device_Reader.h)
Device\::Reader objects connect to the Device Module to check if the Wifi device exists, if it is enabled, and if it is being turned on or off.

#### [Wifi\::Device\::Listener](../../Source/System/Wifi/Device/Wifi_Device_Listener.h)
Device\::Listener objects receive updates from the Device Module whenever the Wifi device is enabled, disabled, or starting to turn on or off.

#### [Wifi\::Device\::UpdateInterface](../../Source/System/Wifi/Device/Wifi_Device_UpdateInterface.h)
Device\::UpdateInterface is the interface inherited by Listener objects and used by the Device Module to send updates to all Listener objects.

#### [Wifi\::Device\::Controller](../../Source/System/Wifi/Device/Wifi_Device_Controller.h)
Device\::Controller objects are used to enable or disable Wifi, or to make the Wifi device scan for new access points.

## Wifi Access Points

#### [Wifi\::AccessPoint](../../Source/System/Wifi/Wifi_AccessPoint.h)
AccessPoint represents a Wifi access point visible to the system Wifi device. An AccessPoint may also represent multiple identical access points that connect to the same network. Each AccessPoint is automatically updated whenever its signal strength should change, when a compatible saved connection is added or removed, or when its last connection time changes.

#### [Wifi\::AP\::Data](../../Source/System/Wifi/AP/Wifi_AP_Data.h)
AP\::Data objects hold internal Wifi access point data, shared by AccessPoint objects to ensure access point state updates are applied to every AccessPoint.

#### [Wifi\::AP\::StrengthListener](../../Source/System/Wifi/AP/Wifi_AP_StrengthListener.h)
AP\::StrengthListener objects receive updates when the signal strength of Wifi access points changes. StrengthListener objects may choose to ignore all updates, get updates from a single access point, or get updates from all visible access points.

#### [Wifi\::AP\::UpdateInterface](../../Source/System/Wifi/AP/Wifi_AP_UpdateInterface.h)
AP\::UpdateInterface is the interface inherited by StrengthListener and used by the Wifi\::Signal\::APModule to send updates to StrengthListener objects.

## Access Point List Submodule
The APList submodule monitors updates to the list of visible Wifi access points, caching and sharing visible access point data.

#### [Wifi\::APList\::Module](../../Source/System/Wifi/APList/Wifi_APList_Module.h)
The APList\::Module object uses LibNM access point data to construct, update, and share a list of AccessPoint objects representing all Wifi access points visible to the system's Wifi device.

#### [Wifi\::APList\::Reader](../../Source/System/Wifi/APList/Wifi_APList_Reader.h)
APList\::Reader objects connect to the APList module to read the list of visible access points. Reader objects may search for a single access point by its hash value, or get the entire access point list.

#### [Wifi\::APList\::Listener](../../Source/System/Wifi/APList/Wifi_APList_Listener.h)
APList\::Listener objects receive notification from the APList module when new access points are discovered and added to the list, or when existing access points are lost.

#### [Wifi\::APList\::UpdateInterface](../../Source/System/Wifi/APList/Wifi_APList_UpdateInterface.h)
APList\::Update interface is the interface inherited by APList Listener objects and used by the APList Module object to send updates to all Listener objects.

## Wifi Signal Handler Submodules
The signal handler submodules receive and handle update signals from the wifi device, the network client, and individual access points.

#### [Wifi\::Signal\::DeviceModule](../../Source/System/Wifi/Signal/Wifi_Signal_DeviceModule.h)
The DeviceModule object listens for LibNM Wifi device signals, using them to update other Wifi Resource modules. It passes added and removed access point signals to the APList\::Module, and device state updates and active connection changes to the Connection\::Record\::Module.

#### [Wifi\::Signal\::ClientModule](../../Source/System/Wifi/Signal/Wifi_Signal_ClientModule.h)
The ClientModule object listens for LibNM network client signals indicating that wireless networking was enabled or disabled. It uses these signals to update the Device\::Module object.

#### [Wifi\::Signal\::APModule](../../Source/System/Wifi/Signal/Wifi_Signal_APModule.h)
The APModule listens for signal strength change signals from all LibNM\::AccessPoint objects. It uses these signals to update the signal strengths tracked by the APList\::Module object.

## Wifi Connection Management
The Wifi module is responsible for managing Wifi network connections. It monitors the current connection state, creates new connections, closes existing connections, and manages saved connections.

#### [Wifi\::Connection\::Event](../../Source/System/Wifi/Connection/Wifi_Connection_Event.h)
Event objects store a record of a single Wifi connection event. They hold an EventType enum value describing the type of event, the time that the event occurred, and the AccessPoint object associated with the event.

### Connection Record Submodule
Connection\::Record monitors the current connection state and records all Wifi connection events.

#### [Wifi\::Connection\::Record\::Module](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Module.h)
The Record\::Module object stores Connection\::Event objects representing all Wifi connection events that have occurred since the application started. It uses these records to get information about the current Wifi connection state, or to find the most recent records of an AccessPoint or event type.

#### [Wifi\::Connection\::Record\::Handler](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Handler.h)
Record\::Handler objects connect to the Record\::Module to read the current connection status, get the active AccessPoint object, check recorded events, and check for unrecorded updates to save.

#### [Wifi\::Connection\::Record\::Listener](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Listener.h)
Record\::Listener objects receive notification from the Record\::Module whenever a Wifi connection starts connecting, connects successfully, fails to authenticate, or disconnects.

#### [Wifi\::Connection\::Record\::UpdateInterface](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_UpdateInterface.h)
Record\::UpdateInterface is the interface inherited by Record\::Listener objects and used by the Record\::Module object to send updates to all Listeners.

### Connection Control Submodule
Connection\::Control is responsible for creating new Wifi connections, and closing existing Wifi connections.

#### [Wifi\::Connection\::Control\::Module](../../Source/System/Wifi/Connection/Control/Wifi_Connection_Control_Module.h)
The Control\::Module object is responsible for creating new Wifi connections, and closing existing Wifi connections. When opening a new connection, the Module will monitor the ongoing connection and attempt to fix any problems it encounters.

#### [Wifi\::Connection\::Control\::Handler](../../Source/System/Wifi/Connection/Control/Wifi_Connection_Control_Handler.h)
Control\::Handler objects connect to the Control\::Module to start new Wifi connection attempts, or to close the existing Wifi connection.

### Saved Connection Submodule
Connection\::Saved is responsible for loading saved connection information and deleting saved connections.

#### [Wifi\::Connection\::Saved\::Module](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Module.h)
The Saved\::Module object stores and manages the list of saved network connections. It checks if AccessPoint objects have compatible saved connections and updates those objects with the appropriate saved connection status. Given an AccessPoint, it can also find the last time a compatible saved connection was active, or delete all compatible saved connections associated with that access point.

#### [Wifi\::Connection\::Saved\::Reader](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Reader.h)
Saved\::Reader objects connect to the Saved\::Module object to find saved connection information for individual AccessPoint objects. They can check for a saved connection, return the most appropriate saved connection object for an AccessPoint, or return the last time that connection was active.

#### [Wifi\::Connection\::Saved\::Deleter](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Deleter.h)
Saved\::Deleter objects connect to the Saved\::Module objects to delete saved network connections. Given an AccessPoint object, they find and delete all connections saved with NetworkManager that are compatible with that object.

## LibNM Interface
The Wifi module accesses the Wifi device and network connections through NetworkManager, using libnm-glib. Wifi\::LibNM manages LibNM data resources and provides a more convenient interface for using LibNM functions.

#### [Wifi\::LibNM\::APHash](../../Source/System/Wifi/LibNM/Wifi_LibNM_APHash.h)
APHash objects uniquely identify a Wifi connection point. Each Wifi\::AccessPoint object will have a unique APHash value. LibNM\::AccessPoint objects will share an APHash value if they are associated with the same network, otherwise their APHash values will also be unique.

#### [Wifi\::LibNM\::APMode](../../Source/System/Wifi/LibNM/Wifi_LibNM_APMode.h)
APMode is an enum class representing the four access point modes tracked by LibNM.

#### [Wifi\::LibNM\::SecurityType](../../Source/System/Wifi/LibNM/Wifi_LibNM_SecurityType.h)
SecurityType is an enum class representing the main types of Wifi connection security handled by LibNM.

#### [Wifi\::LibNM\::ConnectionHandler](../../Source/System/Wifi/LibNM/Wifi_LibNM_ConnectionHandler.h)
ConnectionHandler is an interface provided for objects that handle LibNM connection opening callbacks. The Connection\::Control\::Module object implements this interface in order to handle new connection attempts that it starts.

#### [Wifi\::LibNM\::SSID](../../Source/System/Wifi/LibNM/Wifi_LibNM_SSID.h)
SSID objects store the name used to identify an access point. SSID values may contain unprintable characters, so SSID objects provide a method to access their data as a printable string.

#### [Wifi\::LibNM\::ContextTest](../../Source/System/Wifi/LibNM/Wifi_LibNM_ContextTest.h)
ContextTest provides a debug macro for asserting that code is running within the LibNM thread's GLib event loop.

### LibNM Event Thread
Data managed by LibNM needs to be accessed only within a GLib event loop running within a single thread, using the global default GLib main context. Wifi::LibNM runs this event loop within a Wifi::Resource module.

#### [Wifi\::LibNM\::Thread\::Module](../../Source/System/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Module.h)
The Thread\::Module object runs the LibNM GLib event loop within its own thread. It initializes LibNM functionality, creating the LibNM client and wifi device objects.

#### [Wifi\::LibNM\::Thread\::Handler](../../Source/System/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Handler.h)
Thread\::Handler objects connect to the Thread\::Module to access the shared client and wifi device objects, and to schedule code to run within the Wifi event loop.

### LibNM GObject Wrappers
Wifi\::LibNM uses the [GLib module](./GLib.md) to provide container objects for managing LibNM GObject data pointers.

#### [Wifi\::LibNM\::Client](../../Source/System/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Client.h)
Client objects manage NMClient* values, which represent a connection to NetworkManager. Client objects load LibNM\::DeviceWifi objects for system wifi devices, and LibNM\::ActiveConnection objects for active or activating network connections. Client objects are also used to enable or disable wireless networing, and to create or close network connections.

#### [Wifi\::LibNM\::AccessPoint](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_AccessPoint.h)
AccessPoint objects manage NMAccessPoint* values, used by the NetworkManager to represent nearby visible access points. They may be used to access all relevant data NetworkManager stores regarding the access point they represent. A valid AccessPoint object is required by the Client in order to open a Wifi connection.

#### [Wifi\::LibNM\::DeviceWifi](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_DeviceWifi.h)
DeviceWifi objects manage NMDeviceWifi* data, used to represent a system Wifi device tracked by NetworkManager. DeviceWifi objects provide general information about the device they represent, load LibNM\::AccessPoint objects representing access points the device detects, and provide limited control over the Wifi device.

#### [Wifi\::LibNM\::ActiveConnection](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_ActiveConnection.h)
ActiveConnection objects manage NMActiveConnection* data, used to represent an active or activating network connection. It provides information about the status and properties of the connection, including the LibNM\::AccessPoint used to create the connection.

#### [Wifi\::LibNM\::Connection](../../Source/System/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Connection.h)
Connection objects manage NMConnection* data, holding the settings and information needed to create a Wifi connection. A valid Connection object is required by the Client in order to activate a Wifi connection.

#### [Wifi\::LibNM\::Settings\::Object](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Object.h)
Settings\::Object objects manage NMSetting* data. NMSetting is an abstract basis for other LibNM setting data types used to store different types of connection data. Relevant connection data is added to Connection objects through Settings\::Object subclasses. Connection objects may also be loaded from SavedConnection objects in order to view their properties or reactivate their connections.

#### [Wifi\::LibNM\::Settings\::Connection](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Connection.h)
Settings\::Connection objects manage NMSettingConnection* data, holding the most basic connection data common to all network connections. This includes the connection's name ID, it's unique UUID value, the name of its network interface, and the last time it was activated.

#### [Wifi\::LibNM\::Settings\::Wireless](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Wireless.h)
Settings\::Wireless objects manage NMSettingWireless* data, holding information specific to wireless connection types. This includes SSID, access point mode, BSSIDs of known access point devices, and whether the connection is hidden.

#### [Wifi\::LibNM\::Settings\::WirelessSecurity](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_WirelessSecurity.h)
Settings\::WirelessSecurity objects manage NMSettingWirelessSecurity* data, holding security settings and any necessary key or passphrase needed to open a secured wireless connection.

### LibNM Signal Handling
LibNM uses GObject signals to send out updates when networking data is updated. LibNM\::Signal provides classes used to build objects that receive these signals.

#### [Wifi\::LibNM\::Signal\::ClientHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_ClientHandler.h)
ClientHandler is a basis for classes that receive signals from the LibNM\::Client object. When connected to the Client, these objects receive a ClientHandler::wirelessStateChange call whenever wireless networking is enabled or disabled. Wifi\::Signal\::ClientModule serves as the only necessary Signal\::ClientHandler subclass.

#### [Wifi\::LibNM\::Signal\::DeviceHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_DeviceHandler.h)
DeviceHandler is a basis for classes that receive signals from the LibNM\::DeviceWifi object. When connected to the DeviceWifi, these classes recieve signals when the device state changes, when visible access points are added or removed, and when the device's active connection changes. Wifi\::Signal\::DeviceModule serves as the only necessary Signal\::DeviceHandler class.

#### [Wifi\::LibNM\::Signal\::APHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_APHandler.h)
APHandler is a basis for classes that receive signals from LibNM\::AccessPoint objects. When connected to an AccessPoint, these objects receive an APHandler::signalStrengthChanged call whenever the signal strength changes for a connected access point. Wifi\::Signal\::APModule serves as the only necessary Signal\::APHandler class.

### LibNM D-Bus Access
The version of LibNM used by this module fails to load saved connection data within a multi-threaded environment, even if all LibNM actions are confined to a single thread. Because of this, information about saved network connections is loaded directly from NetworkManager over a D-Bus connection.

#### [Wifi\::LibNM\::DBus\::SavedConnectionLoader](../../Source/System/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnectionLoader.h)
SavedConnectionLoader objects load NetworkManager's saved connections over DBus. It loads the list of saved Wifi connections, and finds saved connections that are compatible with specific access points.

#### [Wifi\::LibNM\::DBus\::SavedConnection](../../Source/System/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnection.h)
Each SavedConnection object represents a single saved network connection provided by SavedConnectionLoader. SavedConnection objects provide information about the connection they represent, including the connection path, connection type, and the last time the connection was active. SavedConnection objects can create a LibNM\::Connection object needed to reactivate their connection. They can also connect with NetworkManager over D-Bus to delete their saved connection.

