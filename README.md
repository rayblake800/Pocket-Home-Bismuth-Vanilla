# Pocket-Home Bismuth Edition

Pocket-Home Bismuth is an application launcher for PocketCHIP and other
portable Linux devices.  It provides a highly configurable application menu, along with power controls, a basic WiFi manager, and other assorted system controls and information.

  Bismuth is (mostly) compliant with FreeDesktop application menu standards, automatically detecting installed applications.  The menu interface and contents are fully customizable, either within the application or by editing ~/.pocket-home/apps.json.

  
config options

wifi manager

better process management

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
     libnm-gtk-dev

     gio

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
     libnm-gtk-dev

     gio

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
