pocket-home freedesktop launcher

I'm working on building a proper application launcher from pocket-home marshmallow that
follows the freedesktop standard.  Built-in application management is currently
lacking, but there a lot more options you can tweak in ~/.pocket-home/config.json.
![Menu example](home.gif?raw=true "Home Menu")
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


####  2. Cloning and building
      
      git clone --recursive https://github.com/o-marshmallow/PocketCHIP-pocket-home/
      make
      make devinstall
      sudo systemctl restart lightdm


#### 3. Updating

      git pull
      git submodule update
      make
      make devinstall
      sudo systemctl restart lightdm
