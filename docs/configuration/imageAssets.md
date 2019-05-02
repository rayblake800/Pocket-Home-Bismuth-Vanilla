# Image Asset Configuration
The [imageAssets.json](../../assets/configuration/imageAssets.json) configuration file defines all image assets used in the application. Image assets may be provided as absolute paths, or as paths relative to the pocket-home assets directory.

## Basic image keys
Basic image keys refer to a single image file with fixed colors.

Key                       | Description
------------------------- | ---
"home page background"    | An optional background image drawn behind the main application launcher page.
"login page background"   | An optional background image drawn behind the application's login page.
"default icon"            | The default icon image used to represent application menu items that are missing a valid icon.
"login image"             | An optional image to draw in the center of the login page.
"ControlWidget wifi icon" | The Wifi icon drawn on the wifi control widget on the quick settings page.

## Complex image keys
Complex image keys may provide multiple image file paths to switch between, and may provide custom color values. Each of these keys stores a JSON object with an optional color list stored under the "colours" key, and an image list stored under the "image files" key. Configurable colors may only be applied to .svg image files. When provided, each configurable color value will replace a single color in each image file. The colors to be replaced are defined in order in the colours.json configuration file, under the keys "Image0", "Image1", "Image2", "Image3", and "Image4".

Key                    | Image Count | Description
---------------------- | ----------- | ---
"power button"         | 1           | The button that opens the power page from the home page.
"settings button"      | 1           | The button that opens the quick settings page from the home page.
"settings list button" | 1           | The button that opens the settings list page from the quick settings page.
"lock icon"            | 1           | The icon used to show that a wireless network is secured.
"menu frame"           | 1           | The frame drawn on the home page above the application menu but below all home page widgets.
"battery"              | 9           | The home page battery status icon.
"signal strength"      | 5           | Wifi signal strength icons.
"popup menu"           | 1           | The frame drawn behind the component used to edit application menu items.
"navigation buttons"   | 4           | Directional navigation button icons.
"loading spinner"      | 8           | The animated loading spinner.
"volume slider"        | 2           | Low and high volume symbols to draw around the volume slider.
"brightness slider"    | 2           | Low and high brightness symbols to draw around the display brightness slider.
"cancel button"        | 1           | The small button used to cancel changes and remove items in editor components.
"confirm button"       | 1           | The small button used to confirm changes in editor components.

### Alternate image descriptions
Image keys with multiple image assets use each image for a specific purpose, documented here.

### Battery status widget

- Discharging, 0-24%
- Discharging, 25-49%
- Discharging, 50-74%
- Discharging, 75-100%
- Charging, 0-24%
- Charging, 25-49%
- Charging, 50-74%
- Charging, 75-100%
- Missing battery

### Wifi signal strength

- Wifi disabled
- 0-24% signal strength
- 25-49% signal strength
- 50-74% signal strength
- 75-100% signal strength

### Navigation buttons

- Up navigation button
- Down navigation button
- Left navigation button
- Right navigation button

### Loading spinner
Each image is a separate frame in the loading spinner animation.

### Volume slider

- Low volume symbol
- High volume symbol

### Brightness slider

- Low brightness symbol
- High brightness symbol
