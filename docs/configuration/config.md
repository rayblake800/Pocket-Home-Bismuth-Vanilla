# General Application Configuration
The config.json file is used to store general application settings. This page explains how settings defined in this file are used, and the values they are allowed to hold.

#### Wifi AP Scan frequency
**Permitted values:** Any integer.
**Purpose:** Sets how frequently in milliseconds that the system should scan for new Wifi access points while the Wifi Connection page is open. If this value is zero or less, the connection page will only scan for access points once when it is opened.

#### Wifi interface
**Permitted values:** Any string.
**Purpose:** Selects the Wifi interface that pocket-home should use when monitoring and controlling Wifi connections. If this value isn't set to a valid Wifi interface name, the Wifi module will attempt to automatically find and select an appropriate Wifi device interface.

#### Terminal launch command
**Permitted values:** Any string.
**Purpose:** Stores the terminal launch command prefix. Adding this value to the beginning of a command should create a new command that runs the original command within a new terminal window.

#### Show cursor
**Permitted values:** true/false
**Purpose:** Sets if the mouse cursor should be shown within the pocket-home window.

#### Show clock
**Permitted values:** true/false
**Purpose:** Sets if the current time should be shown on the application home page.

#### Use 24h mode
**Permitted values:** true/false
**Purpose:** Sets whether time values should be printed using 24 hour time format, or using twelve hour AM/PM time format.

#### Use IP label on home page
**Permitted values:** true/false
**Purpose:** Sets if the system's IP address should be shown on the application home page.

#### Use IP label on settings page
**Permitted values:** true/false
**Purpose:** Sets if the system's IP address should be shown on the main settings page.

#### IP label prints local IP
**Permitted values:** true/false
**Purpose:** Sets if the IP address label should print the system's address on the local network.

#### IP label prints public IP
**Permitted values:** true/false
**Purpose:** Sets if the IP address label should print the system's public IP address.
