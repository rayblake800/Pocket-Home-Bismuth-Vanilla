# Wifi Module Documentation
The Wifi module allows the application to provide Wifi controls and information. It provides a modular [SharedResource](./SharedResource.md) that monitors and controls the system Wifi device and any active Wifi connection. This is used to implement the Wifi controls provided by [Page\::WifiConnection](../../Source/GUI/Page/PageTypes/Page_WifiConnection.h).

#### [Wifi\::Resource](../../Source/System/Wifi/Wifi_Resource.h)
Resource defines the SharedResource object instance that controls access to the Wifi device and cached Wifi data. The Resource object initializes, manages, and destroys the [SharedResource\::Modular\::Module](../../Source/Foundation/SharedResource/Modular/SharedResource_Modular_Module.h) subclasses that handle the Wifi module's responsibilities.

#### [Wifi\::Module](../../Source/System/Wifi/Wifi_Module.h)
Module is the shared basis for all resource modules managed by the Wifi\::Resource. It provides additional functions allowing its subclasses to safely run asynchronous actions and asynchronously interact with connected Module Handler objects.

#### [Wifi\::DebugOutput](../../Source/System/Wifi/Wifi_DebugOutput.h)
DebugOutput converts Wifi enum types to text in order to construct debug output. DebugOutput functions are not included in release builds of the application.

## Wifi Access Points

#### [Wifi\::AccessPoint](../../Source/System/Wifi/Wifi_AccessPoint.h)
AccessPoint represents a Wifi access point visible to the system Wifi device. An AccessPoint may also represent multiple identical access points that connect to the same network. Each AccessPoint is automatically updated whenever its signal strength should change, when a compatible saved connection is added or removed, or when its last connection time changes.

#### [Wifi\::AP\::Data](../../Source/System/Wifi/AP/Wifi_AP_Data.h)
Data objects hold internal Wifi access point data, shared by AccessPoint objects to ensure access point state updates are applied to every AccessPoint.

#### [Wifi\::AP\::StrengthListener](../../Source/System/Wifi/AP/Wifi_AP_StrengthListener.h)
StrengthListener objects receive updates when the signal strength of Wifi access points changes. StrengthListener objects may choose to ignore all updates, get updates from a single access point, or get updates from all visible access points.

#### [Wifi\::AP\::UpdateInterface](../../Source/System/Wifi/AP/Wifi_AP_UpdateInterface.h)
UpdateInterface is the interface inherited by StrengthListener and used by the Wifi\::Signal\::APModule to send updates to StrengthListener objects.

## Wifi UI Components

#### [Wifi\::ConnectionIcon](../../Source/System/Wifi/Component/Wifi_ConnectionIcon.h)

#### [Wifi\::SignalIcon](../../Source/System/Wifi/Component/Wifi_SignalIcon.h)

#### [Wifi\::ControlWidget](../../Source/System/Wifi/Component/Wifi_ControlWidget.h)

#### [Wifi\::ConnectionList\::ListComponent](../../Source/System/Wifi/Component/ConnectionList/Wifi_ConnectionList_ListComponent.h)

#### [Wifi\::ConnectionList\::ControlComponent](../../Source/System/Wifi/Component/ConnectionList/Wifi_ConnectionList_ControlComponent.h)

#### [Wifi\::ConnectionList\::ListButton](../../Source/System/Wifi/Component/ConnectionList/Wifi_ConnectionList_ListButton.h)

## Access Point List Submodule

#### [Wifi\::APList\::Reader](../../Source/System/Wifi/APList/Wifi_APList_Reader.h)

#### [Wifi\::APList\::Writer](../../Source/System/Wifi/APList/Wifi_APList_Writer.h)

#### [Wifi\::APList\::UpdateInterface](../../Source/System/Wifi/APList/Wifi_APList_UpdateInterface.h)

#### [Wifi\::APList\::Module](../../Source/System/Wifi/APList/Wifi_APList_Module.h)

#### [Wifi\::APList\::Listener](../../Source/System/Wifi/APList/Wifi_APList_Listener.h)

#### [Wifi\::APList\::NMReader](../../Source/System/Wifi/APList/Wifi_APList_NMReader.h)

## Wifi Signal Handler Submodules

#### [Wifi\::Signal\::DeviceModule](../../Source/System/Wifi/Signal/Wifi_Signal_DeviceModule.h)

#### [Wifi\::Signal\::ClientModule](../../Source/System/Wifi/Signal/Wifi_Signal_ClientModule.h)

#### [Wifi\::Signal\::APModule](../../Source/System/Wifi/Signal/Wifi_Signal_APModule.h)

## LibNM Interface

#### [Wifi\::LibNM\::APHash](../../Source/System/Wifi/LibNM/Wifi_LibNM_APHash.h)

#### [Wifi\::LibNM\::APMode](../../Source/System/Wifi/LibNM/Wifi_LibNM_APMode.h)

#### [Wifi\::LibNM\::ConnectionHandler](../../Source/System/Wifi/LibNM/Wifi_LibNM_ConnectionHandler.h)

#### [Wifi\::LibNM\::SSID](../../Source/System/Wifi/LibNM/Wifi_LibNM_SSID.h)

#### [Wifi\::LibNM\::ContextTest](../../Source/System/Wifi/LibNM/Wifi_LibNM_ContextTest.h)

#### [Wifi\::LibNM\::SecurityType](../../Source/System/Wifi/LibNM/Wifi_LibNM_SecurityType.h)

### LibNM Event Thread

#### [Wifi\::LibNM\::Thread\::Handler](../../Source/System/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Handler.h)

#### [Wifi\::LibNM\::Thread\::Module](../../Source/System/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Module.h)

### LibNM GObject Wrappers

#### [Wifi\::LibNM\::Connection](../../Source/System/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Connection.h)

#### [Wifi\::LibNM\::Client](../../Source/System/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Client.h)

#### [Wifi\::LibNM\::Settings\::Connection](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Connection.h)

#### [Wifi\::LibNM\::Settings\::Wireless](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Wireless.h)

#### [Wifi\::LibNM\::Settings\::Object](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_Object.h)

#### [Wifi\::LibNM\::Settings\::WirelessSecurity](../../Source/System/Wifi/LibNM/GObjects/Owned/Settings/Wifi_LibNM_Settings_WirelessSecurity.h)

#### [Wifi\::LibNM\::ActiveConnection](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_ActiveConnection.h)

#### [Wifi\::LibNM\::DeviceWifi](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_DeviceWifi.h)

#### [Wifi\::LibNM\::AccessPoint](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_AccessPoint.h)

### LibNM Signal Handling

#### [Wifi\::LibNM\::Signal\::ClientHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_ClientHandler.h)

#### [Wifi\::LibNM\::Signal\::DeviceHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_DeviceHandler.h)

#### [Wifi\::LibNM\::Signal\::APHandler](../../Source/System/Wifi/LibNM/Signal/Wifi_LibNM_Signal_APHandler.h)

### LibNM D-Bus Access

#### [Wifi\::LibNM\::DBus\::SavedConnectionLoader](../../Source/System/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnectionLoader.h)

#### [Wifi\::LibNM\::DBus\::SavedConnection](../../Source/System/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnection.h)

## Wifi Connection Management

#### [Wifi\::Connection\::Event](../../Source/System/Wifi/Connection/Wifi_Connection_Event.h)

### Connection Control Submodule

#### [Wifi\::Connection\::Control\::Module](../../Source/System/Wifi/Connection/Control/Wifi_Connection_Control_Module.h)

#### [Wifi\::Connection\::Control\::Handler](../../Source/System/Wifi/Connection/Control/Wifi_Connection_Control_Handler.h)

### Saved Connection Submodule

#### [Wifi\::Connection\::Saved\::Module](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Module.h)

#### [Wifi\::Connection\::Saved\::Reader](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Reader.h)

#### [Wifi\::Connection\::Saved\::Deleter](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Deleter.h)

### Connection Record Submodule

#### [Wifi\::Connection\::Record\::Reader](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Reader.h)

#### [Wifi\::Connection\::Record\::Listener](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Listener.h)

#### [Wifi\::Connection\::Record\::Module](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Module.h)

#### [Wifi\::Connection\::Record\::UpdateInterface](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_UpdateInterface.h)

#### [Wifi\::Connection\::Record\::Writer](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Writer.h)

## Wifi Device Submodule

#### [Wifi\::Device\::Controller](../../Source/System/Wifi/Device/Wifi_Device_Controller.h)

#### [Wifi\::Device\::UpdateInterface](../../Source/System/Wifi/Device/Wifi_Device_UpdateInterface.h)

#### [Wifi\::Device\::Reader](../../Source/System/Wifi/Device/Wifi_Device_Reader.h)

#### [Wifi\::Device\::Module](../../Source/System/Wifi/Device/Wifi_Device_Module.h)

#### [Wifi\::Device\::Listener](../../Source/System/Wifi/Device/Wifi_Device_Listener.h)

