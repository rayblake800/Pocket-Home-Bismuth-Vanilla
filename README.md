
# Pocket-Home Bismuth Edition

  Pocket-Home Bismuth is an application launcher for PocketCHIP and other portable Linux devices.  It provides a configurable application menu, power controls, and a basic WiFi manager.

  Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications. The menu interface and contents are fully customizable, either within the application or by editing the application's JSON configuration files.
 
[Add here: animated screenshot of paged menu]
 
[Add here: animated screenshot of scrolling menu]

[Add here: screenshot of editing a desktop entry]

[Add here: screenshot of adding a folder]

  Although this project started as a fork of Pocket-Home marshmallow, the project has been almost completely re-written. All project files have been documented and organized to simplify the process of introducing new features. Whenever possible, program attributes are designed to be easily modified or user-configurable. 

  Bismuth also introduces significant performance improvements. All background timers and processes now stop whenever the program loses focus. WiFi controls are faster and more reliable. 

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
     libxpm-dev \
     libnm-glib-dev \
     network-manager-dev \
     libi2c-dev \
     libssl-dev \
     libnm-gtk-dev \
     libglib2.0-dev

####  2. Cloning and building
      
      git clone --recursive https://github.com/centuryglass/PocketCHIP-pocket-home.git
      make
      make devinstall

#### 3. Updating

      git pull
      git submodule update
      make 
      make devinstall

# Optional Requirements

 - *network-manager*: Version 0.9.10.0-7 or greater. Required for WiFi controls.
 
 - *polkit-gnome*: Required for setting and changing the application password. Any other polkit package containing pkexec and a graphical authentication agent should work, but others are currently untested.
 
 - *gtk-update-icon-cache*: Required for quickly loading application icons. The system should be configured to run this automatically when new icon directories or files are added.

## Development Goals

  Now that NTC is gone and the PocketCHIP has ceased production, the primary goal of this project is to make Pocket-Home as easy as possible to adapt to similar handheld GNU/Linux devices. Future updates will focus on support for a greater variety of hardware options, input controls, and system configurations. See the [project tasklist](./docs/TODO.txt) for more information on future project goals.

  Pocket-Home Bismuth is inspired by my experiences with the GP2X, the Tapwave Zodiac, and of course, the PocketCHIP itself. Now that quirky, niche handheld computers like these are relatively affordable and easy to design, I want to make Bismuth into my ideal interface for using these devices.

## Additional Documentation

[Change Log](./docs/Changelog.md)

[Project Task List](./docs/taskList/TODO.md)

[Design Overview](./docs/taskList/Modules.md)

[Library References](./docs/Libraries.md)
