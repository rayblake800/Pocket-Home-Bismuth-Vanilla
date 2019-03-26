# Project Design Plans
This document tracks all changes to the way this project is designed and structured that are currently under consideration.

## Module Changes

### GLib Module
- Rename to something that suggests the library name rather than using the same name for the module.

### Layout
- Find a more elegant place for TextSize, or at least a more elegant way to integrate it into Layout::Component.

### Theme
- Divide the LookAndFeel class into more manageable pieces.
- Find a more organized approach to managing Theme::Colour ID values.
- Reduce the number of redundant places ColourId values must be added.
- Make it simpler to manage ColourId values by Component type.

### Wifi
- Get rid of distinct Owned and Borrowed subdirectories for GLib::Object classes.
