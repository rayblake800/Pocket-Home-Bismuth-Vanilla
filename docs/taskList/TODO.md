# Pocket-home: Bismuth Edition Task List
Related Pages:
- [Bug Tracking](KnownBugs.md): Known bugs to fix.
- [Design Changes](DesignChanges.md): Planned improvements to project design.
- [Feature Wishlist](FeatureWishlist.md): New features to implement.

## Testing priorities
Ideally, all non-trivial classes should have their own test classes, but most tests are still unimplemented. The following untested classes are particularly complex, or are the most likely to have hidden bugs. These should be prioritized when implementing missing test classes.
- [DesktopEntry::EntryFile](../../Source/Files/DesktopEntry/DesktopEntry_EntryFile.h)
- [GLib::DBus::Proxy](../../Source/Framework/GLib/DBus/GLib_DBus_Proxy.h)
- [Icon::ThreadResource](../../Source/Files/Icon/Icon_ThreadResource.h)
- [Icon::ThemeIndex](../../Source/Files/Icon/Icon_ThemeIndex.h)
- [Layout::Transition::Animator](../../Source/GUI/Layout/Transition/Layout_Transition_Animator.h)
- [Password](../../Source/GUI/Password/Password.h)
- [Wifi::Connection::Record::Module](../../Source/System/Wifi/Connection/Record/Wifi_Connection_Record_Module.h)
- [Wifi::Connection::Saved::Module](../../Source/System/Wifi/Connection/Saved/Wifi_Connection_Saved_Module.h)
- [Wifi::LibNM::DBus::SavedConnection](../../Source/System/Wifi/LibNM/DBus/Wifi_LibNM_DBus_SavedConnection.h)
- [Wifi::LibNM::AccessPoint](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_AccessPoint.h)
- [Wifi::LibNM::DeviceWifi](../../Source/System/Wifi/LibNM/GObjects/Borrowed/Wifi_LibNM_DeviceWifi.h)
- [Wifi::LibNM::Client](../../Source/System/Wifi/LibNM/GObjects/Owned/Wifi_LibNM_Client.h)
- [Wifi::LibNM::Thread::Module](../../Source/System/Wifi/LibNM/Thread/Wifi_LibNM_Thread_Module.h)

## Misc. fixes and improvements
    - Provide a way to override the system locale.
    - Finish implementing Config::AlertWindow to provide useful error messages when configuration errors occur.
    - Remove or improve unnecessary use of component names.
