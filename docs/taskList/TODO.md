# Pocket-home: Bismuth Edition Task List
Related Pages:
- [Bug Tracking](KnownBugs.md): Known bugs to fix.
- [Release Goals](ReleaseGoals.md): Steps to take before the next release.
- [Design Changes](DesignChanges.md): Planned improvements to project design.
- [Feature Wishlist](FeatureWishlist.md): New features to implement.

### Documentation
- Finish creating pages documenting each module.
- Document all config files and options.
- Generate UML diagrams.

## Testing priorities
Ideally, all non-trivial classes should have their own test classes, but most tests are still unimplemented. The following untested classes are particularly complex, or are the most likely to have hidden bugs.  These should be prioritized when implementing missing test classes.
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
- Add more custom colour values to AppMenu button components.
- Provide a way to override the system locale.
- Finish implementing Config::AlertWindow to provide useful error messages when configuration errors occur.
- Restore the IP address label.
