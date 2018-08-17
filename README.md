# Pocket-Home Bismuth Edition

  Pocket-Home Bismuth is an application launcher for PocketCHIP and other portable Linux devices.  It provides a highly configurable application menu, along with power controls, a basic WiFi manager, and other assorted system controls and information.

  Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications.  The menu interface and contents are fully customizable, either within the application or by editing the config files in the ~/.pocket-home/ directory.
 
[Add here: animated screenshot of paged menu]
 
[Add here: animated screenshot of scrolling menu]

[Add here: animated screenshot of editing a desktop entry]

[Add here: animated screenshot of adding a folder]

  Although this project started as a fork of Pocket-Home marshmallow, the project has been almost completely re-written.  Code modules are thoroughly documented and organized to simplify the process of introducing new features.  Whenever possible, program attributes are now defined in JSON configuration files.  

  Bismuth also introduces significant performance improvements.  All background timers and processes now stop whenever the program loses focus.  WiFi management is significantly faster, and now properly detects and handles saved connections.

## Development Goals

  Now that NTC is gone and the PocketCHIP has ceased production, the primary goal of this project is to make Pocket-Home as easy as possible to adapt and use for similar handheld GNU/Linux devices.  Future updates will focus on support for a greater variety of hardware options, input controls, and system configurations.  See /docs/TODO.txt for more information on future project goals.

# Requirements
    
  Although this project intends
     polkit + authentication agent

     gtk icon caching

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
      make
      make devinstall
      sudo systemctl restart lightdm

#### 3. Updating

      git pull
      git submodule update
      make
      make devinstall
      sudo systemctl restart lightdm
