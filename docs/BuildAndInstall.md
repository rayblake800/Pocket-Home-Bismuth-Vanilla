# Building and Installing Pocket-Home Bismuth

TODO: add precompiled Debian package binaries

## Building from sources
If building on PocketC.H.I.P, you will need to create and use a swapfile on an external USB drive in order to compile this project without running out of memory. To create and use a 2GB swapfile, run these commands:

    sudo dd if=/dev/zero of=[path to USB drive]/swapfile bs=1024 count=2097152
    sudo chmod 600 [path to USB drive]/swapfile
    sudo mkswap [path to USB drive]/swapfile
    sudo swapon [path to USB drive]/swapfile
Make sure to disable the swapfile with `sudo swapoff [path to USB drive]/swapfile` if you want to remove the USB drive before you shut down the system.
    

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
      git clone --recursive https://github.com/centuryglass/Pocket-Home-Bismuth.git
      cd PocketCHIP-pocket-home
      make
      make install
#### 3. Updating
      git pull
      git submodule update
      make
      make install

# Optional Dependencies
 - *network-manager*: Version 0.9.10.0-7 or greater. Required for WiFi controls.
 - *polkit-gnome*: Required for setting and changing the application password. Any other polkit package containing pkexec and a graphical authentication agent should work, but others are currently untested.
 - *gtk-update-icon-cache*: Required for quickly loading application icons. The system should be configured to run this automatically when new icon directories or files are added.
