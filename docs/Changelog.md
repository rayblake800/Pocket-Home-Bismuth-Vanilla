## Release 0.1.1
#### May 7 2019
   - Fixed a crash that could occur when no managed Wifi device is found.
   - UI layouts adapt better to displays in portrait mode.
   - Minor bugfixes.
## Release 0.1.0
#### May 6 2019
   - Updated project name.
   - Updated debian package files.
   - Built and uploaded the initial release.
#### May 2 2019
   - Added user guides.
   - Updated JUCE library submodule.
   - Improved Wifi connection handling, added option to show/hide Wifi passwords.
#### Apr 23 2019
   - Documented all configuration files.
   - Documenting coding style and formatting.
   - Adding and using scripts to automatically format code.
   - Adding implementation guides.
   - Fixed .svg rendering errors.
#### Apr 15 2019
   - Improved ColourId management.
   - Added scripts to handle the process of editing ColourIds.
#### Mar 28 2019
   - Added build options to disable Wifi and PocketCHIP-specific features.
   - Added IP address label.
#### Mar 26 2019
   - Finished refactoring and documenting modules.
   - Improved application password security.
#### Mar 7 2019
   - Added support for .xpm image files.
   - Improved icon file finding.
   - Continuing documentation/design overhaul.
#### Feb 25 2019
   - Redesigned and tested the Wifi module UI.
   - Fixed Page module design issues, and started to remove functionality unrelated to component layout and page transitions from page classes.
#### Feb 21 2019
   - Completely redesigned Wifi module for improved stability and connection management.
   - Continued improvements and bug fixes for the SharedResource module.
#### Dec 2 2018
   - Continuing to restructure the application into more clearly defined modules.
   - Improvements to GLib thread management and event handling.
   - Improvements to LibNM integration.
#### Nov 17 2018
   - Completely redesigned the AppMenu to improve performance, modularity, and maintainability.
   - Testing and bug fixes for DesktopEntry loading.
   - Organized Locale and Config modules.
#### Oct 9 2018
   - Eliminated the project's dependency on the Projucer application.
   - Replaced the auto-generated makefile created by the Projucer with one that's easier to maintain.
#### Oct 3 2018
   - Testing, bug fixes, and design improvements for DesktopEntry file management.
   - Created SharedResource::Thread class for handling shared thread resources.
#### Sep 21 2018
   - Organized various system commands into the SystemCommands module.
#### Sep 15 2018
   - Testing, bug fixes, and design improvements for SharedResource application resource management.
#### Sep 1 2018
   - Testing, bug fixes, and design improvements for GLib/LibNM integration.
#### Aug 17 2018
   - Reimplemented ConfigFile classes with better type security and an improved interface.
   - Created ConfigFile and window management test classes.
#### Aug 2 2018
   - Updates to application launching:
        - Removed xdotool and awesomeWm dependencies by directly handling windows with XLib.
        - Improved ability to locate and focus existing program windows.
        - Tracking child processes in /proc directly to better handle problems with dead processes.
   - Set up juce unit testing as a program option, wrote tests for LaunchedApp.
   - pocket-home now automatically grabs window focus on launch.
#### Jul 23 2018
   - Updated GLibThread to improve thread safety and track window focus.
   - Wifi thread now only runs when the application window is focused.
   - Began documentation overhaul with better task management in TODO.txt.
#### Jul 14 2018
   - Finalized colour management improvements.
   - All Component colours are now controlled through the ColourConfigFile object and the associated colours.json file.
#### Jul 12 2018
   - AdvancedSettingsPage, PageStackComponent, and WifiSettingsComponent now use new layout functionality for animating transitions.
#### Jun 24 2018
   - Added TransitionAnimator class to handle all component transitions.
   - Integrated TransitionAnimator with LayoutManager, enabling animated layout transitions.
   - Refactored LayoutManager to allow for cleaner component layout declarations.
#### Jun 1 2018
   - Reimplemented the application icon search thread to better follow the FreeDesktop icon specification.
   - Added GTK icon cache reading, significantly reducing icon search delays.
#### May 23 2018
   - Completely reimplemented Wifi connection handling to fit LibNM and GLib standards, drastically improving performance.
   - Created C++ RAII object wrappers for GLib/LibNM objects.
   - Created a C++ DBUS interface for accessing saved Wifi connection data.
#### Mar 30 2018
   - Added ResourceManager for safe RAII management of shared resources.
   - Improved password management system, setting or removing the password now requires a PolKit authentication agent.
   - Added localization support
   - Improved PageComponent implementation, added PageFactory class.
#### Mar 8 2018
   - Added paged application menu, along with a configuration option to switch between the new menu type and the old scrolling menu.
#### Feb 25 2018
   - Created WindowFocusedTimer class, all application timers now halt when the application loses focus.
#### Feb 2 2018
   - Improved I2C bus access for battery monitoring and entering fel mode.
#### Jan 20 2018
   - Created popup application menu editor component.
   - Added basic support for editing application menu items and folders
#### Jan 13 2016
   - Created Component layout management class.
   - Added asynchronous DesktopEntry loading.
#### Jan 5 2018
   - Initial DesktopEntry support.
   - Created AppMenuComponent application menu.
   - Began refactoring existing pocket-home code.
   - Created ConfigFile class.
   - Split configuration into three .json files.
