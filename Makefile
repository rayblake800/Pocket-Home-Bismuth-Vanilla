#export CONFIG:=Debug

VERSION=0.0.8.9
BUILD=1

#PKG_CONFIG:=$(shell which pkg-config)

#PKG_CONFIG_PACKAGES = \
#  NetworkManager \
#  libnm-glib \
#	alsa \

#export PKG_CONFIG_CFLAGS=$(foreach pkg, $(PKG_CONFIG_PACKAGES), $(shell $(PKG_CONFIG) --cflags $(pkg)))
#export PKG_CONFIG_LDFLAGS=$(foreach pkg, $(PKG_CONFIG_PACKAGES), $(shell $(PKG_CONFIG) --libs $(pkg)))


all: CONFIG:=Release
all:
	cd Builds/LinuxMakefile && $(MAKE)

clean:
	cd Builds/LinuxMakefile && $(MAKE) clean

wifitest: CONFIG:=Debug
wifitest:
	cd Builds/LinuxMakefile && $(MAKE) -f UnitTests.mk ../../build/$(CONFIG)/wifitest
	
debug: CONFIG:=Debug
debug:
	reset
	cd Builds/LinuxMakefile && $(MAKE)
	killall pocket-home ;\
	sudo cp build/$(CONFIG)/pocket-home /usr/bin/pocket-home && \
	sudo cp -R assets/* /usr/share/pocket-home

pack: all
	mkdir -p pack-debian/usr/bin/ pack-debian/usr/share/pocket-home/
	cp build/Release/pocket-home pack-debian/usr/bin/
	cp -R assets/* pack-debian/usr/share/pocket-home/
	fakeroot dpkg-deb --build pack-debian pocket-home_$(VERSION)-$(BUILD)_armhf.deb

install: pack
	sudo dpkg -i pocket-home_$(VERSION)-$(BUILD)_armhf.deb

devinstall: CONFIG:=Release
devinstall:
	killall pocket-home ;\
	sudo cp build/$(CONFIG)/pocket-home /usr/bin/pocket-home && \
	sudo cp -R assets/* /usr/share/pocket-home
