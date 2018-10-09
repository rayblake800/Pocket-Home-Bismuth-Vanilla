##################### Pocket-Home Bismuth Makefile: ############################ 
################################################################################
## Main build targets:                                                        ##
##                                                                            ##
##   build:                Compile default build type.                        ##
##   devinstall:           Install default build binaries.                    ##
##   debug:                Compile debug build, install, and open in gdb.     ##
##   release:              Compile release build and install binaries.        ##
##   check-pkg-config:     Verifies all pkg-config libraries.                 ##
##   clean:                Removes all build files.                           ##
##   strip:                Removes symbols from build files.                  ##
##   uninstall:            Removes installed binaries and data files.         ##
##                                                                            ##
################################################################################
## Build options:                                                             ##
##                                                                            ##
## CONFIG=Debug            Build debug binaries skipping optimizations,       ##
##                         including all test code, and enabling gdb          ##
##                         debugging symbols.                                 ##
##                                                                            ##
## CONFIG=Release          Build release binaries using full optimization,    ##
##                         removing all test code, and omitting gdb flags.    ##
##                                                                            ##
## V=1                     Enable verbose build output.                       ##
##                                                                            ##
## TARGET_ARCH=ARCH_TYPE   Build for system architecture ARCH_TYPE, rather    ##
##                         than building for the compiling system's           ##
##                         architecture.  For example, to build for           ##
##                         PocketCHIP, use TARGET_ARCH=armv7l                 ##
##                                                                            ##
################################################################################
 
######### Build values: #########
# Executable name:
JUCE_TARGET_APP = pocket-home
# Version number:
APP_VERSION = 0.0.8.10
# Version hex 
APP_VERSION_HEX = 0x80a

# Build directories:
JUCE_BINDIR := build
JUCE_LIBDIR := build
# Directory where .o files will be created:
JUCE_OBJDIR := build/intermediate
# Directory where executable files will be created:
JUCE_OUTDIR := build/Debug
# Data installation directory
DATA_PATH := /usr/share/$(JUCE_TARGET_APP)

# Pkg-config libraries:
PKG_CONFIG_LIBS = NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 \
                  x11 xext xinerama
  
# Additional library flags:
LDFLAGS := -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

# Preprocessor values used by JUCE library code:
JUCE_DEFS         := -DDONT_SET_USING_JUCE_NAMESPACE=1 \
                     -DJUCER_LINUX_MAKE_6D53C8B4=1 \
                     -DJUCE_APP_VERSION=$(APP_VERSION) \
                     -DJUCE_APP_VERSION_HEX=$(APP_VERSION_HEX) \

JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 \
                     -DJucePlugin_Build_VST3=0 \
                     -DJucePlugin_Build_AU=0 \
                     -DJucePlugin_Build_AUv3=0 \
                     -DJucePlugin_Build_RTAS=0 \
                     -DJucePlugin_Build_AAX=0 \
                     -DJucePlugin_Build_Standalone=0

# Extra compilation flags:
CPPFLAGS := -pthread $(CPPFLAGS) 

# Extra compilation flags (C++ only):
CXXFLAGS := -std=gnu++14 $(CXXFLAGS)

# Directories to search for header files:
INCLUDE_DIRS := JuceLibraryCode deps/JUCE/modules

# Directories to recursively search for header files:
RECURSIVE_INCLUDE_DIRS := Source Tests

######### Load default values: #########
# Default build type:
ifndef CONFIG
    CONFIG := Debug
endif

# Command used to strip unneeded symbols from object files:
ifndef STRIP
    STRIP = strip
endif

# Use the build system's architecture by default.
ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
endif

#### Setup: #### 

# build with "V=1" for verbose builds
ifeq ($(V), 1)
    V_AT =
else
    V_AT = @
endif

# Disable dependency generation if multiple architectures are set
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

# Generate the list of directory include flags:
DIR_FLAGS := $(shell echo $(INCLUDE_DIRS) | xargs printf " -I'%s'") \
             $(shell find $(RECURSIVE_INCLUDE_DIRS) -type d \
                     -printf " -I'%p'")

FAKE := $(shell echo $(DIR_FLAGS) > dirflags.txt )

# Keep debug and release build files in separate directories:
JUCE_OBJDIR := $(JUCE_OBJDIR)/$(CONFIG)
JUCE_OUTDIR := $(OUTDIR)/$(CONFIG)

ifeq ($(CONFIG),Debug)
    # Debug-specific preprocessor definitions:
    JUCE_CONFIG_FLAGS = -DDEBUG=1 -D_DEBUG=1
    # Debug-specific compiler flags:
    CONFIG_CFLAGS  = -g -ggdb -O0 
	CONFIG_LDFLAGS =
endif

ifeq ($(CONFIG),Release)
    # Release-specific preprocessor definitions:
    JUCE_CONFIG_FLAGS = -DNDEBUG=1
    # Release-specific compiler flags:
    CONFIG_CFLAGS  = -03 -flto
    CONFIG_LDFLAGS =-fvisibility=hidden -flto
endif
  
JUCE_CPPFLAGS := $(DEPFLAGS) \
                 $(JUCE_CONFIG_FLAGS) \
                 $(JUCE_DEFS)\
                 $(shell pkg-config --cflags $(PKG_CONFIG_LIBS)) \
                 $(DIR_FLAGS)

JUCE_CFLAGS   += $(JUCE_CPPFLAGS) \
			     $(TARGET_ARCH) \
			     $(CONFIG_CFLAGS) \
			     $(CFLAGS)

JUCE_CXXFLAGS += $(JUCE_CFLAGS)  \
				 $(CXXFLAGS)

JUCE_LDFLAGS  += $(TARGET_ARCH) \
                 -L$(JUCE_BINDIR) \
                 -L$(JUCE_LIBDIR) \
                 $(shell pkg-config --libs $(PKG_CONFIG_LIBS)) \
                 $(CONFIG_LDFLAGS) \
                 $(LDFLAGS)

CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)

OBJECTS_APP := \
  $(JUCE_OBJDIR)/NetworkInterface.o \
  $(JUCE_OBJDIR)/LibNMInterface.o \
  $(JUCE_OBJDIR)/WifiAccessPoint.o \
  $(JUCE_OBJDIR)/WifiStateManager.o \
  $(JUCE_OBJDIR)/JsonWifiInterface.o \
  $(JUCE_OBJDIR)/BluetoothDevice.o \
  $(JUCE_OBJDIR)/BluetoothStatus.o \
  $(JUCE_OBJDIR)/BluezAdapter.o \
  $(JUCE_OBJDIR)/Audio.o \
  $(JUCE_OBJDIR)/BatteryMonitor.o \
  $(JUCE_OBJDIR)/Display.o \
  $(JUCE_OBJDIR)/I2CBus.o \
  $(JUCE_OBJDIR)/SystemCommands.o \
  $(JUCE_OBJDIR)/XWindowInterface.o \
  $(JUCE_OBJDIR)/ProcessUtils.o \
  $(JUCE_OBJDIR)/AssetFiles.o \
  $(JUCE_OBJDIR)/ConfigurableComponent.o \
  $(JUCE_OBJDIR)/ConfigurableImageButton.o \
  $(JUCE_OBJDIR)/ConfigurableImageComponent.o \
  $(JUCE_OBJDIR)/ConfigurableLabel.o \
  $(JUCE_OBJDIR)/ScrollingAppFolder.o \
  $(JUCE_OBJDIR)/ScrollingAppMenu.o \
  $(JUCE_OBJDIR)/PageAppFolder.o \
  $(JUCE_OBJDIR)/PagedAppMenu.o \
  $(JUCE_OBJDIR)/AppMenuComponent.o \
  $(JUCE_OBJDIR)/AppMenuFolder.o \
  $(JUCE_OBJDIR)/AppMenuButton.o \
  $(JUCE_OBJDIR)/NewFolderEditor.o \
  $(JUCE_OBJDIR)/NewDesktopAppEditor.o \
  $(JUCE_OBJDIR)/NewConfigAppEditor.o \
  $(JUCE_OBJDIR)/CategoryPopupEditor.o \
  $(JUCE_OBJDIR)/AppMenuPopupEditor.o \
  $(JUCE_OBJDIR)/HomePage.o \
  $(JUCE_OBJDIR)/BatteryIcon.o \
  $(JUCE_OBJDIR)/ClockLabel.o \
  $(JUCE_OBJDIR)/WifiIcon.o \
  $(JUCE_OBJDIR)/LoginPage.o \
  $(JUCE_OBJDIR)/FelPage.o \
  $(JUCE_OBJDIR)/PowerPage.o \
  $(JUCE_OBJDIR)/QuickSettingsPage.o \
  $(JUCE_OBJDIR)/WifiSettingsComponent.o \
  $(JUCE_OBJDIR)/BluetoothSettingsComponent.o \
  $(JUCE_OBJDIR)/ConnectionSettingsComponent.o \
  $(JUCE_OBJDIR)/SettingsListPage.o \
  $(JUCE_OBJDIR)/WifiSettingsPage.o \
  $(JUCE_OBJDIR)/BluetoothSettingsPage.o \
  $(JUCE_OBJDIR)/FocusingListPage.o \
  $(JUCE_OBJDIR)/HomeSettingsPage.o \
  $(JUCE_OBJDIR)/UIPage.o \
  $(JUCE_OBJDIR)/ComponentEditorPage.o \
  $(JUCE_OBJDIR)/ColourPage.o \
  $(JUCE_OBJDIR)/InputSettingsPage.o \
  $(JUCE_OBJDIR)/KeybindingPage.o \
  $(JUCE_OBJDIR)/Password.o \
  $(JUCE_OBJDIR)/RemovePasswordPage.o \
  $(JUCE_OBJDIR)/SetPasswordPage.o \
  $(JUCE_OBJDIR)/DateTimePage.o \
  $(JUCE_OBJDIR)/PageComponent.o \
  $(JUCE_OBJDIR)/PageFactory.o \
  $(JUCE_OBJDIR)/PageStackComponent.o \
  $(JUCE_OBJDIR)/ScalingTextButton.o \
  $(JUCE_OBJDIR)/PagedList.o \
  $(JUCE_OBJDIR)/NavButton.o \
  $(JUCE_OBJDIR)/ColourPicker.o \
  $(JUCE_OBJDIR)/CounterComponent.o \
  $(JUCE_OBJDIR)/DrawableImageButton.o \
  $(JUCE_OBJDIR)/DrawableImageComponent.o \
  $(JUCE_OBJDIR)/FileSelectTextEditor.o \
  $(JUCE_OBJDIR)/IconSliderComponent.o \
  $(JUCE_OBJDIR)/ListEditor.o \
  $(JUCE_OBJDIR)/OverlaySpinner.o \
  $(JUCE_OBJDIR)/PopupEditorComponent.o \
  $(JUCE_OBJDIR)/ScalingLabel.o \
  $(JUCE_OBJDIR)/Spinner.o \
  $(JUCE_OBJDIR)/SwitchComponent.o \
  $(JUCE_OBJDIR)/PocketHomeWindow.o \
  $(JUCE_OBJDIR)/PokeLookAndFeel.o \
  $(JUCE_OBJDIR)/LayoutManager.o \
  $(JUCE_OBJDIR)/TransitionAnimator.o \
  $(JUCE_OBJDIR)/MainJSON.o \
  $(JUCE_OBJDIR)/ComponentConfigFile.o \
  $(JUCE_OBJDIR)/ComponentJSON.o \
  $(JUCE_OBJDIR)/ComponentSettings.o \
  $(JUCE_OBJDIR)/ColourConfigFile.o \
  $(JUCE_OBJDIR)/ColourConfigKeys.o \
  $(JUCE_OBJDIR)/ColourJSON.o \
  $(JUCE_OBJDIR)/AppMenuItem.o \
  $(JUCE_OBJDIR)/ConfigItemData.o \
  $(JUCE_OBJDIR)/DesktopEntryItemData.o \
  $(JUCE_OBJDIR)/MenuItemData.o \
  $(JUCE_OBJDIR)/AppConfigFile.o \
  $(JUCE_OBJDIR)/AppJSON.o \
  $(JUCE_OBJDIR)/ConfigKey.o \
  $(JUCE_OBJDIR)/ConfigAlertWindows.o \
  $(JUCE_OBJDIR)/JSONFile.o \
  $(JUCE_OBJDIR)/ConfigJSON.o \
  $(JUCE_OBJDIR)/XDGDirectories.o \
  $(JUCE_OBJDIR)/IconCache.o \
  $(JUCE_OBJDIR)/IconLoader.o \
  $(JUCE_OBJDIR)/IconThemeIndex.o \
  $(JUCE_OBJDIR)/IconThread.o \
  $(JUCE_OBJDIR)/DesktopEntryUtils.o \
  $(JUCE_OBJDIR)/DesktopEntryLoader.o \
  $(JUCE_OBJDIR)/DesktopEntry.o \
  $(JUCE_OBJDIR)/GVariantConverter.o \
  $(JUCE_OBJDIR)/GPPDBusProxy.o \
  $(JUCE_OBJDIR)/SavedConnection.o \
  $(JUCE_OBJDIR)/SavedConnections.o \
  $(JUCE_OBJDIR)/NMPPAccessPoint.o \
  $(JUCE_OBJDIR)/NMPPActiveConnection.o \
  $(JUCE_OBJDIR)/NMPPClient.o \
  $(JUCE_OBJDIR)/NMPPConnection.o \
  $(JUCE_OBJDIR)/NMPPDeviceWifi.o \
  $(JUCE_OBJDIR)/GLibSignalThread.o \
  $(JUCE_OBJDIR)/GLibThread.o \
  $(JUCE_OBJDIR)/GPPObject.o \
  $(JUCE_OBJDIR)/GPPWeakRef.o \
  $(JUCE_OBJDIR)/GSignalHandler.o \
  $(JUCE_OBJDIR)/ThreadResource.o \
  $(JUCE_OBJDIR)/ScopedThreadWriteLock.o \
  $(JUCE_OBJDIR)/ScopedThreadReadLock.o \
  $(JUCE_OBJDIR)/ThreadLock.o \
  $(JUCE_OBJDIR)/SharedResource.o \
  $(JUCE_OBJDIR)/LaunchedProcess.o \
  $(JUCE_OBJDIR)/LocalizedTime.o \
  $(JUCE_OBJDIR)/TempTimer.o \
  $(JUCE_OBJDIR)/Utils.o \
  $(JUCE_OBJDIR)/WindowFocus.o \
  $(JUCE_OBJDIR)/WindowFocusedTimer.o \
  $(JUCE_OBJDIR)/Localized.o \
  $(JUCE_OBJDIR)/AppLauncher.o \
  $(JUCE_OBJDIR)/Main.o \
  $(JUCE_OBJDIR)/PocketHomeApplication.o \
  $(JUCE_OBJDIR)/gtest_object.o \
  $(JUCE_OBJDIR)/GPPTestObject.o \
  $(JUCE_OBJDIR)/test_GPPObject.o \
  $(JUCE_OBJDIR)/StressTest.o \
  $(JUCE_OBJDIR)/test_ConfigFile.o \
  $(JUCE_OBJDIR)/test_LaunchedApp.o \
  $(JUCE_OBJDIR)/test_WifiStateManager.o \
  $(JUCE_OBJDIR)/test_XWindowInterface.o \
  $(JUCE_OBJDIR)/BinaryData.o \
  $(JUCE_OBJDIR)/include_juce_core.o \
  $(JUCE_OBJDIR)/include_juce_data_structures.o \
  $(JUCE_OBJDIR)/include_juce_events.o \
  $(JUCE_OBJDIR)/include_juce_graphics.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics.o \

.PHONY: build devInstall debug release clean strip uninstall

build : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(OBJECTS_APP) $(RESOURCES)
	@echo Linking "pocket-home - App"
	for dir in $(DIR_FLAGS); do
	    @echo $(dir)
	done
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) \
	                 $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) \
					 $(TARGET_ARCH)

$(JUCE_OBJDIR)/NetworkInterface.o: \
    Source/System/Wifi/NetworkInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NetworkInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/LibNMInterface.o: \
    Source/System/Wifi/LibNMInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LibNMInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiAccessPoint.o: \
    Source/System/Wifi/WifiAccessPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiAccessPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiStateManager.o: \
    Source/System/Wifi/WifiStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsonWifiInterface.o: \
    Source/System/Wifi/JsonWifiInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsonWifiInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothDevice.o: \
    Source/System/Bluetooth/BluetoothDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothStatus.o: \
    Source/System/Bluetooth/BluetoothStatus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothStatus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluezAdapter.o: \
    Source/System/Bluetooth/BluezAdapter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluezAdapter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	               -o "$@" -c "$<"

$(JUCE_OBJDIR)/Audio.o: \
    Source/System/Audio.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Audio.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryMonitor.o: \
    Source/System/BatteryMonitor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryMonitor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/Display.o: \
    Source/System/Display.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Display.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/I2CBus.o: \
    Source/System/I2CBus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling I2CBus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/SystemCommands.o: \
    Source/System/SystemCommands.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SystemCommands.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/XWindowInterface.o: \
    Source/System/XWindowInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling XWindowInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessUtils.o: \
    Source/System/ProcessUtils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessUtils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/AssetFiles.o: \
    Source/System/AssetFiles.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AssetFiles.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableComponent.o: \
    Source/Components/Configurable/ConfigurableComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableImageButton.o: \
    Source/Components/Configurable/ConfigurableImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableImageComponent.o: \
    Source/Components/Configurable/ConfigurableImageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableImageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableLabel.o: \
    Source/Components/Configurable/ConfigurableLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScrollingAppFolder.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu/ScrollingAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScrollingAppMenu.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu/ScrollingAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppMenu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageAppFolder.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu/PageAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/PagedAppMenu.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu/PagedAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PagedAppMenu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	              -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuComponent.o: \
    Source/Components/Pages/Home\ Page/AppMenu/AppMenuComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
            	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuFolder.o: \
    Source/Components/Pages/Home\ Page/AppMenu/AppMenuFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuButton.o: \
    Source/Components/Pages/Home\ Page/AppMenu/AppMenuButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewFolderEditor.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewFolderEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewFolderEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewDesktopAppEditor.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewDesktopAppEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewDesktopAppEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewConfigAppEditor.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewConfigAppEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewConfigAppEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/CategoryPopupEditor.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/CategoryPopupEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CategoryPopupEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuPopupEditor.o: \
    Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/AppMenuPopupEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuPopupEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/HomePage.o: \
    Source/Components/Pages/Home\ Page/HomePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryIcon.o: \
    Source/Components/Pages/Home\ Page/Widgets/BatteryIcon.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryIcon.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ClockLabel.o: \
    Source/Components/Pages/Home\ Page/Widgets/ClockLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ClockLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiIcon.o: \
    Source/Components/Pages/Home\ Page/Widgets/WifiIcon.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiIcon.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/LoginPage.o: \
    Source/Components/Pages/Login\ Page/LoginPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoginPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/FelPage.o: \
    Source/Components/Pages/Power\ Page/FelPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FelPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PowerPage.o: \
    Source/Components/Pages/Power\ Page/PowerPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PowerPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/QuickSettingsPage.o: \
    Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/QuickSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling QuickSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsComponent.o: \
    Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/WifiSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsComponent.o: \
    Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/BluetoothSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectionSettingsComponent.o: \
    Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/ConnectionSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectionSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsListPage.o: \
    Source/Components/Pages/Settings\ Pages/Settings\ List\ Page/SettingsListPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsListPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsPage.o: \
    Source/Components/Pages/Settings\ Pages/Network\ Pages/WifiSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsPage.o: \
    Source/Components/Pages/Settings\ Pages/Network\ Pages/BluetoothSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/FocusingListPage.o: \
    Source/Components/Pages/Settings\ Pages/Network\ Pages/FocusingListPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FocusingListPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/HomeSettingsPage.o: \
    Source/Components/Pages/Settings\ Pages/Home\ Settings\ Page/HomeSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomeSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/UIPage.o: \
    Source/Components/Pages/Settings\ Pages/UI\ Page/UIPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UIPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentEditorPage.o: \
    Source/Components/Pages/Settings\ Pages/UI\ Page/ComponentEditorPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentEditorPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourPage.o: \
    Source/Components/Pages/Settings\ Pages/UI\ Page/ColourPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputSettingsPage.o: \
    Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/InputSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/KeybindingPage.o: \
    Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/KeybindingPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KeybindingPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/Password.o: \
    Source/Components/Pages/Settings\ Pages/Password\ Pages/Password.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Password.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/RemovePasswordPage.o: \
    Source/Components/Pages/Settings\ Pages/Password\ Pages/RemovePasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RemovePasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SetPasswordPage.o: \
    Source/Components/Pages/Settings\ Pages/Password\ Pages/SetPasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SetPasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DateTimePage.o: \
    Source/Components/Pages/Settings\ Pages/DateTime\ Page/DateTimePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DateTimePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageComponent.o: \
    Source/Components/Pages/PageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageFactory.o: \
    Source/Components/Pages/PageFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageStackComponent.o: \
    Source/Components/Pages/PageStackComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageStackComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScalingTextButton.o: \
    Source/Components/Widgets/ScalingTextButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScalingTextButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PagedList.o: \
    Source/Components/Widgets/PagedList.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PagedList.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NavButton.o: \
    Source/Components/Widgets/NavButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NavButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourPicker.o: \
    Source/Components/Widgets/ColourPicker.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourPicker.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/CounterComponent.o: \
    Source/Components/Widgets/CounterComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CounterComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DrawableImageButton.o: \
    Source/Components/Widgets/DrawableImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DrawableImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DrawableImageComponent.o: \
    Source/Components/Widgets/DrawableImageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DrawableImageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/FileSelectTextEditor.o: \
    Source/Components/Widgets/FileSelectTextEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FileSelectTextEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconSliderComponent.o: \
    Source/Components/Widgets/IconSliderComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconSliderComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ListEditor.o: \
    Source/Components/Widgets/ListEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ListEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/OverlaySpinner.o: \
    Source/Components/Widgets/OverlaySpinner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OverlaySpinner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PopupEditorComponent.o: \
    Source/Components/Widgets/PopupEditorComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PopupEditorComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScalingLabel.o: \
    Source/Components/Widgets/ScalingLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScalingLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spinner.o: \
    Source/Components/Widgets/Spinner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spinner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SwitchComponent.o: \
    Source/Components/Widgets/SwitchComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SwitchComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PocketHomeWindow.o: \
    Source/Components/PocketHomeWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PocketHomeWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PokeLookAndFeel.o: \
    Source/Components/PokeLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PokeLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/LayoutManager.o: \
    Source/Components/LayoutManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LayoutManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransitionAnimator.o: \
    Source/Components/TransitionAnimator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransitionAnimator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainJSON.o: \
    Source/Config\ Files/Main/MainJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentConfigFile.o: \
    Source/Config\ Files/Component/ComponentConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentJSON.o: \
    Source/Config\ Files/Component/ComponentJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentSettings.o: \
    Source/Config\ Files/Component/ComponentSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourConfigFile.o: \
    Source/Config\ Files/Colour/ColourConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourConfigKeys.o: \
    Source/Config\ Files/Colour/ColourConfigKeys.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourConfigKeys.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourJSON.o: \
    Source/Config\ Files/Colour/ColourJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuItem.o: \
    Source/Config\ Files/AppMenu/Menu\ Items/AppMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigItemData.o: \
    Source/Config\ Files/AppMenu/Menu\ Items/ConfigItemData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigItemData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntryItemData.o: \
    Source/Config\ Files/AppMenu/Menu\ Items/DesktopEntryItemData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntryItemData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/MenuItemData.o: \
    Source/Config\ Files/AppMenu/Menu\ Items/MenuItemData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MenuItemData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppConfigFile.o: \
    Source/Config\ Files/AppMenu/AppConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppJSON.o: \
    Source/Config\ Files/AppMenu/AppJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigKey.o: \
    Source/Config\ Files/ConfigKey.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigKey.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigAlertWindows.o: \
    Source/Config\ Files/ConfigAlertWindows.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigAlertWindows.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/JSONFile.o: \
    Source/Config\ Files/JSONFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JSONFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigJSON.o: \
    Source/Config\ Files/ConfigJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/XDGDirectories.o: \
    Source/FreeDesktop/XDGDirectories.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling XDGDirectories.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconCache.o: \
    Source/FreeDesktop/Icon\ Themes/IconCache.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconCache.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconLoader.o: \
    Source/FreeDesktop/Icon\ Themes/IconLoader.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconLoader.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconThemeIndex.o: \
    Source/FreeDesktop/Icon\ Themes/IconThemeIndex.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconThemeIndex.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconThread.o: \
    Source/FreeDesktop/Icon\ Themes/IconThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntryUtils.o: \
    Source/FreeDesktop/Desktop\ Entries/DesktopEntryUtils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntryUtils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntryLoader.o: \
    Source/FreeDesktop/Desktop\ Entries/DesktopEntryLoader.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntryLoader.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntry.o: \
    Source/FreeDesktop/Desktop\ Entries/DesktopEntry.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntry.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GVariantConverter.o: \
    Source/GLib/DBus/GVariantConverter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GVariantConverter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPDBusProxy.o: \
    Source/GLib/DBus/GPPDBusProxy.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPDBusProxy.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SavedConnection.o: \
    Source/GLib/LibNM/SavedConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SavedConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SavedConnections.o: \
    Source/GLib/LibNM/SavedConnections.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SavedConnections.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPAccessPoint.o: \
    Source/GLib/LibNM/NMPPAccessPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPAccessPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPActiveConnection.o: \
    Source/GLib/LibNM/NMPPActiveConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPActiveConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPClient.o: \
    Source/GLib/LibNM/NMPPClient.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPClient.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPConnection.o: \
    Source/GLib/LibNM/NMPPConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPDeviceWifi.o: \
    Source/GLib/LibNM/NMPPDeviceWifi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPDeviceWifi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibSignalThread.o: \
    Source/GLib/GLibSignalThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibSignalThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibThread.o: \
    Source/GLib/GLibThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPObject.o: \
    Source/GLib/GPPObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPWeakRef.o: \
    Source/GLib/GPPWeakRef.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPWeakRef.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GSignalHandler.o: \
    Source/GLib/GSignalHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GSignalHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThreadResource.o: \
    Source/Util/SharedResource/ThreadResource.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThreadResource.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScopedThreadWriteLock.o: \
    Source/Util/SharedResource/ScopedThreadWriteLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScopedThreadWriteLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScopedThreadReadLock.o: \
    Source/Util/SharedResource/ScopedThreadReadLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScopedThreadReadLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThreadLock.o: \
    Source/Util/SharedResource/ThreadLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThreadLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/SharedResource.o: \
    Source/Util/SharedResource/SharedResource.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SharedResource.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/LaunchedProcess.o: \
    Source/Util/LaunchedProcess.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LaunchedProcess.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/LocalizedTime.o: \
    Source/Util/LocalizedTime.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LocalizedTime.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/TempTimer.o: \
    Source/Util/TempTimer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TempTimer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/Utils.o: \
    Source/Util/Utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/WindowFocus.o: \
    Source/Util/WindowFocus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WindowFocus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/WindowFocusedTimer.o: \
    Source/Util/WindowFocusedTimer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WindowFocusedTimer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/Localized.o: \
    Source/Util/Localized.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Localized.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppLauncher.o: \
    Source/Util/AppLauncher.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppLauncher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main.o: \
    Source/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/PocketHomeApplication.o: \
    Source/PocketHomeApplication.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PocketHomeApplication.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/gtest_object.o: \
    Tests/GLib/gtest_object.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling gtest_object.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPTestObject.o: \
    Tests/GLib/GPPTestObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPTestObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_GPPObject.o: \
    Tests/GLib/test_GPPObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_GPPObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/StressTest.o: \
    Tests/StressTest.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StressTest.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_ConfigFile.o: \
    Tests/test_ConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_ConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_LaunchedApp.o: \
    Tests/test_LaunchedApp.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_LaunchedApp.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_WifiStateManager.o: \
    Tests/test_WifiStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_WifiStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_XWindowInterface.o: \
    Tests/test_XWindowInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_XWindowInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData.o: \
    JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core.o: \
    JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures.o: \
    JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events.o: \
    JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics.o: \
    JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics.o: \
    JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) \
	    -o "$@" -c "$<"

devinstall:
	killall $(JUCE_TARGET_APP);\
	sudo cp build/$(CONFIG)/$(JUCE_TARGET_APP) /usr/bin/$(JUCE_TARGET_APP) && \
	if [ ! -d $(DATA_PATH) ]; then \
	    sudo mkdir $(DATA_PATH) ; \
	fi && \
	sudo cp -R assets/* $(DATA_PATH)

debug:
	$(MAKE) CONFIG=Debug
	reset
	$(MAKE) devInstall CONFIG=Debug
	gdb $(JUCE_TARGET_APP)

release:
	$(MAKE) CONFIG=Release
	reset
	$(MAKE) devInstall CONFIG=Release

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 \
		"pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors $(PKG_CONFIG_LIBS)

clean:
	@echo Cleaning $(JUCE_TARGET_APP)
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping $(JUCE_TARGET_APP)
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

uninstall:
	@echo Uninstalling $(JUCE_TARGET_APP)
	killall $(JUCE_TARGET_APP);\
	sudo rm /usr/bin/$(JUCE_TARGET_APP) && \
	sudo rm -r  $(DATA_PATH) ; \

-include $(OBJECTS_APP:%.o=%.d)
