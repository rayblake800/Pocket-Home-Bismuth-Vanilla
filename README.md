# Pocket-Home Bismuth Edition

  Pocket-Home Bismuth is an application launcher for PocketCHIP and other portable Linux devices.  It provides a highly configurable application menu, along with power controls, a basic WiFi manager, and other assorted system controls and information.

  Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications.  The menu interface and contents are fully customizable, either within the application or by editing the config files in the ~/.pocket-home/ directory.
 
[Add here: animated screenshot of paged menu]
 
[Add here: animated screenshot of scrolling menu]

[Add here: screenshot of editing a desktop entry]

[Add here: screenshot of adding a folder]

  Although this project started as a fork of Pocket-Home marshmallow, the project has been almost completely re-written.  Code modules are thoroughly documented and organized to simplify the process of introducing new features.  Whenever possible, program attributes are now defined in JSON configuration files.  

  Bismuth also introduces significant performance improvements.  All background timers and processes now stop whenever the program loses focus.  WiFi management is significantly faster, and now properly detects and handles saved connections.

## Development Goals

  Now that NTC is gone and the PocketCHIP has ceased production, the primary goal of this project is to make Pocket-Home as easy as possible to adapt and use for similar handheld GNU/Linux devices.  Future updates will focus on support for a greater variety of hardware options, input controls, and system configurations.  See the [project tasklist](./docs/TODO.txt) for more information on future project goals.

# Optional Requirements

**network-manager(>= 0.9.10.0-7)**: Required for WiFi controls.

**polkit-gnome**: Required for setting and changing the application password. Any other polkit package containing pkexec and a graphical authentication agent should work, but others are currently untested.

**gtk-update-icon-cache**: Required for quickly loading application icons. The system should be configured to run this automatically when new icon directories or files are added. Pocket-home does not yet support automatic icon cache updates.

# How to install

## Building from sources

#### 1. Required Packages

     sudo apt-get install \
     git \
     build-essential \
     libasound2-dev \
     libx11-dev \
     libxrandr-dev \
     libxcursor-dev \
     libxft-dev \
     libxinerama-dev \
     libnm-glib-dev \
     network-manager-dev \
     libi2c-dev \
     libssl-dev \
     libnm-gtk-dev \
     libglib2.0-dev

####  2. Cloning and building
      
      git clone --recursive https://github.com/centuryglass/PocketCHIP-pocket-home.git
      make build
      make devinstall

#### 3. Updating

      git pull
      git submodule update
      make build
      make devinstall


