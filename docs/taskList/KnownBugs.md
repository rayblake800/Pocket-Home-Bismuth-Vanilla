## Known Issues

### Icon Module
- .xpm icon files are not supported by the Juce library's Image class.
- Juce .svg rendering fails to correctly render some .svg files.

### DesktopEntry Module
- Poorly formatted .desktop files may cause unexpected behavior.

### Theme Module
- ColourIds using default colour values don't show their default color in the color page.
- Clicking on color values in the color page doesn't copy them to the color picker.
- Changes to color values are usually not applied to components until they're reconstructed.
- Popup windows do a poor job of formatting text to fit.
- The file label gets cut off in file selection windows.
