VERSION=0.0.8.10
BUILD=1
DATA_PATH="/usr/share/pocket-home"

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
	if [ ! -d $(DATA_PATH) ]; then \
	    sudo mkdir $(DATA_PATH) ; \
	fi && \
	sudo cp -R assets/* $(DATA_PATH)

clean:
	cd Builds/LinuxMakefile && env CONFIG=Release $(MAKE) clean

build:
	echo building Release
	cd Builds/LinuxMakefile && env CONFIG=Release $(MAKE)

devinstall:
	killall pocket-home ;\
	sudo cp build/Release/pocket-home /usr/bin/pocket-home && \
	if [ ! -d $(DATA_PATH) ]; then \
	    sudo mkdir $(DATA_PATH) ; \
	fi && \
	sudo cp -R assets/* $(DATA_PATH)

pack: all
	mkdir -p pack-debian/usr/bin/ pack-debian/usr/share/pocket-home/
	cp build/Release/pocket-home pack-debian/usr/bin/
	cp -R assets/* pack-debian/usr/share/pocket-home/
	fakeroot dpkg-deb --build pack-debian pocket-home_$(VERSION)-$(BUILD)_armhf.deb

install: pack
	sudo dpkg -i pocket-home_$(VERSION)-$(BUILD)_armhf.deb



# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre:
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...
