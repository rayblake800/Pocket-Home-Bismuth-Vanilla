VERSION=0.0.8.10
BUILD=1

.PHONY: all debug debugClean debugBuild debugInstall clean build devinstall pack install

all:
	$(MAKE) build
	$(MAKE) devinstall

debug:
	$(MAKE) debugBuild
	reset
	$(MAKE) debugInstall

debugClean:
	cd Builds/LinuxMakefile && env CONFIG=Debug $(MAKE) clean

debugBuild:
	echo building Debug
	cd Builds/LinuxMakefile && env CONFIG=Debug $(MAKE)

debugInstall:
	killall pocket-home ;\
	sudo cp build/Debug/pocket-home /usr/bin/pocket-home && \
	sudo cp -R assets/* /usr/share/pocket-home

clean:
	cd Builds/LinuxMakefile && env CONFIG=Release $(MAKE) clean

build:
	echo building Release
	cd Builds/LinuxMakefile && env CONFIG=Release $(MAKE)

devinstall:
	killall pocket-home ;\
	sudo cp build/Release/pocket-home /usr/bin/pocket-home && \
	sudo cp -R assets/* /usr/share/pocket-home

pack: all
	mkdir -p pack-debian/usr/bin/ pack-debian/usr/share/pocket-home/
	cp build/Release/pocket-home pack-debian/usr/bin/
	cp -R assets/* pack-debian/usr/share/pocket-home/
	fakeroot dpkg-deb --build pack-debian pocket-home_$(VERSION)-$(BUILD)_armhf.deb

install: pack
	sudo dpkg -i pocket-home_$(VERSION)-$(BUILD)_armhf.deb

