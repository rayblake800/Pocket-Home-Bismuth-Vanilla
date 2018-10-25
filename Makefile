##################### Pocket-Home Bismuth Makefile: ############################ 
################################################################################
## Main build targets:	                                                      ##
##	                                                                          ##
##   build:	            Compile default build type.                           ##
##   devinstall:	    Install default build binaries.                       ##
##   debug:	            Compile debug build, install, and open in gdb.        ##
##   release:	        Compile release build and install binaries.           ##
##   check-pkg-config:	Verifies all pkg-config libraries.                    ##
##   clean:	            Removes all build files.                              ##
##   strip:	            Removes symbols from build files.                     ##
##   uninstall:	        Removes installed binaries and data files.            ##
##	                                                                          ##
################################################################################
## Build options:	                                                          ##
##	                                                                          ##
## CONFIG=Debug	        Build debug binaries skipping optimizations,          ##
##	                     including all test code, and enabling gdb            ##
##	                     debugging symbols.                                   ##
##	                                                                          ##
## CONFIG=Release	      Build release binaries using full optimization,     ##
##	                     removing all test code, and omitting gdb flags.      ##
##	                                                                          ##
## V=1	                 Enable verbose build output.                         ##
##	                                                                          ##
## TARGET_ARCH=ARCH_TYPE   Build for system architecture ARCH_TYPE, rather    ##
##	                     than building for the compiling system's             ##
##	                     architecture.  For example, to build for             ##
##	                     PocketCHIP, use TARGET_ARCH=armv7l                   ##
##	                                                                          ##
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
JUCE_OUTDIR := build
# Data installation directory
DATA_PATH := /usr/share/$(JUCE_TARGET_APP)

# Pkg-config libraries:
PKG_CONFIG_LIBS = NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 \
	              x11 xext xinerama
  
# Additional library flags:
LDFLAGS := -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

# Preprocessor values used by JUCE library code:
JUCE_DEFS	     := -DDONT_SET_USING_JUCE_NAMESPACE=1 \
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
CONFIG ?= Debug

# Command used to strip unneeded symbols from object files:
STRIP ?= strip

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

# Keep debug and release build files in separate directories:
JUCE_OBJDIR := $(JUCE_OBJDIR)/$(CONFIG)
JUCE_OUTDIR := $(JUCE_OUTDIR)/$(CONFIG)

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

# AppMenu Module:
APPMENU_PREFIX := $(JUCE_OBJDIR)/AppMenu_
SCROLLING_MENU_PREFIX := $(APPMENU_PREFIX)Scrolling_
PAGED_MENU_PREFIX := $(APPMENU_PREFIX)Paged_
OBJECTS_APPMENU_SCROLLING := \
  $(SCROLLING_MENU_PREFIX)MenuButton.o \
  $(SCROLLING_MENU_PREFIX)FolderComponent.o \
  $(SCROLLING_MENU_PREFIX)MenuComponent.o \
  $(SCROLLING_MENU_PREFIX)InputHandler.o \
  $(SCROLLING_MENU_PREFIX)Initializer.o 
OBJECTS_APPMENU_PAGED := \
  $(PAGED_MENU_PREFIX)MenuButton.o \
  $(PAGED_MENU_PREFIX)FolderComponent.o \
  $(PAGED_MENU_PREFIX)MenuComponent.o \
  $(PAGED_MENU_PREFIX)InputHandler.o \
  $(PAGED_MENU_PREFIX)Initializer.o 
OBJECTS_APPMENU_ABSTRACT_COMPONENT := \
  $(APPMENU_PREFIX)MenuButton.o \
  $(APPMENU_PREFIX)FolderComponent.o \
  $(APPMENU_PREFIX)MenuComponent.o
OBJECTS_APPMENU_CONTROL := \
  $(APPMENU_PREFIX)Controller.o \
  $(APPMENU_PREFIX)InputHandler.o 
OBJECTS_APPMENU_EDITOR := \
  $(APPMENU_PREFIX)NewConfigItemEditor.o \
  $(APPMENU_PREFIX)NewDesktopAppEditor.o \
  $(APPMENU_PREFIX)ExistingItemEditor.o \
  $(APPMENU_PREFIX)CategoryEditor.o \
  $(APPMENU_PREFIX)PopupEditor.o
OBJECTS_APPMENU_DATA := \
  $(APPMENU_PREFIX)JSONResource.o \
  $(APPMENU_PREFIX)ConfigFile.o \
  $(APPMENU_PREFIX)MenuItem.o \
  $(APPMENU_PREFIX)DesktopEntryData.o \
  $(APPMENU_PREFIX)ConfigData.o \
  $(APPMENU_PREFIX)ItemData.o
OBJECTS_APPMENU := \
  $(OBJECTS_APPMENU_SCROLLING) \
  $(OBJECTS_APPMENU_PAGED) \
  $(OBJECTS_APPMENU_ABSTRACT_COMPONENT) \
  $(OBJECTS_APPMENU_CONTROL) \
  $(OBJECTS_APPMENU_EDITOR) \
  $(OBJECTS_APPMENU_DATA)\
  $(APPMENU_PREFIX)MainComponent.o \
  $(APPMENU_PREFIX)AppMenu.o 

# Config Module:
CONFIG_PREFIX := $(JUCE_OBJDIR)/Config_
OBJECTS_CONFIG := \
  $(CONFIG_PREFIX)MainResource.o \
  $(CONFIG_PREFIX)FileResource.o \
  $(CONFIG_PREFIX)DataKey.o \
  $(CONFIG_PREFIX)AlertWindow.o 

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
  $(OBJECTS_APPMENU) \
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
  $(JUCE_OBJDIR)/ComponentConfigFile.o \
  $(JUCE_OBJDIR)/ComponentJSON.o \
  $(JUCE_OBJDIR)/ComponentSettings.o \
  $(JUCE_OBJDIR)/ColourConfigFile.o \
  $(JUCE_OBJDIR)/ColourConfigKeys.o \
  $(JUCE_OBJDIR)/ColourJSON.o \
  $(OBJECTS_CONFIG) \
  $(JUCE_OBJDIR)/JSONFile.o \
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
  $(JUCE_OBJDIR)/include_juce_gui_basics.o 

.PHONY: build devInstall debug release clean strip uninstall

build : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(OBJECTS_APP) $(RESOURCES)
	@echo Linking "pocket-home - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) \
		             $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) \
					 $(TARGET_ARCH)

$(JUCE_OBJDIR)/NetworkInterface.o: \
	Source/System/Wifi/NetworkInterface.cpp
$(JUCE_OBJDIR)/LibNMInterface.o: \
	Source/System/Wifi/LibNMInterface.cpp
$(JUCE_OBJDIR)/WifiAccessPoint.o: \
	Source/System/Wifi/WifiAccessPoint.cpp
$(JUCE_OBJDIR)/WifiStateManager.o: \
	Source/System/Wifi/WifiStateManager.cpp
$(JUCE_OBJDIR)/JsonWifiInterface.o: \
	Source/System/Wifi/JsonWifiInterface.cpp
$(JUCE_OBJDIR)/BluetoothDevice.o: \
	Source/System/Bluetooth/BluetoothDevice.cpp
$(JUCE_OBJDIR)/BluetoothStatus.o: \
	Source/System/Bluetooth/BluetoothStatus.cpp
$(JUCE_OBJDIR)/BluezAdapter.o: \
	Source/System/Bluetooth/BluezAdapter.cpp
$(JUCE_OBJDIR)/Audio.o: \
	Source/System/Audio.cpp
$(JUCE_OBJDIR)/BatteryMonitor.o: \
	Source/System/BatteryMonitor.cpp
$(JUCE_OBJDIR)/Display.o: \
	Source/System/Display.cpp
$(JUCE_OBJDIR)/I2CBus.o: \
	Source/System/I2CBus.cpp
$(JUCE_OBJDIR)/SystemCommands.o: \
	Source/System/SystemCommands.cpp
$(JUCE_OBJDIR)/XWindowInterface.o: \
	Source/System/XWindowInterface.cpp
$(JUCE_OBJDIR)/ProcessUtils.o: \
	Source/System/ProcessUtils.cpp
$(JUCE_OBJDIR)/AssetFiles.o: \
	Source/System/AssetFiles.cpp
$(JUCE_OBJDIR)/ConfigurableComponent.o: \
	Source/Components/Configurable/ConfigurableComponent.cpp
$(JUCE_OBJDIR)/ConfigurableImageButton.o: \
	Source/Components/Configurable/ConfigurableImageButton.cpp
$(JUCE_OBJDIR)/ConfigurableImageComponent.o: \
	Source/Components/Configurable/ConfigurableImageComponent.cpp
$(JUCE_OBJDIR)/ConfigurableLabel.o: \
	Source/Components/Configurable/ConfigurableLabel.cpp

# AppMenu Module
$(SCROLLING_MENU_PREFIX)MenuButton.o: \
	Source/AppMenu/MenuFormats/Scrolling/MenuButton.cpp
$(SCROLLING_MENU_PREFIX)FolderComponent.o: \
	Source/AppMenu/MenuFormats/Scrolling/FolderComponent.cpp
$(SCROLLING_MENU_PREFIX)MenuComponent.o: \
	Source/AppMenu/MenuFormats/Scrolling/MenuComponent.cpp
$(SCROLLING_MENU_PREFIX)InputHandler.o: \
	Source/AppMenu/MenuFormats/Scrolling/InputHandler.cpp
$(SCROLLING_MENU_PREFIX)Initializer.o: \
	Source/AppMenu/MenuFormats/Scrolling/Initializer.cpp
$(PAGED_MENU_PREFIX)MenuButton.o: \
	Source/AppMenu/MenuFormats/Paged/MenuButton.cpp
$(PAGED_MENU_PREFIX)FolderComponent.o: \
	Source/AppMenu/MenuFormats/Paged/FolderComponent.cpp
$(PAGED_MENU_PREFIX)MenuComponent.o: \
	Source/AppMenu/MenuFormats/Paged/MenuComponent.cpp
$(PAGED_MENU_PREFIX)InputHandler.o: \
	Source/AppMenu/MenuFormats/Paged/InputHandler.cpp
$(PAGED_MENU_PREFIX)Initializer.o: \
	Source/AppMenu/MenuFormats/Paged/Initializer.cpp
$(APPMENU_PREFIX)MenuButton.o: \
    Source/AppMenu/MenuComponents/MenuButton.cpp
$(APPMENU_PREFIX)FolderComponent.o: \
    Source/AppMenu/MenuComponents/FolderComponent.cpp
$(APPMENU_PREFIX)MenuComponent.o: \
    Source/AppMenu/MenuComponents/MenuComponent.cpp
$(APPMENU_PREFIX)Controller.o: \
    Source/AppMenu/Control/Controller.cpp
$(APPMENU_PREFIX)InputHandler.o: \
    Source/AppMenu/Control/InputHandler.cpp
$(APPMENU_PREFIX)NewConfigItemEditor.o: \
    Source/AppMenu/Editors/NewConfigItemEditor.cpp
$(APPMENU_PREFIX)NewDesktopAppEditor.o: \
    Source/AppMenu/Editors/NewDesktopAppEditor.cpp
$(APPMENU_PREFIX)ExistingItemEditor.o: \
    Source/AppMenu/Editors/ExistingItemEditor.cpp
$(APPMENU_PREFIX)PopupEditor.o: \
    Source/AppMenu/Editors/PopupEditor.cpp
$(APPMENU_PREFIX)CategoryEditor.o: \
    Source/AppMenu/Editors/CategoryEditor.cpp
$(APPMENU_PREFIX)JSONResource.o: \
    Source/AppMenu/MenuData/JSONResource.cpp
$(APPMENU_PREFIX)ConfigFile.o: \
    Source/AppMenu/MenuData/ConfigFile.cpp
$(APPMENU_PREFIX)MenuItem.o: \
    Source/AppMenu/MenuData/MenuItem.cpp
$(APPMENU_PREFIX)DesktopEntryData.o: \
    Source/AppMenu/MenuData/DesktopEntryData.cpp
$(APPMENU_PREFIX)ConfigData.o: \
    Source/AppMenu/MenuData/ConfigData.cpp
$(APPMENU_PREFIX)ItemData.o: \
    Source/AppMenu/MenuData/ItemData.cpp
$(APPMENU_PREFIX)MainComponent.o: \
    Source/AppMenu/MainComponent.cpp
$(APPMENU_PREFIX)AppMenu.o: \
    Source/AppMenu/AppMenu.cpp

$(JUCE_OBJDIR)/HomePage.o: \
	Source/Components/Pages/Home\ Page/HomePage.cpp
$(JUCE_OBJDIR)/BatteryIcon.o: \
	Source/Components/Pages/Home\ Page/Widgets/BatteryIcon.cpp
$(JUCE_OBJDIR)/ClockLabel.o: \
	Source/Components/Pages/Home\ Page/Widgets/ClockLabel.cpp
$(JUCE_OBJDIR)/WifiIcon.o: \
	Source/Components/Pages/Home\ Page/Widgets/WifiIcon.cpp
$(JUCE_OBJDIR)/LoginPage.o: \
	Source/Components/Pages/Login\ Page/LoginPage.cpp
$(JUCE_OBJDIR)/FelPage.o: \
	Source/Components/Pages/Power\ Page/FelPage.cpp
$(JUCE_OBJDIR)/PowerPage.o: \
	Source/Components/Pages/Power\ Page/PowerPage.cpp
$(JUCE_OBJDIR)/QuickSettingsPage.o: \
	Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/QuickSettingsPage.cpp
$(JUCE_OBJDIR)/WifiSettingsComponent.o: \
	Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/WifiSettingsComponent.cpp
$(JUCE_OBJDIR)/BluetoothSettingsComponent.o: \
	Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/BluetoothSettingsComponent.cpp
$(JUCE_OBJDIR)/ConnectionSettingsComponent.o: \
	Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/ConnectionSettingsComponent.cpp
$(JUCE_OBJDIR)/SettingsListPage.o: \
	Source/Components/Pages/Settings\ Pages/Settings\ List\ Page/SettingsListPage.cpp
$(JUCE_OBJDIR)/WifiSettingsPage.o: \
	Source/Components/Pages/Settings\ Pages/Network\ Pages/WifiSettingsPage.cpp
$(JUCE_OBJDIR)/BluetoothSettingsPage.o: \
	Source/Components/Pages/Settings\ Pages/Network\ Pages/BluetoothSettingsPage.cpp
$(JUCE_OBJDIR)/FocusingListPage.o: \
	Source/Components/Pages/Settings\ Pages/Network\ Pages/FocusingListPage.cpp
$(JUCE_OBJDIR)/HomeSettingsPage.o: \
	Source/Components/Pages/Settings\ Pages/Home\ Settings\ Page/HomeSettingsPage.cpp
$(JUCE_OBJDIR)/UIPage.o: \
	Source/Components/Pages/Settings\ Pages/UI\ Page/UIPage.cpp
$(JUCE_OBJDIR)/ComponentEditorPage.o: \
	Source/Components/Pages/Settings\ Pages/UI\ Page/ComponentEditorPage.cpp
$(JUCE_OBJDIR)/ColourPage.o: \
	Source/Components/Pages/Settings\ Pages/UI\ Page/ColourPage.cpp
$(JUCE_OBJDIR)/InputSettingsPage.o: \
	Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/InputSettingsPage.cpp
$(JUCE_OBJDIR)/KeybindingPage.o: \
	Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/KeybindingPage.cpp
$(JUCE_OBJDIR)/Password.o: \
	Source/Components/Pages/Settings\ Pages/Password\ Pages/Password.cpp
$(JUCE_OBJDIR)/RemovePasswordPage.o: \
	Source/Components/Pages/Settings\ Pages/Password\ Pages/RemovePasswordPage.cpp
$(JUCE_OBJDIR)/SetPasswordPage.o: \
	Source/Components/Pages/Settings\ Pages/Password\ Pages/SetPasswordPage.cpp
$(JUCE_OBJDIR)/DateTimePage.o: \
	Source/Components/Pages/Settings\ Pages/DateTime\ Page/DateTimePage.cpp
$(JUCE_OBJDIR)/PageComponent.o: \
	Source/Components/Pages/PageComponent.cpp
$(JUCE_OBJDIR)/PageFactory.o: \
	Source/Components/Pages/PageFactory.cpp
$(JUCE_OBJDIR)/PageStackComponent.o: \
	Source/Components/Pages/PageStackComponent.cpp
$(JUCE_OBJDIR)/ScalingTextButton.o: \
	Source/Components/Widgets/ScalingTextButton.cpp
$(JUCE_OBJDIR)/PagedList.o: \
	Source/Components/Widgets/PagedList.cpp
$(JUCE_OBJDIR)/NavButton.o: \
	Source/Components/Widgets/NavButton.cpp
$(JUCE_OBJDIR)/ColourPicker.o: \
	Source/Components/Widgets/ColourPicker.cpp
$(JUCE_OBJDIR)/CounterComponent.o: \
	Source/Components/Widgets/CounterComponent.cpp
$(JUCE_OBJDIR)/DrawableImageButton.o: \
	Source/Components/Widgets/DrawableImageButton.cpp
$(JUCE_OBJDIR)/DrawableImageComponent.o: \
	Source/Components/Widgets/DrawableImageComponent.cpp
$(JUCE_OBJDIR)/FileSelectTextEditor.o: \
	Source/Components/Widgets/FileSelectTextEditor.cpp
$(JUCE_OBJDIR)/IconSliderComponent.o: \
	Source/Components/Widgets/IconSliderComponent.cpp
$(JUCE_OBJDIR)/ListEditor.o: \
	Source/Components/Widgets/ListEditor.cpp
$(JUCE_OBJDIR)/OverlaySpinner.o: \
	Source/Components/Widgets/OverlaySpinner.cpp
$(JUCE_OBJDIR)/PopupEditorComponent.o: \
	Source/Components/Widgets/PopupEditorComponent.cpp
$(JUCE_OBJDIR)/ScalingLabel.o: \
	Source/Components/Widgets/ScalingLabel.cpp
$(JUCE_OBJDIR)/Spinner.o: \
	Source/Components/Widgets/Spinner.cpp
$(JUCE_OBJDIR)/SwitchComponent.o: \
	Source/Components/Widgets/SwitchComponent.cpp
$(JUCE_OBJDIR)/PocketHomeWindow.o: \
	Source/Components/PocketHomeWindow.cpp
$(JUCE_OBJDIR)/PokeLookAndFeel.o: \
	Source/Components/PokeLookAndFeel.cpp
$(JUCE_OBJDIR)/LayoutManager.o: \
	Source/Components/LayoutManager.cpp
$(JUCE_OBJDIR)/TransitionAnimator.o: \
	Source/Components/TransitionAnimator.cpp
$(JUCE_OBJDIR)/ComponentConfigFile.o: \
	Source/Config/Component/ComponentConfigFile.cpp
$(JUCE_OBJDIR)/ComponentJSON.o: \
	Source/Config/Component/ComponentJSON.cpp
$(JUCE_OBJDIR)/ComponentSettings.o: \
	Source/Config/Component/ComponentSettings.cpp
$(JUCE_OBJDIR)/ColourConfigFile.o: \
	Source/Config/Colour/ColourConfigFile.cpp
$(JUCE_OBJDIR)/ColourConfigKeys.o: \
	Source/Config/Colour/ColourConfigKeys.cpp
$(JUCE_OBJDIR)/ColourJSON.o: \
	Source/Config/Colour/ColourJSON.cpp

# Config Module:
$(CONFIG_PREFIX)MainResource.o: \
    Source/Config/MainResource.cpp
$(CONFIG_PREFIX)FileResource.o: \
    Source/Config/FileResource.cpp
$(CONFIG_PREFIX)DataKey.o: \
    Source/Config/DataKey.cpp
$(CONFIG_PREFIX)AlertWindow.o: \
    Source/Config/AlertWindow.cpp

$(JUCE_OBJDIR)/JSONFile.o: \
	Source/Util/JSONFile.cpp
$(JUCE_OBJDIR)/XDGDirectories.o: \
	Source/FreeDesktop/XDGDirectories.cpp
$(JUCE_OBJDIR)/IconCache.o: \
	Source/FreeDesktop/Icon\ Themes/IconCache.cpp
$(JUCE_OBJDIR)/IconLoader.o: \
	Source/FreeDesktop/Icon\ Themes/IconLoader.cpp
$(JUCE_OBJDIR)/IconThemeIndex.o: \
	Source/FreeDesktop/Icon\ Themes/IconThemeIndex.cpp
$(JUCE_OBJDIR)/IconThread.o: \
	Source/FreeDesktop/Icon\ Themes/IconThread.cpp
$(JUCE_OBJDIR)/DesktopEntryUtils.o: \
	Source/FreeDesktop/Desktop\ Entries/DesktopEntryUtils.cpp
$(JUCE_OBJDIR)/DesktopEntryLoader.o: \
	Source/FreeDesktop/Desktop\ Entries/DesktopEntryLoader.cpp
$(JUCE_OBJDIR)/DesktopEntry.o: \
	Source/FreeDesktop/Desktop\ Entries/DesktopEntry.cpp
$(JUCE_OBJDIR)/GVariantConverter.o: \
	Source/GLib/DBus/GVariantConverter.cpp
$(JUCE_OBJDIR)/GPPDBusProxy.o: \
	Source/GLib/DBus/GPPDBusProxy.cpp
$(JUCE_OBJDIR)/SavedConnection.o: \
	Source/GLib/LibNM/SavedConnection.cpp
$(JUCE_OBJDIR)/SavedConnections.o: \
	Source/GLib/LibNM/SavedConnections.cpp
$(JUCE_OBJDIR)/NMPPAccessPoint.o: \
	Source/GLib/LibNM/NMPPAccessPoint.cpp
$(JUCE_OBJDIR)/NMPPActiveConnection.o: \
	Source/GLib/LibNM/NMPPActiveConnection.cpp
$(JUCE_OBJDIR)/NMPPClient.o: \
	Source/GLib/LibNM/NMPPClient.cpp
$(JUCE_OBJDIR)/NMPPConnection.o: \
	Source/GLib/LibNM/NMPPConnection.cpp
$(JUCE_OBJDIR)/NMPPDeviceWifi.o: \
	Source/GLib/LibNM/NMPPDeviceWifi.cpp
$(JUCE_OBJDIR)/GLibSignalThread.o: \
	Source/GLib/GLibSignalThread.cpp
$(JUCE_OBJDIR)/GLibThread.o: \
	Source/GLib/GLibThread.cpp
$(JUCE_OBJDIR)/GPPObject.o: \
	Source/GLib/GPPObject.cpp
$(JUCE_OBJDIR)/GPPWeakRef.o: \
	Source/GLib/GPPWeakRef.cpp
$(JUCE_OBJDIR)/GSignalHandler.o: \
	Source/GLib/GSignalHandler.cpp
$(JUCE_OBJDIR)/ThreadResource.o: \
	Source/Util/SharedResource/ThreadResource.cpp
$(JUCE_OBJDIR)/ScopedThreadWriteLock.o: \
	Source/Util/SharedResource/ScopedThreadWriteLock.cpp
$(JUCE_OBJDIR)/ScopedThreadReadLock.o: \
	Source/Util/SharedResource/ScopedThreadReadLock.cpp
$(JUCE_OBJDIR)/ThreadLock.o: \
	Source/Util/SharedResource/ThreadLock.cpp
$(JUCE_OBJDIR)/SharedResource.o: \
	Source/Util/SharedResource/SharedResource.cpp
$(JUCE_OBJDIR)/LaunchedProcess.o: \
	Source/Util/LaunchedProcess.cpp
$(JUCE_OBJDIR)/LocalizedTime.o: \
	Source/Util/LocalizedTime.cpp
$(JUCE_OBJDIR)/TempTimer.o: \
	Source/Util/TempTimer.cpp
$(JUCE_OBJDIR)/Utils.o: \
	Source/Util/Utils.cpp
$(JUCE_OBJDIR)/WindowFocus.o: \
	Source/Util/WindowFocus.cpp
$(JUCE_OBJDIR)/WindowFocusedTimer.o: \
	Source/Util/WindowFocusedTimer.cpp
$(JUCE_OBJDIR)/Localized.o: \
	Source/Util/Localized.cpp
$(JUCE_OBJDIR)/AppLauncher.o: \
	Source/Util/AppLauncher.cpp
$(JUCE_OBJDIR)/Main.o: \
	Source/Main.cpp
$(JUCE_OBJDIR)/PocketHomeApplication.o: \
	Source/PocketHomeApplication.cpp
$(JUCE_OBJDIR)/gtest_object.o: \
	Tests/GLib/gtest_object.cpp
$(JUCE_OBJDIR)/GPPTestObject.o: \
	Tests/GLib/GPPTestObject.cpp
$(JUCE_OBJDIR)/test_GPPObject.o: \
	Tests/GLib/test_GPPObject.cpp
$(JUCE_OBJDIR)/StressTest.o: \
	Tests/StressTest.cpp
$(JUCE_OBJDIR)/test_ConfigFile.o: \
	Tests/test_ConfigFile.cpp
$(JUCE_OBJDIR)/test_LaunchedApp.o: \
	Tests/test_LaunchedApp.cpp
$(JUCE_OBJDIR)/test_WifiStateManager.o: \
	Tests/test_WifiStateManager.cpp
$(JUCE_OBJDIR)/test_XWindowInterface.o: \
	Tests/test_XWindowInterface.cpp
$(JUCE_OBJDIR)/BinaryData.o: \
	JuceLibraryCode/BinaryData.cpp
$(JUCE_OBJDIR)/include_juce_core.o: \
	JuceLibraryCode/include_juce_core.cpp
$(JUCE_OBJDIR)/include_juce_data_structures.o: \
	JuceLibraryCode/include_juce_data_structures.cpp
$(JUCE_OBJDIR)/include_juce_events.o: \
	JuceLibraryCode/include_juce_events.cpp
$(JUCE_OBJDIR)/include_juce_graphics.o: \
	JuceLibraryCode/include_juce_graphics.cpp
$(JUCE_OBJDIR)/include_juce_gui_basics.o: \
	JuceLibraryCode/include_juce_gui_basics.cpp

$(OBJECTS_APP):
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Building $@:"
	@echo "   Compiling $<"
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
