# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := ../../build/Debug

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.1 -DJUCE_APP_VERSION_HEX=0x1 $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Wifi -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components/Pages -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components/Basic $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl) -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := ../../build/Release

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.1 -DJUCE_APP_VERSION_HEX=0x1 $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Wifi -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components/Pages -I/home/anthony/Workspace/C++/PocketCHIP-pocket-home/Source/Components/Basic $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++11 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl) -fvisibility=hidden -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/WifiStatus_22bcd2a2.o \
  $(JUCE_OBJDIR)/WifiStatusJson_a638b12a.o \
  $(JUCE_OBJDIR)/WifiStatusNM_9c199da1.o \
  $(JUCE_OBJDIR)/Grid_2148acc1.o \
  $(JUCE_OBJDIR)/IconSliderComponent_b915be12.o \
  $(JUCE_OBJDIR)/MenuItem_616a458d.o \
  $(JUCE_OBJDIR)/OverlaySpinner_d95ac424.o \
  $(JUCE_OBJDIR)/SwitchComponent_5fb00cd8.o \
  $(JUCE_OBJDIR)/VectorImageButton_f6208d9.o \
  $(JUCE_OBJDIR)/AdvancedSettingsPage_201ada39.o \
  $(JUCE_OBJDIR)/AppMenuPage_19373814.o \
  $(JUCE_OBJDIR)/DateTimePage_51571d6f.o \
  $(JUCE_OBJDIR)/InputSettingsPage_3bd27201.o \
  $(JUCE_OBJDIR)/LauncherComponent_e925b482.o \
  $(JUCE_OBJDIR)/PageStackComponent_e433769.o \
  $(JUCE_OBJDIR)/PersonalizePageComponent_2cb66d1f.o \
  $(JUCE_OBJDIR)/PowerPageComponent_cac0234e.o \
  $(JUCE_OBJDIR)/PowerPageFelComponent_20226b09.o \
  $(JUCE_OBJDIR)/SettingsPageComponent_4769c410.o \
  $(JUCE_OBJDIR)/AppMenuButton_1edbbd22.o \
  $(JUCE_OBJDIR)/AppMenuComponent_5288ff17.o \
  $(JUCE_OBJDIR)/MainComponent_ec7554f4.o \
  $(JUCE_OBJDIR)/SettingsPageBluetoothComponent_69dfe57b.o \
  $(JUCE_OBJDIR)/SettingsPageLogin_9f7bcfe7.o \
  $(JUCE_OBJDIR)/SettingsPageWifiComponent_4c8b2566.o \
  $(JUCE_OBJDIR)/ConfigFile_cd42f267.o \
  $(JUCE_OBJDIR)/DesktopEntries_5218f65d.o \
  $(JUCE_OBJDIR)/DesktopEntry_8dc99f3f.o \
  $(JUCE_OBJDIR)/ClockMonitor_d7cf07f5.o \
  $(JUCE_OBJDIR)/BatteryMonitor_37302b96.o \
  $(JUCE_OBJDIR)/Utils_e4b11b92.o \
  $(JUCE_OBJDIR)/PokeLookAndFeel_55885106.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/include_juce_core_f26d17db.o \
  $(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o \
  $(JUCE_OBJDIR)/include_juce_events_fd7d695.o \
  $(JUCE_OBJDIR)/include_juce_graphics_f817e147.o \
  $(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o \

.PHONY: clean all

all : $(JUCE_OUTDIR)/$(JUCE_TARGET_APP)

$(JUCE_OUTDIR)/$(JUCE_TARGET_APP) : check-pkg-config $(OBJECTS_APP) $(RESOURCES)
	@echo Linking "pocket-home - App"
	-$(V_AT)mkdir -p $(JUCE_BINDIR)
	-$(V_AT)mkdir -p $(JUCE_LIBDIR)
	-$(V_AT)mkdir -p $(JUCE_OUTDIR)
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/WifiStatus_22bcd2a2.o: ../../Source/Wifi/WifiStatus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStatus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiStatusJson_a638b12a.o: ../../Source/Wifi/WifiStatusJson.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStatusJson.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiStatusNM_9c199da1.o: ../../Source/Wifi/WifiStatusNM.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStatusNM.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Grid_2148acc1.o: ../../Source/Components/Basic/Grid.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Grid.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconSliderComponent_b915be12.o: ../../Source/Components/Basic/IconSliderComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconSliderComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MenuItem_616a458d.o: ../../Source/Components/Basic/MenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OverlaySpinner_d95ac424.o: ../../Source/Components/Basic/OverlaySpinner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OverlaySpinner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SwitchComponent_5fb00cd8.o: ../../Source/Components/Basic/SwitchComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SwitchComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/VectorImageButton_f6208d9.o: ../../Source/Components/Basic/VectorImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling VectorImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AdvancedSettingsPage_201ada39.o: ../../Source/Components/Pages/AdvancedSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AdvancedSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuPage_19373814.o: ../../Source/Components/Pages/AppMenuPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DateTimePage_51571d6f.o: ../../Source/Components/Pages/DateTimePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DateTimePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputSettingsPage_3bd27201.o: ../../Source/Components/Pages/InputSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LauncherComponent_e925b482.o: ../../Source/Components/Pages/LauncherComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LauncherComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageStackComponent_e433769.o: ../../Source/Components/Pages/PageStackComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageStackComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PersonalizePageComponent_2cb66d1f.o: ../../Source/Components/Pages/PersonalizePageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PersonalizePageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PowerPageComponent_cac0234e.o: ../../Source/Components/Pages/PowerPageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PowerPageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PowerPageFelComponent_20226b09.o: ../../Source/Components/Pages/PowerPageFelComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PowerPageFelComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsPageComponent_4769c410.o: ../../Source/Components/Pages/SettingsPageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsPageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuButton_1edbbd22.o: ../../Source/Components/AppMenuButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuComponent_5288ff17.o: ../../Source/Components/AppMenuComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_ec7554f4.o: ../../Source/Components/MainComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsPageBluetoothComponent_69dfe57b.o: ../../Source/Components/SettingsPageBluetoothComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsPageBluetoothComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsPageLogin_9f7bcfe7.o: ../../Source/Components/SettingsPageLogin.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsPageLogin.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsPageWifiComponent_4c8b2566.o: ../../Source/Components/SettingsPageWifiComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsPageWifiComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigFile_cd42f267.o: ../../Source/ConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntries_5218f65d.o: ../../Source/DesktopEntries.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntries.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntry_8dc99f3f.o: ../../Source/DesktopEntry.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntry.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ClockMonitor_d7cf07f5.o: ../../Source/ClockMonitor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ClockMonitor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryMonitor_37302b96.o: ../../Source/BatteryMonitor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryMonitor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Utils_e4b11b92.o: ../../Source/Utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PokeLookAndFeel_55885106.o: ../../Source/PokeLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PokeLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_core_f26d17db.o: ../../JuceLibraryCode/include_juce_core.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_core.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_data_structures_7471b1e3.o: ../../JuceLibraryCode/include_juce_data_structures.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_data_structures.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_events_fd7d695.o: ../../JuceLibraryCode/include_juce_events.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_events.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_graphics_f817e147.o: ../../JuceLibraryCode/include_juce_graphics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_graphics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/include_juce_gui_basics_e3f79785.o: ../../JuceLibraryCode/include_juce_gui_basics.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling include_juce_gui_basics.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors NetworkManager libnm-glib alsa freetype2 libssl

clean:
	@echo Cleaning pocket-home
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping pocket-home
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
