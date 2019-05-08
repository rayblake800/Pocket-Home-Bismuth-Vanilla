# Building and Installing Pocket-Home Bismuth

## Installing on PocketC.H.I.P.
1. Run `wget https://github.com/centuryglass/Pocket-Home-Bismuth/releases/download/v0.1.0/pocket-home_0.1.1_armhf.deb` or just [download the package from this page](https://github.com/centuryglass/Pocket-Home-Bismuth/releases/download/v0.1.1/pocket-home_0.1.1_armhf.deb).
2. Run `sudo dpkg -i pocket-home_0.1.0_armhf.deb` to install the package.

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
      cd Pocket-Home-Bismuth
      make
      make install
#### 3. Updating
      git pull
      git submodule update
      make
      make install
