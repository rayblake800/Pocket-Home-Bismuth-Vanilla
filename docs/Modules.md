## Pocket-Home Bismuth Modules

 Pocket-Home Bismuth divides its functionality into modules. Each Module has its own source directory, makefile, and namespace.

### GUI
GUI modules create the application's user interface.

#### [AppMenu](./modules/AppMenu.md)
The AppMenu module creates and manages the application menu UI component.

#### [Settings](./modules/Settings.md)
The Settings module provides UI components for managing application and system settings.

#### [Page](./modules/Page.md)
The Page module provides the layout of all UI pages, and handles transitions between those pages.

#### [Password](./modules/Password.md)
The Password module securely creates, validates, and updates the Pocket-Home application password.

#### [Theme](./modules/Theme.md)
The Theme module controls the general appearance of the application in all ways unrelated to layout. This module is responsible for selecting configurable UI image assets and colors.

#### [Info](./modules/Info.md)
The Info module provides components that show information to the user.

### Files
File modules find, control, and process different types of files.

#### [Assets](./modules/Assets.md)
The Assets module locates and loads application asset files.

#### [Config](./modules/Config.md)
The Config module reads data from the user's configuration files, writes changes back to those files, and shares configuration data and updates with other modules.

#### [DesktopEntry](./modules/DesktopEntry.md)
The DesktopEntry module and reads .desktop application shortcut files, organizes the resulting data, and shares it with the AppMenu module.

#### [Icon](./modules/Icon.md)
The Icon module locates, loads, and caches icon files using the system's icon theme.

### System
System modules provide tools for interacting with system applications, hardware, and the operating system.

#### [Bluetooth](./modules/Bluetooth.md)
The Bluetooth module is currently unimplemented. It will provide controls allowing the user to create, view, and edit Bluetooth connections.

#### [Process](./modules/Process.md)
The process module creates and manages new child processes.

#### [Hardware](./modules/Hardware.md)
The hardware module provides tools for monitoring and controlling device hardware.

#### [Wifi](./modules/Wifi.md)
The Wifi module controls and tracks the system Wifi device and active Wifi connection.

#### [Window](./modules/Window.md)
The Window module finds, tracks, and controls open windows.

### Foundation
Foundation modules provide tools used to design and build other modules.

#### [GLib](./modules/GLib.md)
The GLib module provides tools for managing the GLib library's C-based custom object system, simplifying interaction with libraries build using GLib.

#### [Layout](./modules/Layout.md)
The Layout module provides tools for arranging UI components within the window, and for animating transitions between different component layouts.

#### [Locale](./modules/Locale.md)
The Locale module provides localized display text throughout the application.

#### [SharedResource](./modules/SharedResource.md)
 Provides a system for safely creating, accessing, and destroying shared application resources.

#### [Widgets](./modules/Widgets.md)
The Widgets module provides UI components that may be reused throughout the application.

#### [Util](./modules/Util.md)
The Util module provides miscellaneous support classes and functions used to build the application.

### Development
Development modules provide tools for testing and improving application design.

#### [Testing](./modules/Testing.md)
The Testing module provides tools used to build application test classes.

#### [Debug](./modules/Debug.md)
The Debug module provides tools for testing application performance and locating errors.

