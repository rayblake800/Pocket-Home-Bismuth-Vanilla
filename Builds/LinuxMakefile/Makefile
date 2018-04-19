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

JUCE_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := ../../build/Debug

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.1 -DJUCE_APP_VERSION_HEX=0x1 $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I../../Source/Components/Pages/Power\ Page -I../../Source/Shared\ Resources/Config\ Files -I../../Source/Hardware/Bluetooth -I../../Source/Components/Pages/Home\ Page/AppMenu -I../../Source/Components/Pages/Login\ Page -I../../Source/Hardware/Wifi -I../../Source/Components/Pages/UI\ Page -I../../Source/Components/Pages -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Types -I../../Source/Components/Pages/Settings\ Page -I../../Source/Components/Pages/Password\ Pages -I../../Source/Components/Widgets -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Folders -I../../Source/Components/Pages/Home\ Page -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items -I../../Source/Shared\ Resources -I../../Source/Components/Pages/Advanced\ Settings\ Page -I../../Source -I../../Source/Hardware -I../../Source/Components/Pages/Home\ Settings\ Page -I../../Source/Components -I../../Source/Components/Pages/DateTime\ Page -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors -I../../Source/Components/Configurable -I../../Source/Components/Pages/Input\ Settings\ Page -I../../Source/Util -I../../Source/Components/Pages/Home\ Page/Widgets $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -ldl -lpthread -lrt $(LDFLAGS)

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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.1 -DJUCE_APP_VERSION_HEX=0x1 $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I../../Source/Components/Pages/Power\ Page -I../../Source/Shared\ Resources/Config\ Files -I../../Source/Hardware/Bluetooth -I../../Source/Components/Pages/Home\ Page/AppMenu -I../../Source/Components/Pages/Login\ Page -I../../Source/Hardware/Wifi -I../../Source/Components/Pages/UI\ Page -I../../Source/Components/Pages -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Types -I../../Source/Components/Pages/Settings\ Page -I../../Source/Components/Pages/Password\ Pages -I../../Source/Components/Widgets -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Folders -I../../Source/Components/Pages/Home\ Page -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items -I../../Source/Shared\ Resources -I../../Source/Components/Pages/Advanced\ Settings\ Page -I../../Source -I../../Source/Hardware -I../../Source/Components/Pages/Home\ Settings\ Page -I../../Source/Components -I../../Source/Components/Pages/DateTime\ Page -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors -I../../Source/Components/Configurable -I../../Source/Components/Pages/Input\ Settings\ Page -I../../Source/Util -I../../Source/Components/Pages/Home\ Page/Widgets $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -fvisibility=hidden -ldl -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/AssetFiles_d42310d.o \
  $(JUCE_OBJDIR)/GLibSignalHandler_b4a512a8.o \
  $(JUCE_OBJDIR)/GLibThread_4bc12b6e.o \
  $(JUCE_OBJDIR)/ConfigFile_77117fb8.o \
  $(JUCE_OBJDIR)/AppConfigFile_5f9d59cf.o \
  $(JUCE_OBJDIR)/ComponentConfigFile_1ed536b.o \
  $(JUCE_OBJDIR)/MainConfigFile_dadbbf51.o \
  $(JUCE_OBJDIR)/ResourceManager_ecf6cd23.o \
  $(JUCE_OBJDIR)/PageFactory_2f0467c0.o \
  $(JUCE_OBJDIR)/PageComponent_cc06e5f3.o \
  $(JUCE_OBJDIR)/PageStackComponent_e433769.o \
  $(JUCE_OBJDIR)/AdvancedSettingsPage_5c5d9c7a.o \
  $(JUCE_OBJDIR)/DateTimePage_300bca2a.o \
  $(JUCE_OBJDIR)/AppMenuComponent_9a61c110.o \
  $(JUCE_OBJDIR)/AppMenuFolder_cf83dd05.o \
  $(JUCE_OBJDIR)/AppMenuButton_163de5c9.o \
  $(JUCE_OBJDIR)/PagedAppMenu_9a7378f9.o \
  $(JUCE_OBJDIR)/ScrollingAppMenu_ad43aa39.o \
  $(JUCE_OBJDIR)/AppMenuItem_38ba9f96.o \
  $(JUCE_OBJDIR)/AppMenuItemFactory_f8d96a9e.o \
  $(JUCE_OBJDIR)/ConfigAppMenuItem_6358e574.o \
  $(JUCE_OBJDIR)/DesktopEntryMenuItem_2a92ff4f.o \
  $(JUCE_OBJDIR)/FolderMenuItem_e878bb87.o \
  $(JUCE_OBJDIR)/BaseFolderMenuItem_15cee338.o \
  $(JUCE_OBJDIR)/ScrollingAppFolder_8acc40fc.o \
  $(JUCE_OBJDIR)/PageAppFolder_ab5326e8.o \
  $(JUCE_OBJDIR)/NewFolderEditor_7ea02224.o \
  $(JUCE_OBJDIR)/NewDesktopAppEditor_6fed0f5b.o \
  $(JUCE_OBJDIR)/NewConfigAppEditor_9be2c48d.o \
  $(JUCE_OBJDIR)/CategoryPopupEditor_2e68b844.o \
  $(JUCE_OBJDIR)/AppMenuPopupEditor_c7c64ffa.o \
  $(JUCE_OBJDIR)/BatteryIcon_ae581b8c.o \
  $(JUCE_OBJDIR)/ClockLabel_6ed3e56a.o \
  $(JUCE_OBJDIR)/WifiIcon_554a52d2.o \
  $(JUCE_OBJDIR)/HomePage_3ab80d32.o \
  $(JUCE_OBJDIR)/DesktopEntries_f47d5bb8.o \
  $(JUCE_OBJDIR)/DesktopEntry_792d9f5a.o \
  $(JUCE_OBJDIR)/IconThread_18b96ec7.o \
  $(JUCE_OBJDIR)/HomeSettingsPage_5c31e9da.o \
  $(JUCE_OBJDIR)/InputSettingsPage_bca1a328.o \
  $(JUCE_OBJDIR)/KeybindingPage_3a39c653.o \
  $(JUCE_OBJDIR)/LoginPage_5afd41d4.o \
  $(JUCE_OBJDIR)/Password_3b135d30.o \
  $(JUCE_OBJDIR)/RemovePasswordPage_c05abb83.o \
  $(JUCE_OBJDIR)/SetPasswordPage_350974a1.o \
  $(JUCE_OBJDIR)/FelPage_223d819c.o \
  $(JUCE_OBJDIR)/PowerPage_a7c5e854.o \
  $(JUCE_OBJDIR)/SettingsPage_a252453a.o \
  $(JUCE_OBJDIR)/WifiSettingsComponent_8fba54e7.o \
  $(JUCE_OBJDIR)/WifiSettingsPage_30391cef.o \
  $(JUCE_OBJDIR)/BluetoothSettingsComponent_587a0b4.o \
  $(JUCE_OBJDIR)/ConnectionSettingsComponent_f60196fe.o \
  $(JUCE_OBJDIR)/BluetoothSettingsPage_60f4e502.o \
  $(JUCE_OBJDIR)/UIPage_7fd19d1c.o \
  $(JUCE_OBJDIR)/ComponentEditorPage_bc490e6a.o \
  $(JUCE_OBJDIR)/ColourPage_8cc97274.o \
  $(JUCE_OBJDIR)/ConfigurableComponent_2fd2b8e8.o \
  $(JUCE_OBJDIR)/ConfigurableImageButton_bf755ad8.o \
  $(JUCE_OBJDIR)/ConfigurableImageComponent_8700fda1.o \
  $(JUCE_OBJDIR)/ConfigurableLabel_16cf925f.o \
  $(JUCE_OBJDIR)/ColourPicker_b22d9d54.o \
  $(JUCE_OBJDIR)/CounterComponent_e2edf03b.o \
  $(JUCE_OBJDIR)/DrawableImageButton_f6e4065f.o \
  $(JUCE_OBJDIR)/DrawableImageComponent_3ecbd0ba.o \
  $(JUCE_OBJDIR)/FileSelectTextEditor_1986e78c.o \
  $(JUCE_OBJDIR)/IconSliderComponent_bea472d3.o \
  $(JUCE_OBJDIR)/ListEditor_47608845.o \
  $(JUCE_OBJDIR)/OverlaySpinner_44ac5ac3.o \
  $(JUCE_OBJDIR)/PopupEditorComponent_389c099e.o \
  $(JUCE_OBJDIR)/ScalingLabel_dfb4d207.o \
  $(JUCE_OBJDIR)/Spinner_20980f09.o \
  $(JUCE_OBJDIR)/SwitchComponent_5e914a19.o \
  $(JUCE_OBJDIR)/PocketHomeWindow_b7303d3f.o \
  $(JUCE_OBJDIR)/PokeLookAndFeel_14171995.o \
  $(JUCE_OBJDIR)/RelativeLayoutManager_47b4eae7.o \
  $(JUCE_OBJDIR)/BluetoothDevice_e0a04f25.o \
  $(JUCE_OBJDIR)/BluetoothStatus_c0d8d2c1.o \
  $(JUCE_OBJDIR)/BluezAdapter_15dceeb8.o \
  $(JUCE_OBJDIR)/GDBusProxyObject_3f38de33.o \
  $(JUCE_OBJDIR)/GVariantConverter_9ef27fa3.o \
  $(JUCE_OBJDIR)/LibNMHandler_80342d02.o \
  $(JUCE_OBJDIR)/WifiAccessPoint_9918f865.o \
  $(JUCE_OBJDIR)/WifiStateManager_eb7649ed.o \
  $(JUCE_OBJDIR)/JsonWifiInterface_80394d0a.o \
  $(JUCE_OBJDIR)/LibNMInterface_c0abbeb1.o \
  $(JUCE_OBJDIR)/Audio_9f9a54d8.o \
  $(JUCE_OBJDIR)/BatteryMonitor_e04a8815.o \
  $(JUCE_OBJDIR)/Display_ae2c5c84.o \
  $(JUCE_OBJDIR)/I2CBus_c2ea13ae.o \
  $(JUCE_OBJDIR)/TempTimer_170bcb6d.o \
  $(JUCE_OBJDIR)/Utils_94161fcd.o \
  $(JUCE_OBJDIR)/WindowFocus_936f2124.o \
  $(JUCE_OBJDIR)/WindowFocusedTimer_3fa91c4c.o \
  $(JUCE_OBJDIR)/Localized_63fd9217.o \
  $(JUCE_OBJDIR)/AppLauncher_58352abd.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/PocketHomeApplication_4065085c.o \
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
	$(V_AT)$(CXX) -o $(JUCE_OUTDIR)/$(JUCE_TARGET_APP) $(OBJECTS_APP) $(JUCE_LDFLAGS) $(JUCE_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(JUCE_OBJDIR)/AssetFiles_d42310d.o: ../../Source/Shared\ Resources/AssetFiles.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AssetFiles.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibSignalHandler_b4a512a8.o: ../../Source/Shared\ Resources/GLibSignalHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibSignalHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibThread_4bc12b6e.o: ../../Source/Shared\ Resources/GLibThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigFile_77117fb8.o: ../../Source/Shared\ Resources/Config\ Files/ConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppConfigFile_5f9d59cf.o: ../../Source/Shared\ Resources/Config\ Files/AppConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentConfigFile_1ed536b.o: ../../Source/Shared\ Resources/Config\ Files/ComponentConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainConfigFile_dadbbf51.o: ../../Source/Shared\ Resources/Config\ Files/MainConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ResourceManager_ecf6cd23.o: ../../Source/Shared\ Resources/ResourceManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ResourceManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageFactory_2f0467c0.o: ../../Source/Components/Pages/PageFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageComponent_cc06e5f3.o: ../../Source/Components/Pages/PageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageStackComponent_e433769.o: ../../Source/Components/Pages/PageStackComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageStackComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AdvancedSettingsPage_5c5d9c7a.o: ../../Source/Components/Pages/Advanced\ Settings\ Page/AdvancedSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AdvancedSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DateTimePage_300bca2a.o: ../../Source/Components/Pages/DateTime\ Page/DateTimePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DateTimePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuComponent_9a61c110.o: ../../Source/Components/Pages/Home\ Page/AppMenu/AppMenuComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuFolder_cf83dd05.o: ../../Source/Components/Pages/Home\ Page/AppMenu/AppMenuFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuButton_163de5c9.o: ../../Source/Components/Pages/Home\ Page/AppMenu/AppMenuButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PagedAppMenu_9a7378f9.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Types/PagedAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PagedAppMenu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScrollingAppMenu_ad43aa39.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Types/ScrollingAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppMenu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuItem_38ba9f96.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/AppMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuItemFactory_f8d96a9e.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/AppMenuItemFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuItemFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigAppMenuItem_6358e574.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/ConfigAppMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigAppMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntryMenuItem_2a92ff4f.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/DesktopEntryMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntryMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FolderMenuItem_e878bb87.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/FolderMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FolderMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BaseFolderMenuItem_15cee338.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items/BaseFolderMenuItem.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BaseFolderMenuItem.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScrollingAppFolder_8acc40fc.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Folders/ScrollingAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageAppFolder_ab5326e8.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Folders/PageAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewFolderEditor_7ea02224.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewFolderEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewFolderEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewDesktopAppEditor_6fed0f5b.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewDesktopAppEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewDesktopAppEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NewConfigAppEditor_9be2c48d.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/NewConfigAppEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NewConfigAppEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CategoryPopupEditor_2e68b844.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/CategoryPopupEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CategoryPopupEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppMenuPopupEditor_c7c64ffa.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors/AppMenuPopupEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppMenuPopupEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryIcon_ae581b8c.o: ../../Source/Components/Pages/Home\ Page/Widgets/BatteryIcon.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryIcon.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ClockLabel_6ed3e56a.o: ../../Source/Components/Pages/Home\ Page/Widgets/ClockLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ClockLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiIcon_554a52d2.o: ../../Source/Components/Pages/Home\ Page/Widgets/WifiIcon.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiIcon.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HomePage_3ab80d32.o: ../../Source/Components/Pages/Home\ Page/HomePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntries_f47d5bb8.o: ../../Source/Components/Pages/Home\ Page/DesktopEntries.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntries.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntry_792d9f5a.o: ../../Source/Components/Pages/Home\ Page/DesktopEntry.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntry.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconThread_18b96ec7.o: ../../Source/Components/Pages/Home\ Page/IconThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HomeSettingsPage_5c31e9da.o: ../../Source/Components/Pages/Home\ Settings\ Page/HomeSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomeSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputSettingsPage_bca1a328.o: ../../Source/Components/Pages/Input\ Settings\ Page/InputSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KeybindingPage_3a39c653.o: ../../Source/Components/Pages/Input\ Settings\ Page/KeybindingPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KeybindingPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LoginPage_5afd41d4.o: ../../Source/Components/Pages/Login\ Page/LoginPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoginPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Password_3b135d30.o: ../../Source/Components/Pages/Password\ Pages/Password.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Password.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RemovePasswordPage_c05abb83.o: ../../Source/Components/Pages/Password\ Pages/RemovePasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RemovePasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SetPasswordPage_350974a1.o: ../../Source/Components/Pages/Password\ Pages/SetPasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SetPasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FelPage_223d819c.o: ../../Source/Components/Pages/Power\ Page/FelPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FelPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PowerPage_a7c5e854.o: ../../Source/Components/Pages/Power\ Page/PowerPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PowerPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsPage_a252453a.o: ../../Source/Components/Pages/Settings\ Page/SettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsComponent_8fba54e7.o: ../../Source/Components/Pages/Settings\ Page/WifiSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsPage_30391cef.o: ../../Source/Components/Pages/Settings\ Page/WifiSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsComponent_587a0b4.o: ../../Source/Components/Pages/Settings\ Page/BluetoothSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectionSettingsComponent_f60196fe.o: ../../Source/Components/Pages/Settings\ Page/ConnectionSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectionSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsPage_60f4e502.o: ../../Source/Components/Pages/Settings\ Page/BluetoothSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UIPage_7fd19d1c.o: ../../Source/Components/Pages/UI\ Page/UIPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UIPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentEditorPage_bc490e6a.o: ../../Source/Components/Pages/UI\ Page/ComponentEditorPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentEditorPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourPage_8cc97274.o: ../../Source/Components/Pages/UI\ Page/ColourPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableComponent_2fd2b8e8.o: ../../Source/Components/Configurable/ConfigurableComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableImageButton_bf755ad8.o: ../../Source/Components/Configurable/ConfigurableImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableImageComponent_8700fda1.o: ../../Source/Components/Configurable/ConfigurableImageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableImageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigurableLabel_16cf925f.o: ../../Source/Components/Configurable/ConfigurableLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigurableLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourPicker_b22d9d54.o: ../../Source/Components/Widgets/ColourPicker.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourPicker.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/CounterComponent_e2edf03b.o: ../../Source/Components/Widgets/CounterComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling CounterComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DrawableImageButton_f6e4065f.o: ../../Source/Components/Widgets/DrawableImageButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DrawableImageButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DrawableImageComponent_3ecbd0ba.o: ../../Source/Components/Widgets/DrawableImageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DrawableImageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FileSelectTextEditor_1986e78c.o: ../../Source/Components/Widgets/FileSelectTextEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FileSelectTextEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconSliderComponent_bea472d3.o: ../../Source/Components/Widgets/IconSliderComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconSliderComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ListEditor_47608845.o: ../../Source/Components/Widgets/ListEditor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ListEditor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/OverlaySpinner_44ac5ac3.o: ../../Source/Components/Widgets/OverlaySpinner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling OverlaySpinner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PopupEditorComponent_389c099e.o: ../../Source/Components/Widgets/PopupEditorComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PopupEditorComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScalingLabel_dfb4d207.o: ../../Source/Components/Widgets/ScalingLabel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScalingLabel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Spinner_20980f09.o: ../../Source/Components/Widgets/Spinner.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Spinner.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SwitchComponent_5e914a19.o: ../../Source/Components/Widgets/SwitchComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SwitchComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PocketHomeWindow_b7303d3f.o: ../../Source/Components/PocketHomeWindow.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PocketHomeWindow.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PokeLookAndFeel_14171995.o: ../../Source/Components/PokeLookAndFeel.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PokeLookAndFeel.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RelativeLayoutManager_47b4eae7.o: ../../Source/Components/RelativeLayoutManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RelativeLayoutManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothDevice_e0a04f25.o: ../../Source/Hardware/Bluetooth/BluetoothDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothStatus_c0d8d2c1.o: ../../Source/Hardware/Bluetooth/BluetoothStatus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothStatus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluezAdapter_15dceeb8.o: ../../Source/Hardware/Bluetooth/BluezAdapter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluezAdapter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GDBusProxyObject_3f38de33.o: ../../Source/Hardware/Bluetooth/GDBusProxyObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GDBusProxyObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GVariantConverter_9ef27fa3.o: ../../Source/Hardware/Bluetooth/GVariantConverter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GVariantConverter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LibNMHandler_80342d02.o: ../../Source/Hardware/Wifi/LibNMHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LibNMHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiAccessPoint_9918f865.o: ../../Source/Hardware/Wifi/WifiAccessPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiAccessPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiStateManager_eb7649ed.o: ../../Source/Hardware/Wifi/WifiStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsonWifiInterface_80394d0a.o: ../../Source/Hardware/Wifi/JsonWifiInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsonWifiInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LibNMInterface_c0abbeb1.o: ../../Source/Hardware/Wifi/LibNMInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LibNMInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Audio_9f9a54d8.o: ../../Source/Hardware/Audio.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Audio.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryMonitor_e04a8815.o: ../../Source/Hardware/BatteryMonitor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryMonitor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Display_ae2c5c84.o: ../../Source/Hardware/Display.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Display.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/I2CBus_c2ea13ae.o: ../../Source/Hardware/I2CBus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling I2CBus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TempTimer_170bcb6d.o: ../../Source/Util/TempTimer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TempTimer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Utils_94161fcd.o: ../../Source/Util/Utils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Utils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WindowFocus_936f2124.o: ../../Source/Util/WindowFocus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WindowFocus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WindowFocusedTimer_3fa91c4c.o: ../../Source/Util/WindowFocusedTimer.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WindowFocusedTimer.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Localized_63fd9217.o: ../../Source/Util/Localized.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Localized.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppLauncher_58352abd.o: ../../Source/Util/AppLauncher.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppLauncher.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PocketHomeApplication_4065085c.o: ../../Source/PocketHomeApplication.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PocketHomeApplication.cpp"
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
	@pkg-config --print-errors NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama

clean:
	@echo Cleaning pocket-home
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping pocket-home
	-$(V_AT)$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

-include $(OBJECTS_APP:%.o=%.d)
