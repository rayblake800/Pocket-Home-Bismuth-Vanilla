## Known Issues

### DesktopEntry Module
- Poorly formatted .desktop files may cause unexpected behavior.

### Theme Module
- ColourIds using default colour values don't show their default colour in the colour page.
- Clicking on colour values in the colour page doesn't copy them to the colour picker.
- Changes to colour values are usually not applied to components until they're reconstructed.
- Popup windows do a poor job of formatting text to fit.
- The file label gets cut off in file selection windows.

### Wifi Module
- Detecting when connections fail because of an invalid key/passphrase fails on desktop tests with modern versions of NetworkManager.
- Invalid new saved connections aren't being properly deleted.
