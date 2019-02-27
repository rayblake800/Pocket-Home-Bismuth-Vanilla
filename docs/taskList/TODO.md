# Pocket-home: Bismuth Edition Task List
  Related Pages:
    - [Release Goals](ReleaseGoals.md)
    - [Bug Tracking](KnownBugs.md)
    - [Feature Wishlist](FeatureWishlist.md)

### Documentation
    1. Finish creating pages documenting each module.
    2. Document all config files and options.
    3. Generate UML diagrams.

## Testing priorities
  Ideally, all non-trivial classes should have their own test classes, but very few classes actually do. The following untested classes are particularly complex, or are the most likely to be hiding hidden bugs.  These should be prioritized when implementing missing test classes.
    - [Config::FileResource](../../Source/Config/Config_FileResource.h) 
    - [Config::Listener](../../Source/Config/Config_Listener.h) 
    - [DesktopEntry::EntryFile](../../Source/DesktopEntry/DesktopEntry_EntryFile.h) 
    - [GLib::DBus::Proxy](../../Source/GLib/DBus/GLib_DBus_Proxy.h)
    - [Icon::ThreadResource](../../Source/Icon/Icon_ThreadResource.h)
    - [Icon::ThemeIndex](../../Source/Icon/Icon_ThemeIndex.h)
    - [Layout::Transition::Animator](../../Source/Layout/Transition/Layout_Transition_Animator.h)
    - [Password](../../Source/Password/Password.h)
    - [Wifi::Connection::Record::Module](../../Source/Wifi/Connection/Record/Wifi_Connection_Record_Module.h)
    - [Wifi::Connection::Saved::Module](../../Source/Wifi/Connection/Saved/Wifi_Connection_Saved_Module.h)
    - [Wifi::LibNM::DBus::SavedConnection](../../Source/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnection.h)
    - [Wifi::LibNM::AccessPoint](../../Source/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_AccessPoint.h)
    - [Wifi::LibNM::DeviceWifi](../../Source/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_DeviceWifi.h)
    - [Wifi::LibNM::Client](../../Source/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Client.h)
    - [Wifi::LibNM::Thread::Module](../../Source/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Module.h)

## Misc. fixes and improvements
    - Define missing ColourId values for custom component classes.
    - Figure out why the "file" label in the file selection window gets cut off.

    - ### Component editing
        - Set component layout within a UI settings page.
        - Provide controls for setting font sizes within the application.
        - Provide controls for selecting image resources within the application. 

## Bluetooth
    1. Study [bluez bluetooth code](https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/client/main.c).
    2. Implement and test a GLib::DBusProxy class to handle bluetooth actions.

## Config
    - Finish implementing Config::AlertWindow to provide useful error messages when configuration errors occur.
