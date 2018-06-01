VERSION=0.0.8.9
BUILD=1

all: CONFIG:=Release
all:
	echo building $(CONFIG)
	cd Builds/LinuxMakefile && $(MAKE)

clean:
	cd Builds/LinuxMakefile && $(MAKE) clean

debug: CONFIG:=Debug
debug:
	reset
	echo building $(CONFIG)
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
