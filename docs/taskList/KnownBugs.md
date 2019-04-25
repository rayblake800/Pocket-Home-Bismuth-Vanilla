## Known Issues

### DesktopEntry Module
- Poorly formatted .desktop files may cause unexpected behavior.
- Problems occur when recreating custom .desktop files that still exist as hidden entries.

### Theme Module
- Changes to colour values are usually not applied to components until the application restarts.
- Popup windows do a poor job of formatting text to fit.
- The file label gets cut off in file selection windows.

### Wifi Module
- Detecting when connections fail because of an invalid key/passphrase fails on desktop tests with modern versions of NetworkManager.
- Invalid new saved connections aren't being properly deleted.

### Windows Module
- Checking if the pocket-home main window is focused using XWindows doesn't seem to be working properly on PocketCHIP.
