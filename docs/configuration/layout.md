# Layout Configuration File
The layout.json configuration file sets the positions and sizes of UI components that shouldn't be managed using a relative group layout. Application font sizes are also defined here.

## Font sizes
Pocket-Home Bismuth uses three font size keys, "small text", "medium text", and "large text". Font sizes may be defined as either a fraction of the application window height, or as a fixed size in pixels. In most circumstances, text components will use the largest of these font sizes that would still allow their text to fit within the component bounds. If none of the configurable sizes would fit, these values are ignored and an even smaller font size is selected automatically.

## Component Layouts
Component layouts each define a UI component's position and size. X-position and width are defined as fractions of the application window's width, and Y-position and height are defined as fractions of the window height. Layouts may not need to provide all values, as some components may override parts of the layout anyway.

Layout Key                  | Layout Values Used  | Description
--------------------------- | ------------------- | ---
"scrolling app menu"        | x, y, width, height | The application launch menu, when using the scrolling menu format.
"paged app menu"            | x, y, width, height | The application launch menu, when using the paged menu format.
"home page battery icon"    | x, y, width, height | The icon used to indicate battery charge level and charging state.
"battery percent text"      | x, y, width, height | The battery charge percentage label.
"home page clock label"     | x, y, width, height | The label that prints the current time.
"home page IP label"        | x, y, width, height | The label that prints the system's local and/or public IP address.
"home page Wifi icon"       | x, y, width, height | The icon used to indicate Wifi connection strength.
"menu frame"                | x, y, width, height | The frame image drawn on the home page in front of the application launch menu and behind all home page widgets.
"power page button"         | x, y, width, height | The image button used to open the power page.
"settings page button"      | x, y, width, height | The image button used to open the quick settings page.
"popup editor menu"         | width, height       | The popup editor component used to edit application menu items.
"left arrow button"         | x, y, width, height | The directional navigation button pointing left.
"right arrow button"        | x, y, width, height | The directional navigation button pointing right.
"up arrow button"           | x, y, width, height | The directional navigation button pointing up.
"down arrow button"         | x, y, width, height | The directional navigation button pointing down.
"settings list page button" | x, y, width, height | The image button that opens the settings list page.
"loading spinner"           | width, height       | The animated loading spinner widget.
