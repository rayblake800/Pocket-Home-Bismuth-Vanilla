## Pocket-Home Bismuth Modules

 Pocket-Home Bismuth divides its functionality into modules. Each Module has its own source directory and its own makefile.

 Eventually, each module should have its own namespace, and a clearly defined public interface for other modules to use. At the moment, some modules are just collections of similar classes with ill-defined boundaries and little to no cohesion. See [the main task list](TODO.txt) for lists of modules that need refactoring.

#### [AppMenu](./modules/AppMenu.md)
 Manages the application menu UI component. This module handles the menu's appearance, loads menu contents, and allows the user to edit the menu.

#### [Bluetooth](./modules/Bluetooth.md)
 This module is currently unimplemented. It will provide controls allowing the user to create, view, and edit Bluetooth connections.

#### [Config](./modules/Config.md)
 Reads data from the user's configuration files, writes changes back to those files, and shares configuration data and updates with other modules.

#### [DesktopEntry](./modules/DesktopEntry.md)
 Finds and reads .desktop application shortcut files, organizes the resulting data, and shares it with the AppMenu module.

#### [GLib](./modules/GLib.md)
 Provides tools for managing the GLib library's C-based custom object system, simplifying interaction with libraries build using GLib.

#### [Layout](./modules/Layout.md)
 Provides tools for arranging UI components within the window, and for animating transitions between different component layouts.

#### [Locale](./modules/Locale.md)
 Provides localized display text throughout the application.

#### [Page](./modules/Page.md)
 Provides the layout of all UI pages, and handles transitions between those 
pages.

#### [Password](./modules/Password.md)
 Securely creates, validates, and updates the Pocket-Home application password.

#### [Process](./modules/Process.md)
 Creates and manages new child processes.

#### [Settings](./modules/Settings.md)
 Provides UI components for managing application and system settings.

#### [SharedResource](./modules/SharedResource.md)
 Provides a system for safely creating, accessing, and destroying shared
application resources.

#### [System](./modules/System.md)
 Handles miscellaneous system tasks related to file IO, window management, power management, and general system controls.

#### [Theme](./modules/Theme.md)
 Controls the general appearance of the application in all ways unrelated to layout. This module is responsible for selecting configurable UI image assets and colors.

#### [Testing](./modules/Testing.md)
 Defines all application test classes, along with a set of supporting classes useful for testing and debugging.

#### [Util](./modules/Util.md)
 Contains assorted helper classes that don't really fit in other modules.

#### [Widgets](./modules/Widgets.md)
 Provides basic UI components used throughout the application.

#### [Wifi](./modules/Wifi.md)
 Provides the application's Wifi connection controls, and keeps track of the current state of the system's Wifi device.
