# Pocket-home: Bismuth Edition Task List

### Documentation
    1. Create pages documenting each module.
    2. Document all config files and options.
    3. Generate UML diagrams.
        
## Misc. bug fixes and improvements
    -Define missing ColourId values for custom component classes.
    -Figure out why the "file" label in the file selection window gets cut off.

# Task list for release 0.10

## AppMenu Improvements
    1. Write unit tests
    2. Add desktop actions to context menus
    3. Show error alerts for desktop entry exceptions.

## Wifi Improvements
    1. Continue implementing test classes.

## FreeDesktop Standards/ DesktopEntry Improvements
    1. Fix problems with recreating hidden entries.
    2. Add support for launching applications through d-bus when desktop entries
       request it.
    3. Research the FreeDesktop standard for tracking launched applications, and
       integrate it into AppLauncher. 
    4. Move all functionality in AssetFiles to XDGDirectories
    5. Place config files and lock files in directories selected with
       XDGDirectories.
    6. Write DesktopEntry and DesktopEntryLoader test classes.

## Configuration Improvements
    1. Add config. option for enabling/disabling that PocketCHIP audio hack. 
    2. Recreate config test class.

## Settings Page Improvements

### Quick Settings Page

    1. Restore the IP address label.

### DateTime Page

    1. Remove this page until admin/authentication handling is implemented.
    2. Consider restoring with built-in date/time setting components.

### Home Settings Page

    1. Move the clock settings to this page from the DateTime page.
    2. Add options to show/hide the battery icon and wifi icon.
    3. Change the page background settings to control background image only.

### Input Page

    1. Remove FN key fix - it doesn't seem to actually fix anything.
 
### Password Pages

    1. The set password page isn`t hiding the current password field.
    2. Fix issue where the program hangs on authentication in PocketCHIP.

### Colour Page

    1. Add the color page to the settings list page.
    2. Add missing ColourID values to custom component classes.
    3. Adjust color picker and color list components.
    4. Save and apply changes through ColourConfigFile.

# Unscheduled future improvements

## Admin and Authentication
    1. Write a setuid program to handle root actions.
        -Updating the application password.
        -Saving calibration data. 
    2. Create custom PolKit policies for interacting with the setuid program.
    3. Look into implementing a custom PolKit authentication agent.

## Keybinding Controls
    -Decide on implementation approach, add to input settings page.

## Component editing
    -Consider a custom page for this.
    -Support for editing configurableComponents
    -Directly set font size.

## Icons
    -Add an icon selection Component that uses IconThread's index of icon files
        1. Create a method that loads all available icon names from icon
           directories and cache files.
        2. Create a list component that displays all available icons.
        3. Create a popup editor component, containing that list component and
           a field for entering misc. icon paths.
    -Add .xpm image support
    -Fix rendering issues with some .svg icons

## Bluetooth
    1. Study [bluez bluetooth code.]: https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/client/main.c
    2. Implement all basic bluetooth functionality.
        1. Detecting if bluetooth is enabled.
        2. Enabling and disabling the bluetooth radio.
        3. Listing all connected, paired, visible, and saved bluetooth devices.
        4. Pairing with new bluetooth devices.
        5. Setting the bluetooth radio as discoverable.
    3. Implement BluetoothSettingComponent and BluetoothSettingPage
    4. Research bluetooth file transfer to see if pocket-home should handle it.

## Multi-System Support 
    -Test pocket-home on all of my linux devices.
    -Define all system commands in a single .json file in assets
        (*not* editable without admin permissions!) 
    -List all pocket-home functionality that is PocketCHIP specific
    -Create a class that scans for various system features
        -Wifi
        -Bluetooth
        -Controllable backlight
        -Audio support
        -Flashing support
        -Input options
    -Create a class that can automatically disable a component if one or more
     of these features is not available

## Control/Input improvements
    -Create an organized control system for managing key presses and 
     transferring keyboard focus.
    -Ensure that everything controllable by touch/mouse input can also be
     controlled by keyboard input, and vice versa.
    -Look into alternate text entry methods for systems without physical
     keyboards.
     
## Build improvements
   -Switch to using CMake  
