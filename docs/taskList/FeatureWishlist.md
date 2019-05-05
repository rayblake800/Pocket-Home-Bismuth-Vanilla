# New Features to Implement

### Icon selection:
- Create a component for selecting icon files from the list of all icons.
    1. Add functions to the Icon module for getting all icon file names that meet a set of criteria.
    2. Create a list component for browsing through the resulting list and selecting icons.
    3. Replace the existing icon selection components with the new component.
- Add UI controls for selecting icon themes.
- Add support for generating gtk icon cache files on systems that don't update them automatically.

### Improving system compatibility:
- Allow all hardware and system specific features to be enabled or disabled when building.
- Create a configure file for greater build customization.
    - Automatically detect supported hardware features.
    - Detect and adjust appropriate system commands for SystemCommands.json.
    - Provide finer levels of control for optimization, debugging and testing features.

### Improved application launching and process management:
- Add support for launching applications through d-bus.
- Make better use of application launch data provided in .desktop files.
- Find a fix for the Juce framework's difficulties with reading output from processes that exited with errors.

### Keybinding controls:
- Add a JSON configuration file defining all keybindings.
- Create a UI component that captures and saves keybindings.
- Add a settings page for editing keybindings.
- Replace all hard-coded bindings with config bindings.

### Compatibility with a broader set of input schemes:
- The program currently requires a full keyboard and a touch screen or mouse.
- Update UI to support any combination of the following input devices:
    1. Touchscreen or mouse
    2. Full keyboard
    3. Simple button controls (gameboy style)
- Create JUCE::KeyboardFocusTraverser classes for the entire UI
    - Ensure all focusable UI components provide some sort of indication that they're focused.
    - Consider handling focus traversal at the Page::Component level.
- Implement or include an on-screen keyboard
    - Keyboard should be usable with a mouse, touch screen, or simple navigation buttons.

### Better testing for UI components:
- Create a TestPage Component class that displays a single UI component along with controls for triggering various component tests.
- Provide a function for automatically triggering and evaluating component tests when possible.
- Create a TestList Window class that finds and lists buttons to launch every TestPage type.
- Add a command line option to create the TestList window instead of the PocketHomeWindow on launch.

### Bluetooth connection controls:
- Study [bluez bluetooth code](https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/client/main.c).
- Add UI controls for managing bluetooth connections.
    1. Create a control component modeled after the Wifi::ControlWidget
        - Detects and displays whether bluetooth is enabled.
        - Turns bluetooth on or off.
        - Opens a bluetooth connection list page.
    2. Create a bluetooth connection list and bluetooth connection control page modeled after Wifi::ConnectionList and Page::WifiConnection.
        - Provide a button to make bluetooth temporarily discoverable.
        - Display connected, paired, and visible devices.
        - Expand to provide additional information and controls for individual devices.
        - Provide controls for creating new connections, reopening saved connections, and deleting saved connections.

### Admin and authentication tools:
- Write a setuid program to handle root actions.
    - Updating the application password.
    - Saving calibration data.
    - Changing the date and time.
- Create custom PolKit policies for interacting with the setuid program.
- Implement a PolKit authentication agent within the application to eliminate the dependency on the GNOME authentication agent.

### Date/Time control page:
- Re-implement with built in controls for system time and date.
- Postpone this until Admin and Authentication Tools are updated.

### Improved debug output:
- Allow debug messages to be categorized as error, debug, or verbose.
- Provide command line options to enable or disable output based on these categories.
- Provide command line options to enable or disable output for individual modules.

### GLib GVariant support:
- Finish writing a wrapper class that can handle any type of GVariant value.
- Use this wrapper class to simplify existing d-bus code.

### Theme support:
- Save colour, imageAssets, layout, and menuOptions config files together as a single theme.
- Preview saved themes within a settings page.
- Load and apply saved themes without restarting.

### Component editing:
- Set component layouts within a UI settings page.
- Provide controls for setting font sizes within the application.
- Provide controls for selecting image resources within the application.
