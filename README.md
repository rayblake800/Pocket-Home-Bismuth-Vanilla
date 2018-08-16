# Pocket-Home Bismuth Edition

  Pocket-Home Bismuth is an application launcher for PocketCHIP and other portable Linux devices.  It provides a highly configurable application menu, along with power controls, a basic WiFi manager, and other assorted system controls and information.

  Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications.  The menu interface and contents are fully customizable, either within the application or by editing the config files in the ~/.pocket-home/ directory.
 
[Add here: animated screenshot of paged menu]
 
[Add here: animated screenshot of scrolling menu]

[Add here: animated screenshot of editing a desktop entry]

[Add here: animated screenshot of adding a folder]



### Less obvious improvements vs. ntc pocket-home and marshmallow:
-Code is actually organized and documented.

-Encapsulation and modularity enforced: no more .cpp files filled with four or five loosely related classes full of public data members.

-All UI elements scale based on window size.  All UI elements are now either managed by a LayoutManager object or defined as a ConfigurableComponent.

-WiFi code now runs in a proper GLib event loop, drastically improving Wifi performance.

# Dependencies
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
     glib-2.0.dev

# Optional
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
