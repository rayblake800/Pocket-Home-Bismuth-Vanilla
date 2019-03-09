# Project Design Plans
This document tracks all changes to the way this project is designed and structured that are currently under consideration.

## Project Level Changes
- Reorganize all modules into better broad categories.

## Module Changes

### GLib Module
- Rename to something that suggests the library name rather than using the same name for the module.

### Layout
- Find a more elegant place for TextSize, or at least a more elegant way to integrate it into Layout::Component.

### Password
- Use XDG Data directories to select where to save the password file.
- Check both custom and default locations in case someone messes with the XDG_DATA_HOME variable.

### Process
- Create a module namespace and use it for all module files.

### Settings
- Move functionality from page classes into component group classes, move pages to Page module.
- Consider completely dissolving this module, possibly integrating all of it into the Page module.

### System
- Create a module namespace and use it for all module files.
- Move power page to Page module

### SharedResource
- Remove from the Util directory

### Theme
- Divide the LookAndFeel class into more manageable pieces.
- Find a more organized approach to managing Theme::Colour ID values.
- Reduce the number of redundant places ColourId values must be added.
- Make it simpler to manage ColourId values by Component type.

### Util
- Divide into modules with coherent themes, even if they're small.

### Widgets
- Move Widgets::Clock to the Page module

### Wifi
- Get rid of distinct Owned and Borrowed subdirectories for GLib::Object classes.
