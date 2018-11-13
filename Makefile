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
## Main build options:	                                                      ##
##	                                                                          ##
## CONFIG=Debug	         Build debug binaries skipping optimizations,         ##
##	                     including all test code, and enabling gdb            ##
##	                     debugging symbols.                                   ##
##	                                                                          ##
## CONFIG=Release	     Build release binaries using full optimization,      ##
##	                     removing all test code, and omitting gdb flags.      ##
##	                                                                          ##
## V=1	                 Enable verbose build output.                         ##
##	                                                                          ##
################################################################################
######### Build values: #########

# Default Options:
# Build type: either Debug or Release
CONFIG ?= Debug
# Command used to strip unneeded symbols from object files:
STRIP ?= strip
# Use the build system's architecture by default.
TARGET_ARCH ?= -march=native
# Enable or disable verbose output
V ?= 0
# Whether test classes will be built
BUILD_TESTS ?= 1

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

# Generate the build label from the /etc/os-release file:
BUILD_NAME := $(shell ./project-scripts/buildLabel.sh)

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
	CONFIG_CFLAGS  = -O3 -flto
	CONFIG_LDFLAGS =-fvisibility=hidden -flto
endif
  
JUCE_CPPFLAGS := $(DEPFLAGS) \
	             $(JUCE_CONFIG_FLAGS) \
	             $(JUCE_DEFS)\
                 -DBUILD_NAME="\"$(shell ./project-scripts/buildLabel.sh)\"" \
	             $(shell pkg-config --cflags $(PKG_CONFIG_LIBS)) \
	             $(DIR_FLAGS)

JUCE_CFLAGS   += $(JUCE_CPPFLAGS) \
				 $(TARGET_ARCH) \
				 $(CONFIG_CFLAGS) \
				 $(CFLAGS)

JUCE_CXXFLAGS += $(JUCE_CFLAGS) \
				 $(CXXFLAGS)

JUCE_LDFLAGS  += $(TARGET_ARCH) \
	             -L$(JUCE_BINDIR) \
	             -L$(JUCE_LIBDIR) \
	             $(shell pkg-config --libs $(PKG_CONFIG_LIBS)) \
	             $(CONFIG_LDFLAGS) \
	             $(LDFLAGS)

CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)

.PHONY: build devInstall debug release clean strip uninstall
build : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

# Juce Library Modules:
JUCE_PREFIX := $(JUCE_OBJDIR)/juce_
OBJECTS_JUCE := \
  $(JUCE_PREFIX)core.o \
  $(JUCE_PREFIX)data_structures.o \
  $(JUCE_PREFIX)events.o \
  $(JUCE_PREFIX)graphics.o \
  $(JUCE_PREFIX)gui_basics.o \
  $(JUCE_PREFIX)BinaryData.o 
juce : $(OBJECTS_JUCE)
	@echo "Built Juce library modules"

# Utility Module:
UTIL_PREFIX := $(JUCE_OBJDIR)/Util_
OBJECTS_UTIL := \
  $(UTIL_PREFIX)Utils.o \
  $(UTIL_PREFIX)TempTimer.o \
  $(UTIL_PREFIX)JSONFile.o \
  $(UTIL_PREFIX)XDGDirectories.o \
  $(UTIL_PREFIX)AssetFiles.o \
  $(UTIL_PREFIX)SharedResource.o \
  $(UTIL_PREFIX)ThreadResource.o \
  $(UTIL_PREFIX)WindowFocus.o \
  $(UTIL_PREFIX)WindowFocusedTimer.o 
OBJECTS_UTIL_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_UTIL := $(OBJECTS_UTIL) $(OBJECTS_UTIL_TEST)
endif
util : $(OBJECTS_UTIL)
	@echo "Built Utility module"

# Layout Module:
LAYOUT_PREFIX := $(JUCE_OBJDIR)/Layout_
OBJECTS_LAYOUT := \
  $(LAYOUT_PREFIX)Manager.o \
  $(LAYOUT_PREFIX)TransitionAnimator.o 
OBJECTS_LAYOUT_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LAYOUT := $(OBJECTS_LAYOUT) $(OBJECTS_LAYOUT_TEST)
endif
layout : $(OBJECTS_LAYOUT)
	@echo "Built Layout module"

# Config Module:
CONFIG_PREFIX := $(JUCE_OBJDIR)/Config_
OBJECTS_CONFIG := \
  $(CONFIG_PREFIX)MainResource.o \
  $(CONFIG_PREFIX)FileResource.o \
  $(CONFIG_PREFIX)DataKey.o \
  $(CONFIG_PREFIX)AlertWindow.o 
OBJECTS_CONFIG_TEST := \
  $(CONFIG_PREFIX)Test_FileResource.o \
  $(CONFIG_PREFIX)Test_FileHandler.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_CONFIG := $(OBJECTS_CONFIG) $(OBJECTS_CONFIG_TEST)
endif
config : $(OBJECTS_CONFIG)
	@echo "Built Config module"

# Locale Module:
LOCALE_PREFIX := $(JUCE_OBJDIR)/Locale_
OBJECTS_LOCALE := \
  $(LOCALE_PREFIX)Time.o \
  $(LOCALE_PREFIX)TextUser.o \
  $(LOCALE_PREFIX)Locale.o 
OBJECTS_LOCALE_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LOCALE := $(OBJECTS_LOCALE) $(OBJECTS_LOCALE_TEST)
endif
locale : $(OBJECTS_LOCALE)
	@echo "Built Locale module"

# Configurable Module:
CONFIGURABLE_PREFIX := $(JUCE_OBJDIR)/Configurable_
OBJECTS_CONFIGURABLE := \
  $(CONFIGURABLE_PREFIX)JSONResource.o \
  $(CONFIGURABLE_PREFIX)ConfigFile.o \
  $(CONFIGURABLE_PREFIX)Component.o \
  $(CONFIGURABLE_PREFIX)ComponentSettings.o \
  $(CONFIGURABLE_PREFIX)ImageButton.o \
  $(CONFIGURABLE_PREFIX)ImageComponent.o \
  $(CONFIGURABLE_PREFIX)Label.o 
OBJECTS_CONFIGURABLE_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_CONFIGURABLE := $(OBJECTS_CONFIGURABLE) $(OBJECTS_CONFIGURABLE_TEST)
endif
configurable : $(OBJECTS_CONFIGURABLE)
	@echo "Built Configurable module"

# Widget Module:
WIDGET_PREFIX := $(JUCE_OBJDIR)/Widget_
OBJECTS_WIDGET := \
  $(WIDGET_PREFIX)ScalingLabel.o \
  $(WIDGET_PREFIX)ScalingTextButton.o \
  $(WIDGET_PREFIX)DrawableImage.o \
  $(WIDGET_PREFIX)DrawableImageButton.o \
  $(WIDGET_PREFIX)PagedList.o \
  $(WIDGET_PREFIX)NavButton.o \
  $(WIDGET_PREFIX)IconSlider.o \
  $(WIDGET_PREFIX)ListEditor.o \
  $(WIDGET_PREFIX)Spinner.o \
  $(WIDGET_PREFIX)OverlaySpinner.o \
  $(WIDGET_PREFIX)Switch.o \
  $(WIDGET_PREFIX)ColourPicker.o \
  $(WIDGET_PREFIX)ClockLabel.o \
  $(WIDGET_PREFIX)PopupEditor.o \
  $(WIDGET_PREFIX)FileSelectTextEditor.o \
  $(WIDGET_PREFIX)Counter.o 
OBJECTS_WIDGET_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIDGET := $(OBJECTS_WIDGET) $(OBJECTS_WIDGET_TEST)
endif
widget : $(OBJECTS_WIDGET)
	@echo "Built Widget module"

# Page Module
PAGE_PREFIX := $(JUCE_OBJDIR)/Page_
OBJECTS_PAGE := \
  $(PAGE_PREFIX)Component.o \
  $(PAGE_PREFIX)FocusingList.o \
  $(PAGE_PREFIX)Factory.o \
  $(PAGE_PREFIX)Stack.o 
OBJECTS_PAGE_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PAGE := $(OBJECTS_PAGE) $(OBJECTS_PAGE_TEST)
endif
page : $(OBJECTS_PAGE)
	@echo "Built Page module"

# Theme Module:
THEME_PREFIX := $(JUCE_OBJDIR)/Theme_
OBJECTS_THEME := \
  $(THEME_PREFIX)ColourJSON.o \
  $(THEME_PREFIX)ColourConfigFile.o \
  $(THEME_PREFIX)ColourConfigKeys.o \
  $(THEME_PREFIX)ColourPage.o \
  $(THEME_PREFIX)LookAndFeel.o  
OBJECTS_THEME_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_THEME := $(OBJECTS_THEME) $(OBJECTS_THEME_TEST)
endif
theme : $(OBJECTS_THEME)
	@echo "Built Theme module"

# System Module:
SYSTEM_PREFIX := $(JUCE_OBJDIR)/System_
OBJECTS_SYSTEM := \
  $(SYSTEM_PREFIX)Commands.o \
  $(SYSTEM_PREFIX)XWindowInterface.o \
  $(SYSTEM_PREFIX)Audio.o \
  $(SYSTEM_PREFIX)Display.o \
  $(SYSTEM_PREFIX)I2CBus.o \
  $(SYSTEM_PREFIX)BatteryMonitor.o \
  $(SYSTEM_PREFIX)BatteryIcon.o \
  $(SYSTEM_PREFIX)PowerPage.o \
  $(SYSTEM_PREFIX)FelPage.o  
OBJECTS_SYSTEM_TEST := \
  $(SYSTEM_PREFIX)Test_XWindowInterface.o  
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SYSTEM := $(OBJECTS_SYSTEM) $(OBJECTS_SYSTEM_TEST)
endif
system : $(OBJECTS_SYSTEM)
	@echo "Built System module"

# Password Module:
PASSWORD_PREFIX := $(JUCE_OBJDIR)/Password_
OBJECTS_PASSWORD := \
  $(PASSWORD_PREFIX)Password.o \
  $(PASSWORD_PREFIX)RemovePage.o \
  $(PASSWORD_PREFIX)SetPage.o \
  $(PASSWORD_PREFIX)LoginPage.o 
OBJECTS_PASSWORD_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PASSWORD := $(OBJECTS_PASSWORD) $(OBJECTS_PASSWORD_TEST)
endif
password : $(OBJECTS_PASSWORD)
	@echo "Built Password module"

# Settings Module:
SETTINGS_PREFIX := $(JUCE_OBJDIR)/Settings_
OBJECTS_SETTINGS := \
  $(SETTINGS_PREFIX)QuickPage.o \
  $(SETTINGS_PREFIX)DateTimePage.o \
  $(SETTINGS_PREFIX)HomePage.o \
  $(SETTINGS_PREFIX)InputPage.o \
  $(SETTINGS_PREFIX)ConnectionComponent.o \
  $(SETTINGS_PREFIX)ListPage.o 
OBJECTS_SETTINGS_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SETTINGS := $(OBJECTS_SETTINGS) $(OBJECTS_SETTINGS_TEST)
endif
settings : $(OBJECTS_SETTINGS)
	@echo "Built Settings module"

# Icon Module:
ICON_PREFIX := $(JUCE_OBJDIR)/Icon_
OBJECTS_ICON := \
  $(ICON_PREFIX)IconCache.o \
  $(ICON_PREFIX)IconLoader.o \
  $(ICON_PREFIX)IconThemeIndex.o \
  $(ICON_PREFIX)IconThread.o 
OBJECTS_ICON_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_ICON := $(OBJECTS_ICON) $(OBJECTS_ICON_TEST)
endif
icon : $(OBJECTS_ICON)
	@echo "Built Icon module"

# DesktopEntry Module:
DESKTOP_ENTRY_PREFIX := $(JUCE_OBJDIR)/DesktopEntry_
OBJECTS_DESKTOP_ENTRY := \
  $(DESKTOP_ENTRY_PREFIX)FileUtils.o \
  $(DESKTOP_ENTRY_PREFIX)EntryFile.o \
  $(DESKTOP_ENTRY_PREFIX)LoadingThread.o \
  $(DESKTOP_ENTRY_PREFIX)UpdateListener.o \
  $(DESKTOP_ENTRY_PREFIX)Loader.o
OBJECTS_DESKTOP_ENTRY_TEST := \
  $(DESKTOP_ENTRY_PREFIX)Test_FileUtils.o \
  $(DESKTOP_ENTRY_PREFIX)Test_EntryFile.o \
  $(DESKTOP_ENTRY_PREFIX)Test_LoadingThread.o \
  $(DESKTOP_ENTRY_PREFIX)Test_Loader.o
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_DESKTOP_ENTRY := $(OBJECTS_DESKTOP_ENTRY) \
                             $(OBJECTS_DESKTOP_ENTRY_TEST)
endif
desktopEntry : $(OBJECTS_DESKTOP_ENTRY)
	@echo "Built DesktopEntry module"

# Process Module:
PROCESS_PREFIX := $(JUCE_OBJDIR)/Process_
OBJECTS_PROCESS := \
  $(CONFIG_PREFIX)Reader.o \
  $(CONFIG_PREFIX)Launched.o \
  $(CONFIG_PREFIX)Launcher.o  
OBJECTS_PROCESS_TEST := \
  $(CONFIG_PREFIX)Test_Launched.o
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PROCESS := $(OBJECTS_PROCESS) $(OBJECTS_PROCESS_TEST)
endif
process : $(OBJECTS_PROCESS)
	@echo "Built Process module"

# AppMenu Module:
APPMENU_PREFIX := $(JUCE_OBJDIR)/AppMenu_
OBJECTS_APPMENU_DATA := \
  $(APPMENU_PREFIX)JSONResource.o \
  $(APPMENU_PREFIX)ConfigFile.o \
  $(APPMENU_PREFIX)MenuItem.o \
  $(APPMENU_PREFIX)EntryData.o \
  $(APPMENU_PREFIX)EntryActions.o \
  $(APPMENU_PREFIX)EntryLoader.o \
  $(APPMENU_PREFIX)EntryUpdater.o \
  $(APPMENU_PREFIX)ConfigData.o \
  $(APPMENU_PREFIX)ItemData.o
OBJECTS_APPMENU_COMPONENT := \
  $(APPMENU_PREFIX)MenuButton.o \
  $(APPMENU_PREFIX)FolderComponent.o \
  $(APPMENU_PREFIX)MenuComponent.o \
  $(APPMENU_PREFIX)MainComponent.o 
OBJECTS_APPMENU_CONTROL := \
  $(APPMENU_PREFIX)Controller.o \
  $(APPMENU_PREFIX)InputHandler.o 
OBJECTS_APPMENU_EDITOR := \
  $(APPMENU_PREFIX)NewConfigItemEditor.o \
  $(APPMENU_PREFIX)NewDesktopAppEditor.o \
  $(APPMENU_PREFIX)ExistingItemEditor.o \
  $(APPMENU_PREFIX)CategoryEditor.o \
  $(APPMENU_PREFIX)PopupEditor.o
SCROLLING_MENU_PREFIX := $(APPMENU_PREFIX)Scrolling_
OBJECTS_APPMENU_SCROLLING := \
  $(SCROLLING_MENU_PREFIX)Initializer.o \
  $(SCROLLING_MENU_PREFIX)MenuButton.o \
  $(SCROLLING_MENU_PREFIX)FolderComponent.o \
  $(SCROLLING_MENU_PREFIX)MenuComponent.o \
  $(SCROLLING_MENU_PREFIX)InputHandler.o 
PAGED_MENU_PREFIX := $(APPMENU_PREFIX)Paged_
OBJECTS_APPMENU_PAGED := \
  $(PAGED_MENU_PREFIX)MenuButton.o \
  $(PAGED_MENU_PREFIX)FolderComponent.o \
  $(PAGED_MENU_PREFIX)MenuComponent.o \
  $(PAGED_MENU_PREFIX)InputHandler.o \
  $(PAGED_MENU_PREFIX)Initializer.o 
OBJECTS_APPMENU := \
  $(APPMENU_PREFIX)AppMenu.o \
  $(OBJECTS_APPMENU_DATA) \
  $(OBJECTS_APPMENU_COMPONENT) \
  $(OBJECTS_APPMENU_CONTROL) \
  $(OBJECTS_APPMENU_EDITOR) \
  $(OBJECTS_APPMENU_SCROLLING) \
  $(OBJECTS_APPMENU_PAGED)  
OBJECTS_APPMENU_TEST := \
  $(APPMENU_PREFIX)Test_AppMenu.o \
  $(APPMENU_PREFIX)Test_ConfigFile.o \
  $(APPMENU_PREFIX)Test_Controller.o \
  $(APPMENU_PREFIX)Test_InputHandler.o \
  $(APPMENU_PREFIX)Test_MenuComponent.o \
  $(APPMENU_PREFIX)Test_PagedMenu.o \
  $(APPMENU_PREFIX)Test_ScrollingMenu.o
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_APPMENU := $(OBJECTS_APPMENU) $(OBJECTS_APPMENU_TEST)
endif
appMenu : $(OBJECTS_APPMENU)
	@echo "Built AppMenu module"

# GLib Module:
GLIB_PREFIX := $(JUCE_OBJDIR)/GLib_
OBJECTS_GLIB := \
  $(GLIB_PREFIX)Object.o \
  $(GLIB_PREFIX)WeakRef.o \
  $(GLIB_PREFIX)Thread.o \
  $(GLIB_PREFIX)SignalThread.o \
  $(GLIB_PREFIX)SignalHandler.o \
  $(GLIB_PREFIX)VariantConverter.o \
  $(GLIB_PREFIX)DBusProxy.o  
OBJECTS_GLIB_TEST := \
  $(GLIB_PREFIX)Test_Object.o \
  $(GLIB_PREFIX)GPPTestObject.o \
  $(GLIB_PREFIX)gtest_object.o
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_GLIB := $(OBJECTS_GLIB) $(OBJECTS_GLIB_TEST)
endif
glib : $(OBJECTS_GLIB)
	@echo "Built GLib module"

# LibNM Module:
NM_PREFIX := $(JUCE_OBJDIR)/LibNM_
OBJECTS_NM := \
  $(NM_PREFIX)Client.o \
  $(NM_PREFIX)Connection.o \
  $(NM_PREFIX)ActiveConnection.o \
  $(NM_PREFIX)DeviceWifi.o \
  $(NM_PREFIX)AccessPoint.o \
  $(NM_PREFIX)SavedConnection.o \
  $(NM_PREFIX)SavedConnectionLoader.o  
OBJECTS_NM_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_NM := $(OBJECTS_NM) $(OBJECTS_NM_TEST)
endif
libNM : $(OBJECTS_NM)
	@echo "Built LibNM module"

# Wifi Module:
WIFI_PREFIX := $(JUCE_OBJDIR)/Wifi_
OBJECTS_WIFI := \
  $(WIFI_PREFIX)StateManager.o \
  $(WIFI_PREFIX)AccessPoint.o \
  $(WIFI_PREFIX)NetworkInterface.o \
  $(WIFI_PREFIX)LibNMInterface.o \
  $(WIFI_PREFIX)JsonInterface.o \
  $(WIFI_PREFIX)SettingsComponent.o \
  $(WIFI_PREFIX)SettingsPage.o \
  $(WIFI_PREFIX)Icon.o
OBJECTS_WIFI_TEST := \
  $(WIFI_PREFIX)Test_StateManager.o
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIFI := $(OBJECTS_WIFI) $(OBJECTS_WIFI_TEST)
endif
wifi : $(OBJECTS_WIFI)
	@echo "Built Wifi module"

# Bluetooth Module:
BLUETOOTH_PREFIX := $(JUCE_OBJDIR)/Bluetooth_
OBJECTS_BLUETOOTH := \
  $(BLUETOOTH_PREFIX)BluezAdapter.o \
  $(BLUETOOTH_PREFIX)Status.o \
  $(BLUETOOTH_PREFIX)Device.o \
  $(BLUETOOTH_PREFIX)SettingsComponent.o \
  $(BLUETOOTH_PREFIX)SettingsPage.o 
OBJECTS_BLUETOOTH_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_BLUETOOTH := $(OBJECTS_BLUETOOTH) $(OBJECTS_BLUETOOTH_TEST)
endif
bluetooth : $(OBJECTS_BLUETOOTH)
	@echo "Built Bluetooth module"

OBJECTS_MAIN := \
  $(JUCE_OBJDIR)/Main.o \
  $(JUCE_OBJDIR)/PocketHomeApplication.o \
  $(JUCE_OBJDIR)/PocketHomeWindow.o \
  $(JUCE_OBJDIR)/HomePage.o 
OBJECTS_MAIN_TEST :=
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_MAIN := $(OBJECTS_MAIN) $(OBJECTS_MAIN_TEST)
endif
main : $(OBJECTS_MAIN)
	@echo "Built pocket-home"

# Testing Module:
TEST_PREFIX := $(JUCE_OBJDIR)/Test_
OBJECTS_TEST := \
  $(TEST_PREFIX)StressTest.o \
  $(TEST_PREFIX)TestWindow.o \
  $(TEST_PREFIX)DelayUtils.o
testing : $(OBJECTS_TEST)
	@echo "Built Testing module"

MODULES := juce util layout config locale configurable widget page \
           theme system password settings icon desktopEntry process appMenu \
           glib libNM wifi bluetooth main
ifeq ($(BUILD_TESTS), 1)
    MODULES := $(MODULES) testing
endif

OBJECTS_APP := \
  $(OBJECTS_JUCE) \
  $(OBJECTS_UTIL) \
  $(OBJECTS_LAYOUT) \
  $(OBJECTS_CONFIG) \
  $(OBJECTS_LOCALE) \
  $(OBJECTS_CONFIGURABLE) \
  $(OBJECTS_WIDGET) \
  $(OBJECTS_PAGE) \
  $(OBJECTS_THEME) \
  $(OBJECTS_SYSTEM) \
  $(OBJECTS_PASSWORD) \
  $(OBJECTS_SETTINGS) \
  $(OBJECTS_ICON) \
  $(OBJECTS_DESKTOP_ENTRY) \
  $(OBJECTS_PROCESS) \
  $(OBJECTS_APPMENU) \
  $(OBJECTS_GLIB) \
  $(OBJECTS_NM) \
  $(OBJECTS_WIFI) \
  $(OBJECTS_BLUETOOTH) \
  $(OBJECTS_MAIN)
ifeq ($(BUILD_TESTS), 1)
    OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_TEST)
endif

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(MODULES) $(RESOURCES)
	@echo Linking "pocket-home - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) \
		             $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) \
					 $(TARGET_ARCH)

$(OBJECTS_APP) :
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "   Building $(@F):"
	@echo "      Compiling $<"
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
	$(MAKE) devinstall CONFIG=Debug
	gdb $(JUCE_TARGET_APP)

release:
	$(MAKE) CONFIG=Release
	reset
	$(MAKE) devinstall CONFIG=Release

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

$(JUCE_OBJDIR)/Main.o: \
	Source/Main.cpp
$(JUCE_OBJDIR)/PocketHomeApplication.o: \
	Source/PocketHomeApplication.cpp
$(JUCE_OBJDIR)/PocketHomeWindow.o: \
	Source/PocketHomeWindow.cpp
$(JUCE_OBJDIR)/HomePage.o: \
	Source/HomePage.cpp

#Juce Library Modules:
$(JUCE_PREFIX)BinaryData.o: \
	JuceLibraryCode/BinaryData.cpp
$(JUCE_PREFIX)core.o: \
	JuceLibraryCode/include_juce_core.cpp
$(JUCE_PREFIX)data_structures.o: \
	JuceLibraryCode/include_juce_data_structures.cpp
$(JUCE_PREFIX)events.o: \
	JuceLibraryCode/include_juce_events.cpp
$(JUCE_PREFIX)graphics.o: \
	JuceLibraryCode/include_juce_graphics.cpp
$(JUCE_PREFIX)gui_basics.o: \
	JuceLibraryCode/include_juce_gui_basics.cpp

# Utility Module:
$(UTIL_PREFIX)Utils.o : \
    Source/Util/Utils.cpp
$(UTIL_PREFIX)TempTimer.o : \
    Source/Util/TempTimer.cpp
$(UTIL_PREFIX)JSONFile.o : \
    Source/Util/Files/JSONFile.cpp
$(UTIL_PREFIX)XDGDirectories.o : \
    Source/Util/Files/XDGDirectories.cpp
$(UTIL_PREFIX)AssetFiles.o : \
    Source/Util/Files/AssetFiles.cpp
$(UTIL_PREFIX)SharedResource.o : \
    Source/Util/SharedResource/SharedResource.cpp
$(UTIL_PREFIX)ThreadResource.o : \
    Source/Util/SharedResource/ThreadResource.cpp
$(UTIL_PREFIX)WindowFocus.o : \
    Source/Util/WindowFocus/WindowFocus.cpp
$(UTIL_PREFIX)WindowFocusedTimer.o : \
    Source/Util/WindowFocus/WindowFocusedTimer.cpp

# Layout Module:
$(LAYOUT_PREFIX)Manager.o : \
    Source/Layout/LayoutManager.cpp
$(LAYOUT_PREFIX)TransitionAnimator.o : \
    Source/Layout/TransitionAnimator.cpp

# Config Module:
$(CONFIG_PREFIX)MainResource.o: \
    Source/Config/MainResource.cpp
$(CONFIG_PREFIX)FileResource.o: \
    Source/Config/FileResource.cpp
$(CONFIG_PREFIX)DataKey.o: \
    Source/Config/DataKey.cpp
$(CONFIG_PREFIX)AlertWindow.o: \
    Source/Config/AlertWindow.cpp
# Tests:
$(CONFIG_PREFIX)Test_FileResource.o: \
    Tests/Config/FileResource.cpp
$(CONFIG_PREFIX)Test_FileHandler.o: \
    Tests/Config/FileHandler.cpp


# Locale Module:
$(LOCALE_PREFIX)Time.o: \
	Source/Locale/Time.cpp
$(LOCALE_PREFIX)TextUser.o: \
	Source/Locale/TextUser.cpp
$(LOCALE_PREFIX)Locale.o: \
	Source/Locale/Locale.cpp

# Configurable Module:
$(CONFIGURABLE_PREFIX)JSONResource.o : \
    Source/Configurable/ComponentJSON.cpp
$(CONFIGURABLE_PREFIX)ConfigFile.o : \
    Source/Configurable/ComponentConfigFile.cpp
$(CONFIGURABLE_PREFIX)ComponentSettings.o : \
    Source/Configurable/ComponentSettings.cpp
$(CONFIGURABLE_PREFIX)Component.o : \
    Source/Configurable/ConfigurableComponent.cpp
$(CONFIGURABLE_PREFIX)ImageButton.o : \
    Source/Configurable/ConfigurableImageButton.cpp
$(CONFIGURABLE_PREFIX)ImageComponent.o : \
    Source/Configurable/ConfigurableImageComponent.cpp
$(CONFIGURABLE_PREFIX)Label.o : \
    Source/Configurable/ConfigurableLabel.cpp

# Widget Module:
$(WIDGET_PREFIX)ScalingLabel.o : \
    Source/Widgets/ScalingLabel.cpp
$(WIDGET_PREFIX)ScalingTextButton.o : \
    Source/Widgets/ScalingTextButton.cpp
$(WIDGET_PREFIX)DrawableImage.o : \
    Source/Widgets/DrawableImageComponent.cpp
$(WIDGET_PREFIX)DrawableImageButton.o : \
    Source/Widgets/DrawableImageButton.cpp
$(WIDGET_PREFIX)PagedList.o : \
    Source/Widgets/PagedList.cpp
$(WIDGET_PREFIX)NavButton.o : \
    Source/Widgets/NavButton.cpp
$(WIDGET_PREFIX)IconSlider.o : \
    Source/Widgets/IconSliderComponent.cpp
$(WIDGET_PREFIX)ListEditor.o : \
    Source/Widgets/ListEditor.cpp
$(WIDGET_PREFIX)Spinner.o : \
    Source/Widgets/Spinner.cpp
$(WIDGET_PREFIX)OverlaySpinner.o : \
    Source/Widgets/OverlaySpinner.cpp
$(WIDGET_PREFIX)Switch.o : \
    Source/Widgets/SwitchComponent.cpp
$(WIDGET_PREFIX)ColourPicker.o : \
    Source/Widgets/ColourPicker.cpp
$(WIDGET_PREFIX)ClockLabel.o : \
    Source/Widgets/ClockLabel.cpp
$(WIDGET_PREFIX)PopupEditor.o : \
    Source/Widgets/PopupEditorComponent.cpp
$(WIDGET_PREFIX)FileSelectTextEditor.o : \
    Source/Widgets/FileSelectTextEditor.cpp
$(WIDGET_PREFIX)Counter.o : \
    Source/Widgets/CounterComponent.cpp

# Page Module
$(PAGE_PREFIX)Component.o : \
    Source/Page/PageComponent.cpp
$(PAGE_PREFIX)FocusingList.o : \
    Source/Page/FocusingListPage.cpp
$(PAGE_PREFIX)Factory.o : \
    Source/Page/PageFactory.cpp
$(PAGE_PREFIX)Stack.o : \
    Source/Page/PageStackComponent.cpp

# Theme Module:
$(THEME_PREFIX)ColourJSON.o : \
    Source/Theme/ColourJSON.cpp
$(THEME_PREFIX)ColourConfigFile.o : \
    Source/Theme/ColourConfigFile.cpp
$(THEME_PREFIX)ColourConfigKeys.o : \
    Source/Theme/ColourConfigKeys.cpp
$(THEME_PREFIX)ColourPage.o : \
    Source/Theme/ColourPage.cpp
$(THEME_PREFIX)LookAndFeel.o : \
    Source/Theme/PokeLookAndFeel.cpp

# System Module:
$(SYSTEM_PREFIX)Commands.o : \
    Source/System/SystemCommands.cpp
$(SYSTEM_PREFIX)XWindowInterface.o : \
    Source/System/XWindowInterface.cpp
$(SYSTEM_PREFIX)Audio.o : \
    Source/System/Audio.cpp
$(SYSTEM_PREFIX)Display.o : \
    Source/System/Display.cpp
$(SYSTEM_PREFIX)I2CBus.o : \
    Source/System/I2CBus.cpp
$(SYSTEM_PREFIX)BatteryMonitor.o : \
    Source/System/BatteryMonitor.cpp
$(SYSTEM_PREFIX)BatteryIcon.o : \
    Source/System/BatteryIcon.cpp
$(SYSTEM_PREFIX)PowerPage.o : \
    Source/System/PowerPage/PowerPage.cpp
$(SYSTEM_PREFIX)FelPage.o : \
    Source/System/PowerPage/FelPage.cpp
# Tests:
$(SYSTEM_PREFIX)Test_XWindowInterface.o : \
    Tests/System/XWindowInterface.cpp

# Password Module:
$(PASSWORD_PREFIX)Password.o : \
    Source/Password/Password.cpp
$(PASSWORD_PREFIX)RemovePage.o : \
    Source/Password/PasswordPages/RemovePasswordPage.cpp
$(PASSWORD_PREFIX)SetPage.o : \
    Source/Password/PasswordPages/SetPasswordPage.cpp
$(PASSWORD_PREFIX)LoginPage.o : \
    Source/Password/LoginPage/LoginPage.cpp

# Settings Module:
$(SETTINGS_PREFIX)QuickPage.o : \
    Source/Settings/QuickSettingsPage/QuickSettingsPage.cpp
$(SETTINGS_PREFIX)DateTimePage.o : \
    Source/Settings/DateTimePage/DateTimePage.cpp
$(SETTINGS_PREFIX)HomePage.o : \
    Source/Settings/HomeSettingsPage/HomeSettingsPage.cpp
$(SETTINGS_PREFIX)InputPage.o : \
    Source/Settings/InputSettingsPage/InputSettingsPage.cpp
$(SETTINGS_PREFIX)ConnectionComponent.o : \
    Source/Settings/QuickSettingsPage/ConnectionSettingsComponent.cpp
$(SETTINGS_PREFIX)ListPage.o : \
    Source/Settings/SettingsListPage/SettingsListPage.cpp

# Icon Module:
$(ICON_PREFIX)IconCache.o: \
	Source/Icon/IconCache.cpp
$(ICON_PREFIX)IconLoader.o: \
	Source/Icon/IconLoader.cpp
$(ICON_PREFIX)IconThemeIndex.o: \
	Source/Icon/IconThemeIndex.cpp
$(ICON_PREFIX)IconThread.o: \
	Source/Icon/IconThread.cpp

# DesktopEntry Module:
$(DESKTOP_ENTRY_PREFIX)FileUtils.o: \
    Source/DesktopEntry/FileUtils.cpp
$(DESKTOP_ENTRY_PREFIX)EntryFile.o: \
	Source/DesktopEntry/EntryFile.cpp
$(DESKTOP_ENTRY_PREFIX)LoadingThread.o: \
	Source/DesktopEntry/LoadingThread.cpp
$(DESKTOP_ENTRY_PREFIX)UpdateListener.o : \
	Source/DesktopEntry/UpdateListener.cpp
$(DESKTOP_ENTRY_PREFIX)Loader.o: \
	Source/DesktopEntry/Loader.cpp
# Tests:
$(DESKTOP_ENTRY_PREFIX)Test_FileUtils.o: \
    Tests/DesktopEntry/FileUtils.cpp
$(DESKTOP_ENTRY_PREFIX)Test_EntryFile.o: \
	Tests/DesktopEntry/EntryFile.cpp
$(DESKTOP_ENTRY_PREFIX)Test_LoadingThread.o: \
	Tests/DesktopEntry/LoadingThread.cpp
$(DESKTOP_ENTRY_PREFIX)Test_Loader.o: \
	Tests/DesktopEntry/Loader.cpp

# Process Module:
$(CONFIG_PREFIX)Reader.o : \
    Source/Process/ProcessUtils.cpp
$(CONFIG_PREFIX)Launched.o : \
    Source/Process/LaunchedProcess.cpp
$(CONFIG_PREFIX)Launcher.o : \
    Source/Process/AppLauncher.cpp
$(CONFIG_PREFIX)Launched.o : \
    Source/Process/LaunchedProcess.cpp
# Tests:
$(CONFIG_PREFIX)Test_Launched.o : \
    Tests/Process/LaunchedProcess.cpp

# AppMenu Module
$(SCROLLING_MENU_PREFIX)MenuButton.o: \
	Source/AppMenu/Formats/Scrolling/MenuButton.cpp
$(SCROLLING_MENU_PREFIX)FolderComponent.o: \
	Source/AppMenu/Formats/Scrolling/FolderComponent.cpp
$(SCROLLING_MENU_PREFIX)MenuComponent.o: \
	Source/AppMenu/Formats/Scrolling/MenuComponent.cpp
$(SCROLLING_MENU_PREFIX)InputHandler.o: \
	Source/AppMenu/Formats/Scrolling/InputHandler.cpp
$(SCROLLING_MENU_PREFIX)Initializer.o: \
	Source/AppMenu/Formats/Scrolling/Initializer.cpp
$(PAGED_MENU_PREFIX)MenuButton.o: \
	Source/AppMenu/Formats/Paged/MenuButton.cpp
$(PAGED_MENU_PREFIX)FolderComponent.o: \
	Source/AppMenu/Formats/Paged/FolderComponent.cpp
$(PAGED_MENU_PREFIX)MenuComponent.o: \
	Source/AppMenu/Formats/Paged/MenuComponent.cpp
$(PAGED_MENU_PREFIX)InputHandler.o: \
	Source/AppMenu/Formats/Paged/InputHandler.cpp
$(PAGED_MENU_PREFIX)Initializer.o: \
	Source/AppMenu/Formats/Paged/Initializer.cpp
$(APPMENU_PREFIX)MenuButton.o: \
    Source/AppMenu/Components/MenuButton.cpp
$(APPMENU_PREFIX)FolderComponent.o: \
    Source/AppMenu/Components/FolderComponent.cpp
$(APPMENU_PREFIX)MenuComponent.o: \
    Source/AppMenu/Components/MenuComponent.cpp
$(APPMENU_PREFIX)MainComponent.o: \
    Source/AppMenu/Components/MainComponent.cpp
$(APPMENU_PREFIX)Controller.o: \
    Source/AppMenu/Control/Controller.cpp
$(APPMENU_PREFIX)InputHandler.o: \
    Source/AppMenu/Control/InputHandler.cpp
$(APPMENU_PREFIX)NewConfigItemEditor.o: \
    Source/AppMenu/Components/Editors/NewConfigItemEditor.cpp
$(APPMENU_PREFIX)NewDesktopAppEditor.o: \
    Source/AppMenu/Components/Editors/NewDesktopAppEditor.cpp
$(APPMENU_PREFIX)ExistingItemEditor.o: \
    Source/AppMenu/Components/Editors/ExistingItemEditor.cpp
$(APPMENU_PREFIX)PopupEditor.o: \
    Source/AppMenu/Components/Editors/PopupEditor.cpp
$(APPMENU_PREFIX)CategoryEditor.o: \
    Source/AppMenu/Components/Editors/CategoryEditor.cpp
$(APPMENU_PREFIX)JSONResource.o: \
    Source/AppMenu/Data/JSON/JSONResource.cpp
$(APPMENU_PREFIX)ConfigFile.o: \
    Source/AppMenu/Data/JSON/ConfigFile.cpp
$(APPMENU_PREFIX)MenuItem.o: \
    Source/AppMenu/Data/MenuItem.cpp
$(APPMENU_PREFIX)EntryData.o: \
    Source/AppMenu/Data/DesktopEntry/EntryData.cpp
$(APPMENU_PREFIX)EntryActions.o: \
    Source/AppMenu/Data/DesktopEntry/EntryActions.cpp
$(APPMENU_PREFIX)EntryLoader.o: \
    Source/AppMenu/Data/DesktopEntry/EntryLoader.cpp
$(APPMENU_PREFIX)EntryUpdater.o: \
    Source/AppMenu/Data/DesktopEntry/EntryUpdater.cpp
$(APPMENU_PREFIX)ConfigData.o: \
    Source/AppMenu/Data/JSON/ConfigData.cpp
$(APPMENU_PREFIX)ItemData.o: \
    Source/AppMenu/Data/ItemData.cpp
$(APPMENU_PREFIX)AppMenu.o: \
    Source/AppMenu/AppMenu.cpp
# Tests:
$(APPMENU_PREFIX)Test_AppMenu.o: \
    Tests/AppMenu/AppMenu.cpp
$(APPMENU_PREFIX)Test_ConfigFile.o: \
    Tests/AppMenu/ConfigFile.cpp
$(APPMENU_PREFIX)Test_Controller.o: \
    Tests/AppMenu/Controller.cpp
$(APPMENU_PREFIX)Test_InputHandler.o: \
    Tests/AppMenu/InputHandler.cpp
$(APPMENU_PREFIX)Test_MenuComponent.o: \
    Tests/AppMenu/MenuComponent.cpp
$(APPMENU_PREFIX)Test_PagedMenu.o: \
    Tests/AppMenu/PagedMenu.cpp
$(APPMENU_PREFIX)Test_ScrollingMenu.o: \
    Tests/AppMenu/ScrollingMenu.cpp

# GLib Module:
$(GLIB_PREFIX)Object.o : \
    Source/GLib/GPPObject.cpp
$(GLIB_PREFIX)WeakRef.o : \
    Source/GLib/GPPWeakRef.cpp
$(GLIB_PREFIX)Thread.o : \
    Source/GLib/GLibThread.cpp
$(GLIB_PREFIX)SignalThread.o : \
    Source/GLib/GLibSignalThread.cpp
$(GLIB_PREFIX)SignalHandler.o : \
    Source/GLib/GSignalHandler.cpp
$(GLIB_PREFIX)VariantConverter.o : \
    Source/GLib/DBus/GVariantConverter.cpp
$(GLIB_PREFIX)DBusProxy.o : \
    Source/GLib/DBus/GPPDBusProxy.cpp
# Tests
$(GLIB_PREFIX)Test_Object.o : \
    Tests/GLib/GPPObject.cpp
$(GLIB_PREFIX)GPPTestObject.o : \
    Tests/GLib/GPPTestObject.cpp
$(GLIB_PREFIX)gtest_object.o : \
    Tests/GLib/gtest_object.cpp

# LibNM Module:
$(NM_PREFIX)Client.o : \
    Source/LibNM/NMPPClient.cpp
$(NM_PREFIX)Connection.o : \
    Source/LibNM/NMPPConnection.cpp
$(NM_PREFIX)ActiveConnection.o : \
    Source/LibNM/NMPPActiveConnection.cpp
$(NM_PREFIX)DeviceWifi.o : \
    Source/LibNM/NMPPDeviceWifi.cpp
$(NM_PREFIX)AccessPoint.o : \
    Source/LibNM/NMPPAccessPoint.cpp
$(NM_PREFIX)SavedConnection.o : \
    Source/LibNM/SavedConnection.cpp
$(NM_PREFIX)SavedConnectionLoader.o : \
    Source/LibNM/SavedConnections.cpp

# Wifi Module:
$(WIFI_PREFIX)StateManager.o : \
    Source/Wifi/WifiStateManager.cpp
$(WIFI_PREFIX)AccessPoint.o : \
    Source/Wifi/WifiAccessPoint.cpp
$(WIFI_PREFIX)NetworkInterface.o : \
    Source/Wifi/NetworkInterface.cpp
$(WIFI_PREFIX)LibNMInterface.o : \
    Source/Wifi/LibNMInterface.cpp
$(WIFI_PREFIX)JsonInterface.o : \
    Source/Wifi/JsonWifiInterface.cpp
$(WIFI_PREFIX)SettingsComponent.o : \
    Source/Wifi/WifiSettingsComponent.cpp
$(WIFI_PREFIX)SettingsPage.o : \
    Source/Wifi/WifiSettingsPage.cpp
$(WIFI_PREFIX)Icon.o : \
    Source/Wifi/WifiIcon.cpp
# Tests:
$(WIFI_PREFIX)Test_StateManager.o : \
    Tests/Wifi/WifiStateManager.cpp

# Bluetooth Module:
$(BLUETOOTH_PREFIX)BluezAdapter.o : \
    Source/Bluetooth/BluezAdapter.cpp
$(BLUETOOTH_PREFIX)Status.o : \
    Source/Bluetooth/BluetoothStatus.cpp
$(BLUETOOTH_PREFIX)Device.o : \
    Source/Bluetooth/BluetoothDevice.cpp
$(BLUETOOTH_PREFIX)SettingsComponent.o : \
    Source/Bluetooth/BluetoothSettingsComponent.cpp
$(BLUETOOTH_PREFIX)SettingsPage.o : \
    Source/Bluetooth/BluetoothSettingsPage.cpp

# Testing Module:
$(TEST_PREFIX)StressTest.o: \
	Tests/StressTest.cpp
$(TEST_PREFIX)TestWindow.o: \
	Tests/TestWindow.cpp
$(TEST_PREFIX)DelayUtils.o: \
	Tests/DelayUtils.cpp
