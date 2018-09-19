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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DDONT_SET_USING_JUCE_NAMESPACE=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.8.10 -DJUCE_APP_VERSION_HEX=0x80a $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I../../Source/System -I../../Source/Config\ Files/Component -I../../Source/Config\ Files/Main -I../../Source/Components/Pages/Settings\ Pages/DateTime\ Page -I../../Source/Components/Pages/Settings\ Pages/Password\ Pages -I../../Source/Components/Pages/Login\ Page -I../../Source/FreeDesktop/Icon\ Themes -I../../Source/Components/Pages/Settings\ Pages/Network\ Pages -I../../Source/Components/Pages/Settings\ Pages/UI\ Page -I../../Source/Components/Pages/Settings\ Pages/Settings\ List\ Page -I../../Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page -I../../Source/Bluetooth -I../../Source/Components/Pages -I../../Source/FreeDesktop -I../../Source/Util -I../../Source/Config\ Files/Colour -I../../Source/Config\ Files -I../../Source/Components/Pages/Settings\ Pages -I../../Source/Components/Pages/Power\ Page -I../../Source/Components/Pages/Home\ Page -I../../Source/Components/Pages/Settings\ Pages/Home\ Settings\ Page -I../../Source/PocketCHIP -I../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page -I../../Source/Wifi -I../../Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu -I../../Source/Util/SharedResource -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items -I../../Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu -I../../Source/GLib/DBus -I../../Source/Components/Configurable -I../../Source/Components -I../../Source/Config\ Files/AppMenu -I../../Source/Components/Pages/Home\ Page/AppMenu -I../../Source/Components/Pages/Home\ Page/Widgets -I../../Source -I../../Source/GLib/LibNM -I../../Source/Components/Widgets -I../../Source/GLib -I../../Source/FreeDesktop/Desktop\ Entries -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

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

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DDONT_SET_USING_JUCE_NAMESPACE=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=0.0.8.10 -DJUCE_APP_VERSION_HEX=0x80a $(shell pkg-config --cflags NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../deps/JUCE/modules -I../../Source/System -I../../Source/Config\ Files/Component -I../../Source/Config\ Files/Main -I../../Source/Components/Pages/Settings\ Pages/DateTime\ Page -I../../Source/Components/Pages/Settings\ Pages/Password\ Pages -I../../Source/Components/Pages/Login\ Page -I../../Source/FreeDesktop/Icon\ Themes -I../../Source/Components/Pages/Settings\ Pages/Network\ Pages -I../../Source/Components/Pages/Settings\ Pages/UI\ Page -I../../Source/Components/Pages/Settings\ Pages/Settings\ List\ Page -I../../Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page -I../../Source/Bluetooth -I../../Source/Components/Pages -I../../Source/FreeDesktop -I../../Source/Util -I../../Source/Config\ Files/Colour -I../../Source/Config\ Files -I../../Source/Components/Pages/Settings\ Pages -I../../Source/Components/Pages/Power\ Page -I../../Source/Components/Pages/Home\ Page -I../../Source/Components/Pages/Settings\ Pages/Home\ Settings\ Page -I../../Source/PocketCHIP -I../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page -I../../Source/Wifi -I../../Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu -I../../Source/Util/SharedResource -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Items -I../../Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu -I../../Source/GLib/DBus -I../../Source/Components/Configurable -I../../Source/Components -I../../Source/Config\ Files/AppMenu -I../../Source/Components/Pages/Home\ Page/AppMenu -I../../Source/Components/Pages/Home\ Page/Widgets -I../../Source -I../../Source/GLib/LibNM -I../../Source/Components/Widgets -I../../Source/GLib -I../../Source/FreeDesktop/Desktop\ Entries -I../../Source/Components/Pages/Home\ Page/AppMenu/Menu\ Editors $(CPPFLAGS)
  JUCE_CPPFLAGS_APP := -DJucePlugin_Build_VST=0 -DJucePlugin_Build_VST3=0 -DJucePlugin_Build_AU=0 -DJucePlugin_Build_AUv3=0 -DJucePlugin_Build_RTAS=0 -DJucePlugin_Build_AAX=0 -DJucePlugin_Build_Standalone=0
  JUCE_TARGET_APP := pocket-home

  JUCE_CFLAGS += $(JUCE_CPPFLAGS) $(TARGET_ARCH) -O3 -flto $(CFLAGS)
  JUCE_CXXFLAGS += $(JUCE_CFLAGS) -std=gnu++14 $(CXXFLAGS)
  JUCE_LDFLAGS += $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs NetworkManager libnm-glib alsa freetype2 libssl gio-2.0 x11 xext xinerama) -fvisibility=hidden -flto -lcrypto -ldl -lpthread -lrt $(LDFLAGS)

  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS_APP := \
  $(JUCE_OBJDIR)/SystemCommands_9ca9c9b8.o \
  $(JUCE_OBJDIR)/BluetoothDevice_b56b2104.o \
  $(JUCE_OBJDIR)/BluetoothStatus_95a3a4a0.o \
  $(JUCE_OBJDIR)/BluezAdapter_eb8139.o \
  $(JUCE_OBJDIR)/ConfigurableComponent_2fd2b8e8.o \
  $(JUCE_OBJDIR)/ConfigurableImageButton_bf755ad8.o \
  $(JUCE_OBJDIR)/ConfigurableImageComponent_8700fda1.o \
  $(JUCE_OBJDIR)/ConfigurableLabel_16cf925f.o \
  $(JUCE_OBJDIR)/ScrollingAppFolder_b3d100d6.o \
  $(JUCE_OBJDIR)/ScrollingAppMenu_8e65aa07.o \
  $(JUCE_OBJDIR)/PageAppFolder_5f41e00e.o \
  $(JUCE_OBJDIR)/PagedAppMenu_e97bee87.o \
  $(JUCE_OBJDIR)/AppMenuComponent_9a61c110.o \
  $(JUCE_OBJDIR)/AppMenuFolder_cf83dd05.o \
  $(JUCE_OBJDIR)/AppMenuButton_163de5c9.o \
  $(JUCE_OBJDIR)/AppMenuItem_38ba9f96.o \
  $(JUCE_OBJDIR)/AppMenuItemFactory_f8d96a9e.o \
  $(JUCE_OBJDIR)/ConfigAppMenuItem_6358e574.o \
  $(JUCE_OBJDIR)/DesktopEntryMenuItem_2a92ff4f.o \
  $(JUCE_OBJDIR)/FolderMenuItem_e878bb87.o \
  $(JUCE_OBJDIR)/BaseFolderMenuItem_15cee338.o \
  $(JUCE_OBJDIR)/NewFolderEditor_7ea02224.o \
  $(JUCE_OBJDIR)/NewDesktopAppEditor_6fed0f5b.o \
  $(JUCE_OBJDIR)/NewConfigAppEditor_9be2c48d.o \
  $(JUCE_OBJDIR)/CategoryPopupEditor_2e68b844.o \
  $(JUCE_OBJDIR)/AppMenuPopupEditor_c7c64ffa.o \
  $(JUCE_OBJDIR)/HomePage_3ab80d32.o \
  $(JUCE_OBJDIR)/BatteryIcon_ae581b8c.o \
  $(JUCE_OBJDIR)/ClockLabel_6ed3e56a.o \
  $(JUCE_OBJDIR)/WifiIcon_554a52d2.o \
  $(JUCE_OBJDIR)/LoginPage_5afd41d4.o \
  $(JUCE_OBJDIR)/FelPage_223d819c.o \
  $(JUCE_OBJDIR)/PowerPage_a7c5e854.o \
  $(JUCE_OBJDIR)/QuickSettingsPage_d4e78d06.o \
  $(JUCE_OBJDIR)/WifiSettingsComponent_892aaeac.o \
  $(JUCE_OBJDIR)/BluetoothSettingsComponent_a1919a0f.o \
  $(JUCE_OBJDIR)/ConnectionSettingsComponent_db36c903.o \
  $(JUCE_OBJDIR)/SettingsListPage_3e4f4a8.o \
  $(JUCE_OBJDIR)/WifiSettingsPage_5d7677f1.o \
  $(JUCE_OBJDIR)/BluetoothSettingsPage_3e421b40.o \
  $(JUCE_OBJDIR)/FocusingListPage_b9e42161.o \
  $(JUCE_OBJDIR)/HomeSettingsPage_6caf58f2.o \
  $(JUCE_OBJDIR)/UIPage_a091ef04.o \
  $(JUCE_OBJDIR)/ComponentEditorPage_b1531582.o \
  $(JUCE_OBJDIR)/ColourPage_cfbe905c.o \
  $(JUCE_OBJDIR)/InputSettingsPage_a37fac40.o \
  $(JUCE_OBJDIR)/KeybindingPage_4b46143b.o \
  $(JUCE_OBJDIR)/Password_91acb048.o \
  $(JUCE_OBJDIR)/RemovePasswordPage_8719109b.o \
  $(JUCE_OBJDIR)/SetPasswordPage_aef8f689.o \
  $(JUCE_OBJDIR)/DateTimePage_cc988012.o \
  $(JUCE_OBJDIR)/PageComponent_cc06e5f3.o \
  $(JUCE_OBJDIR)/PageFactory_2f0467c0.o \
  $(JUCE_OBJDIR)/PageStackComponent_e433769.o \
  $(JUCE_OBJDIR)/ScalingTextButton_2b003e56.o \
  $(JUCE_OBJDIR)/PagedList_64e4f23.o \
  $(JUCE_OBJDIR)/NavButton_cf1b4265.o \
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
  $(JUCE_OBJDIR)/LayoutManager_6a2c5f53.o \
  $(JUCE_OBJDIR)/TransitionAnimator_716e83f6.o \
  $(JUCE_OBJDIR)/MainJSON_7d61af2a.o \
  $(JUCE_OBJDIR)/ComponentConfigFile_340680dc.o \
  $(JUCE_OBJDIR)/ComponentJSON_30e8db86.o \
  $(JUCE_OBJDIR)/ComponentSettings_c82bd4e1.o \
  $(JUCE_OBJDIR)/ColourConfigFile_64de91a0.o \
  $(JUCE_OBJDIR)/ColourConfigKeys_af9fce18.o \
  $(JUCE_OBJDIR)/ColourJSON_a63dcf4a.o \
  $(JUCE_OBJDIR)/AppConfigFile_38950ae3.o \
  $(JUCE_OBJDIR)/AppFolder_a44c0b73.o \
  $(JUCE_OBJDIR)/AppShortcut_82838b4b.o \
  $(JUCE_OBJDIR)/AppJSON_bd8d36cd.o \
  $(JUCE_OBJDIR)/ConfigKey_75384910.o \
  $(JUCE_OBJDIR)/ConfigAlertWindows_cf8924c0.o \
  $(JUCE_OBJDIR)/JSONFile_f406f23b.o \
  $(JUCE_OBJDIR)/ConfigJSON_cbbe9f81.o \
  $(JUCE_OBJDIR)/XDGDirectories_5308a3d8.o \
  $(JUCE_OBJDIR)/IconCache_491da5cd.o \
  $(JUCE_OBJDIR)/IconLoader_ed34cf72.o \
  $(JUCE_OBJDIR)/IconThemeIndex_dea347e8.o \
  $(JUCE_OBJDIR)/IconThread_94e2d9e9.o \
  $(JUCE_OBJDIR)/DesktopEntryLoader_a86a5c94.o \
  $(JUCE_OBJDIR)/DesktopEntry_3f1762e1.o \
  $(JUCE_OBJDIR)/GVariantConverter_2a602041.o \
  $(JUCE_OBJDIR)/GPPDBusProxy_1d8cecf6.o \
  $(JUCE_OBJDIR)/SavedConnection_87ff9dd2.o \
  $(JUCE_OBJDIR)/SavedConnections_7ba82a2b.o \
  $(JUCE_OBJDIR)/NMPPAccessPoint_680fb63a.o \
  $(JUCE_OBJDIR)/NMPPActiveConnection_d182c3e0.o \
  $(JUCE_OBJDIR)/NMPPClient_2840c667.o \
  $(JUCE_OBJDIR)/NMPPConnection_5641847a.o \
  $(JUCE_OBJDIR)/NMPPDeviceWifi_a7e88da7.o \
  $(JUCE_OBJDIR)/GLibSignalThread_b93342e2.o \
  $(JUCE_OBJDIR)/GLibThread_5746ca5a.o \
  $(JUCE_OBJDIR)/GPPObject_f97abf7e.o \
  $(JUCE_OBJDIR)/GPPWeakRef_58d87b66.o \
  $(JUCE_OBJDIR)/GSignalHandler_3a448c6d.o \
  $(JUCE_OBJDIR)/Audio_f2c8052.o \
  $(JUCE_OBJDIR)/BatteryMonitor_95f8a5db.o \
  $(JUCE_OBJDIR)/Display_81e1917e.o \
  $(JUCE_OBJDIR)/I2CBus_459d5774.o \
  $(JUCE_OBJDIR)/ScopedThreadWriteLock_777c0162.o \
  $(JUCE_OBJDIR)/ScopedThreadReadLock_9b17abb3.o \
  $(JUCE_OBJDIR)/ThreadLock_fe58c0d.o \
  $(JUCE_OBJDIR)/SharedResource_6871b0cb.o \
  $(JUCE_OBJDIR)/LaunchedProcess_f13ef2d9.o \
  $(JUCE_OBJDIR)/XWindowInterface_b020667f.o \
  $(JUCE_OBJDIR)/ProcessUtils_6fe72270.o \
  $(JUCE_OBJDIR)/AssetFiles_de972815.o \
  $(JUCE_OBJDIR)/LocalizedTime_cb579984.o \
  $(JUCE_OBJDIR)/TempTimer_170bcb6d.o \
  $(JUCE_OBJDIR)/Utils_94161fcd.o \
  $(JUCE_OBJDIR)/WindowFocus_936f2124.o \
  $(JUCE_OBJDIR)/WindowFocusedTimer_3fa91c4c.o \
  $(JUCE_OBJDIR)/Localized_63fd9217.o \
  $(JUCE_OBJDIR)/AppLauncher_58352abd.o \
  $(JUCE_OBJDIR)/NetworkInterface_15f7b366.o \
  $(JUCE_OBJDIR)/LibNMInterface_f2f523d0.o \
  $(JUCE_OBJDIR)/WifiAccessPoint_affc3726.o \
  $(JUCE_OBJDIR)/WifiStateManager_b0fae34c.o \
  $(JUCE_OBJDIR)/JsonWifiInterface_6b47df8b.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/PocketHomeApplication_4065085c.o \
  $(JUCE_OBJDIR)/gtest_object_343fa2fd.o \
  $(JUCE_OBJDIR)/GPPTestObject_ea20eb6a.o \
  $(JUCE_OBJDIR)/test_GPPObject_627beab1.o \
  $(JUCE_OBJDIR)/StressTest_8f750ba9.o \
  $(JUCE_OBJDIR)/test_ConfigFile_8cfd452.o \
  $(JUCE_OBJDIR)/test_LaunchedApp_3a56b785.o \
  $(JUCE_OBJDIR)/test_WifiStateManager_7ec85545.o \
  $(JUCE_OBJDIR)/test_XWindowInterface_40175345.o \
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

$(JUCE_OBJDIR)/SystemCommands_9ca9c9b8.o: ../../Source/System/SystemCommands.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SystemCommands.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothDevice_b56b2104.o: ../../Source/Bluetooth/BluetoothDevice.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothDevice.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothStatus_95a3a4a0.o: ../../Source/Bluetooth/BluetoothStatus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothStatus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluezAdapter_eb8139.o: ../../Source/Bluetooth/BluezAdapter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluezAdapter.cpp"
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

$(JUCE_OBJDIR)/ScrollingAppFolder_b3d100d6.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu/ScrollingAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScrollingAppMenu_8e65aa07.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Scrolling\ Menu/ScrollingAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScrollingAppMenu.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageAppFolder_5f41e00e.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu/PageAppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageAppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PagedAppMenu_e97bee87.o: ../../Source/Components/Pages/Home\ Page/AppMenu/Paged\ Menu/PagedAppMenu.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PagedAppMenu.cpp"
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

$(JUCE_OBJDIR)/HomePage_3ab80d32.o: ../../Source/Components/Pages/Home\ Page/HomePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomePage.cpp"
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

$(JUCE_OBJDIR)/LoginPage_5afd41d4.o: ../../Source/Components/Pages/Login\ Page/LoginPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoginPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FelPage_223d819c.o: ../../Source/Components/Pages/Power\ Page/FelPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FelPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PowerPage_a7c5e854.o: ../../Source/Components/Pages/Power\ Page/PowerPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PowerPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/QuickSettingsPage_d4e78d06.o: ../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/QuickSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling QuickSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsComponent_892aaeac.o: ../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/WifiSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsComponent_a1919a0f.o: ../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/BluetoothSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectionSettingsComponent_db36c903.o: ../../Source/Components/Pages/Settings\ Pages/Quick\ Settings\ Page/ConnectionSettingsComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectionSettingsComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SettingsListPage_3e4f4a8.o: ../../Source/Components/Pages/Settings\ Pages/Settings\ List\ Page/SettingsListPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SettingsListPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiSettingsPage_5d7677f1.o: ../../Source/Components/Pages/Settings\ Pages/Network\ Pages/WifiSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BluetoothSettingsPage_3e421b40.o: ../../Source/Components/Pages/Settings\ Pages/Network\ Pages/BluetoothSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BluetoothSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/FocusingListPage_b9e42161.o: ../../Source/Components/Pages/Settings\ Pages/Network\ Pages/FocusingListPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FocusingListPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/HomeSettingsPage_6caf58f2.o: ../../Source/Components/Pages/Settings\ Pages/Home\ Settings\ Page/HomeSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling HomeSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/UIPage_a091ef04.o: ../../Source/Components/Pages/Settings\ Pages/UI\ Page/UIPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling UIPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentEditorPage_b1531582.o: ../../Source/Components/Pages/Settings\ Pages/UI\ Page/ComponentEditorPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentEditorPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourPage_cfbe905c.o: ../../Source/Components/Pages/Settings\ Pages/UI\ Page/ColourPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/InputSettingsPage_a37fac40.o: ../../Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/InputSettingsPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling InputSettingsPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/KeybindingPage_4b46143b.o: ../../Source/Components/Pages/Settings\ Pages/Input\ Settings\ Page/KeybindingPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling KeybindingPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Password_91acb048.o: ../../Source/Components/Pages/Settings\ Pages/Password\ Pages/Password.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Password.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RemovePasswordPage_8719109b.o: ../../Source/Components/Pages/Settings\ Pages/Password\ Pages/RemovePasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RemovePasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SetPasswordPage_aef8f689.o: ../../Source/Components/Pages/Settings\ Pages/Password\ Pages/SetPasswordPage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SetPasswordPage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DateTimePage_cc988012.o: ../../Source/Components/Pages/Settings\ Pages/DateTime\ Page/DateTimePage.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DateTimePage.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageComponent_cc06e5f3.o: ../../Source/Components/Pages/PageComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageFactory_2f0467c0.o: ../../Source/Components/Pages/PageFactory.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageFactory.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PageStackComponent_e433769.o: ../../Source/Components/Pages/PageStackComponent.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PageStackComponent.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScalingTextButton_2b003e56.o: ../../Source/Components/Widgets/ScalingTextButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScalingTextButton.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PagedList_64e4f23.o: ../../Source/Components/Widgets/PagedList.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PagedList.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NavButton_cf1b4265.o: ../../Source/Components/Widgets/NavButton.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NavButton.cpp"
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

$(JUCE_OBJDIR)/LayoutManager_6a2c5f53.o: ../../Source/Components/LayoutManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LayoutManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransitionAnimator_716e83f6.o: ../../Source/Components/TransitionAnimator.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransitionAnimator.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainJSON_7d61af2a.o: ../../Source/Config\ Files/Main/MainJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentConfigFile_340680dc.o: ../../Source/Config\ Files/Component/ComponentConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentJSON_30e8db86.o: ../../Source/Config\ Files/Component/ComponentJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ComponentSettings_c82bd4e1.o: ../../Source/Config\ Files/Component/ComponentSettings.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ComponentSettings.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourConfigFile_64de91a0.o: ../../Source/Config\ Files/Colour/ColourConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourConfigKeys_af9fce18.o: ../../Source/Config\ Files/Colour/ColourConfigKeys.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourConfigKeys.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ColourJSON_a63dcf4a.o: ../../Source/Config\ Files/Colour/ColourJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ColourJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppConfigFile_38950ae3.o: ../../Source/Config\ Files/AppMenu/AppConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppFolder_a44c0b73.o: ../../Source/Config\ Files/AppMenu/AppFolder.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppFolder.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppShortcut_82838b4b.o: ../../Source/Config\ Files/AppMenu/AppShortcut.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppShortcut.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AppJSON_bd8d36cd.o: ../../Source/Config\ Files/AppMenu/AppJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AppJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigKey_75384910.o: ../../Source/Config\ Files/ConfigKey.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigKey.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigAlertWindows_cf8924c0.o: ../../Source/Config\ Files/ConfigAlertWindows.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigAlertWindows.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JSONFile_f406f23b.o: ../../Source/Config\ Files/JSONFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JSONFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConfigJSON_cbbe9f81.o: ../../Source/Config\ Files/ConfigJSON.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConfigJSON.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/XDGDirectories_5308a3d8.o: ../../Source/FreeDesktop/XDGDirectories.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling XDGDirectories.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconCache_491da5cd.o: ../../Source/FreeDesktop/Icon\ Themes/IconCache.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconCache.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconLoader_ed34cf72.o: ../../Source/FreeDesktop/Icon\ Themes/IconLoader.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconLoader.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconThemeIndex_dea347e8.o: ../../Source/FreeDesktop/Icon\ Themes/IconThemeIndex.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconThemeIndex.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/IconThread_94e2d9e9.o: ../../Source/FreeDesktop/Icon\ Themes/IconThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling IconThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntryLoader_a86a5c94.o: ../../Source/FreeDesktop/Desktop\ Entries/DesktopEntryLoader.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntryLoader.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DesktopEntry_3f1762e1.o: ../../Source/FreeDesktop/Desktop\ Entries/DesktopEntry.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DesktopEntry.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GVariantConverter_2a602041.o: ../../Source/GLib/DBus/GVariantConverter.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GVariantConverter.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPDBusProxy_1d8cecf6.o: ../../Source/GLib/DBus/GPPDBusProxy.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPDBusProxy.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SavedConnection_87ff9dd2.o: ../../Source/GLib/LibNM/SavedConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SavedConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SavedConnections_7ba82a2b.o: ../../Source/GLib/LibNM/SavedConnections.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SavedConnections.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPAccessPoint_680fb63a.o: ../../Source/GLib/LibNM/NMPPAccessPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPAccessPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPActiveConnection_d182c3e0.o: ../../Source/GLib/LibNM/NMPPActiveConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPActiveConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPClient_2840c667.o: ../../Source/GLib/LibNM/NMPPClient.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPClient.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPConnection_5641847a.o: ../../Source/GLib/LibNM/NMPPConnection.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPConnection.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NMPPDeviceWifi_a7e88da7.o: ../../Source/GLib/LibNM/NMPPDeviceWifi.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NMPPDeviceWifi.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibSignalThread_b93342e2.o: ../../Source/GLib/GLibSignalThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibSignalThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GLibThread_5746ca5a.o: ../../Source/GLib/GLibThread.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GLibThread.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPObject_f97abf7e.o: ../../Source/GLib/GPPObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPWeakRef_58d87b66.o: ../../Source/GLib/GPPWeakRef.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPWeakRef.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GSignalHandler_3a448c6d.o: ../../Source/GLib/GSignalHandler.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GSignalHandler.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Audio_f2c8052.o: ../../Source/PocketCHIP/Audio.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Audio.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BatteryMonitor_95f8a5db.o: ../../Source/PocketCHIP/BatteryMonitor.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BatteryMonitor.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Display_81e1917e.o: ../../Source/PocketCHIP/Display.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Display.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/I2CBus_459d5774.o: ../../Source/PocketCHIP/I2CBus.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling I2CBus.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScopedThreadWriteLock_777c0162.o: ../../Source/Util/SharedResource/ScopedThreadWriteLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScopedThreadWriteLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ScopedThreadReadLock_9b17abb3.o: ../../Source/Util/SharedResource/ScopedThreadReadLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ScopedThreadReadLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ThreadLock_fe58c0d.o: ../../Source/Util/SharedResource/ThreadLock.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ThreadLock.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/SharedResource_6871b0cb.o: ../../Source/Util/SharedResource/SharedResource.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling SharedResource.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LaunchedProcess_f13ef2d9.o: ../../Source/Util/LaunchedProcess.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LaunchedProcess.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/XWindowInterface_b020667f.o: ../../Source/Util/XWindowInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling XWindowInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ProcessUtils_6fe72270.o: ../../Source/Util/ProcessUtils.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ProcessUtils.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AssetFiles_de972815.o: ../../Source/Util/AssetFiles.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AssetFiles.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LocalizedTime_cb579984.o: ../../Source/Util/LocalizedTime.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LocalizedTime.cpp"
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

$(JUCE_OBJDIR)/NetworkInterface_15f7b366.o: ../../Source/Wifi/NetworkInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NetworkInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LibNMInterface_f2f523d0.o: ../../Source/Wifi/LibNMInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LibNMInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiAccessPoint_affc3726.o: ../../Source/Wifi/WifiAccessPoint.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiAccessPoint.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/WifiStateManager_b0fae34c.o: ../../Source/Wifi/WifiStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling WifiStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/JsonWifiInterface_6b47df8b.o: ../../Source/Wifi/JsonWifiInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling JsonWifiInterface.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PocketHomeApplication_4065085c.o: ../../Source/PocketHomeApplication.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PocketHomeApplication.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/gtest_object_343fa2fd.o: ../../Tests/GLib/gtest_object.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling gtest_object.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/GPPTestObject_ea20eb6a.o: ../../Tests/GLib/GPPTestObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling GPPTestObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_GPPObject_627beab1.o: ../../Tests/GLib/test_GPPObject.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_GPPObject.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/StressTest_8f750ba9.o: ../../Tests/StressTest.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling StressTest.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_ConfigFile_8cfd452.o: ../../Tests/test_ConfigFile.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_ConfigFile.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_LaunchedApp_3a56b785.o: ../../Tests/test_LaunchedApp.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_LaunchedApp.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_WifiStateManager_7ec85545.o: ../../Tests/test_WifiStateManager.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_WifiStateManager.cpp"
	$(V_AT)$(CXX) $(JUCE_CXXFLAGS) $(JUCE_CPPFLAGS_APP) $(JUCE_CFLAGS_APP) -o "$@" -c "$<"

$(JUCE_OBJDIR)/test_XWindowInterface_40175345.o: ../../Tests/test_XWindowInterface.cpp
	-$(V_AT)mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling test_XWindowInterface.cpp"
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
