########################### Wifi Module ########################################
WIFI_DIR = Source/System/Wifi
WIFI_PREFIX := Wifi_
WIFI_OBJ := $(JUCE_OBJDIR)/$(WIFI_PREFIX)

################ LibNM Submodule ################

NM_DIR = $(WIFI_DIR)/LibNM
NM_BORROWED_DIR := $(NM_DIR)/GObjects/Borrowed
NM_OWNED_DIR := $(NM_DIR)/GObjects/Owned
NM_OWNED_SETTINGS_DIR := $(NM_OWNED_DIR)/Settings
NM_THREAD_DIR := $(NM_DIR)/Thread
NM_DBUS_DIR := $(NM_DIR)/DBus
NM_SIGNAL_DIR := $(NM_DIR)/Signal

NM_PREFIX := $(WIFI_PREFIX)LibNM_
NM_OBJ := $(WIFI_OBJ)LibNM_

NM_BORROWED_OBJ := $(NM_OBJ)Borrowed_
OBJECTS_NM_BORROWED := \
  $(NM_BORROWED_OBJ)AccessPoint.o \
  $(NM_BORROWED_OBJ)ActiveConnection.o \
  $(NM_BORROWED_OBJ)DeviceWifi.o

NM_OWNED_OBJ := $(NM_OBJ)Owned_

NM_OWNED_SETTINGS_PREFIX := $(NM_PREFIX)Settings_
NM_OWNED_SETTINGS_OBJ := $(NM_OWNED_OBJ)Settings_
OBJECTS_NM_OWNED_SETTINGS := \
  $(NM_OWNED_SETTINGS_OBJ)Object.o \
  $(NM_OWNED_SETTINGS_OBJ)Connection.o \
  $(NM_OWNED_SETTINGS_OBJ)Wireless.o \
  $(NM_OWNED_SETTINGS_OBJ)WirelessSecurity.o

OBJECTS_NM_OWNED := \
  $(NM_OWNED_OBJ)Client.o \
  $(NM_OWNED_OBJ)Connection.o \
  $(OBJECTS_NM_OWNED_SETTINGS)

NM_THREAD_PREFIX := $(NM_PREFIX)Thread_
NM_THREAD_OBJ := $(NM_OBJ)Thread_
OBJECTS_NM_THREAD := \
  $(NM_THREAD_OBJ)Module.o \
  $(NM_THREAD_OBJ)Handler.o

NM_DBUS_PREFIX := $(NM_PREFIX)DBus_
NM_DBUS_OBJ := $(NM_OBJ)DBus_
OBJECTS_NM_DBUS := \
  $(NM_DBUS_OBJ)SavedConnection.o \
  $(NM_DBUS_OBJ)SavedConnectionLoader.o  

NM_SIGNAL_PREFIX := $(NM_PREFIX)Signal_
NM_SIGNAL_OBJ := $(NM_OBJ)Signal_
OBJECTS_NM_SIGNAL := \
  $(NM_SIGNAL_OBJ)APHandler.o \
  $(NM_SIGNAL_OBJ)ClientHandler.o \
  $(NM_SIGNAL_OBJ)DeviceHandler.o \

OBJECTS_NM := \
  $(NM_OBJ)APHash.o \
  $(NM_OBJ)SSID.o \
  $(OBJECTS_NM_BORROWED) \
  $(OBJECTS_NM_OWNED) \
  $(OBJECTS_NM_DBUS) \
  $(OBJECTS_NM_THREAD) \
  $(OBJECTS_NM_SIGNAL) 

################ Access Point Submodule ################

WIFI_AP_DIR := $(WIFI_DIR)/AP
WIFI_AP_PREFIX := $(WIFI_PREFIX)AP_
WIFI_AP_OBJ := $(WIFI_OBJ)AP_
OBJECTS_WIFI_ACCESS_POINT := \
  $(WIFI_AP_OBJ)Data.o \
  $(WIFI_AP_OBJ)StrengthListener.o

################ Access Point List Submodule ################

WIFI_AP_LIST_DIR := $(WIFI_DIR)/APList
WIFI_AP_LIST_PREFIX := $(WIFI_PREFIX)APList_
WIFI_AP_LIST_OBJ := $(WIFI_OBJ)APList_
OBJECTS_WIFI_ACCESS_POINT_LIST := \
  $(WIFI_AP_LIST_OBJ)Module.o \
  $(WIFI_AP_LIST_OBJ)Listener.o \
  $(WIFI_AP_LIST_OBJ)Reader.o \
  $(WIFI_AP_LIST_OBJ)NMReader.o \
  $(WIFI_AP_LIST_OBJ)Writer.o

################ Connection Submodules ################

WIFI_CONNECTION_DIR := $(WIFI_DIR)/Connection
WIFI_CONNECTION_RECORD_DIR := $(WIFI_CONNECTION_DIR)/Record
WIFI_CONNECTION_SAVED_DIR := $(WIFI_CONNECTION_DIR)/Saved
WIFI_CONNECTION_CONTROL_DIR := $(WIFI_CONNECTION_DIR)/Control

WIFI_CONNECTION_PREFIX := $(WIFI_PREFIX)Connection_
WIFI_CONNECTION_OBJ := $(WIFI_OBJ)Connection_

WIFI_CONNECTION_RECORD_PREFIX := $(WIFI_CONNECTION_PREFIX)Record_
WIFI_CONNECTION_RECORD_OBJ := $(WIFI_CONNECTION_OBJ)Record_
OBJECTS_WIFI_CONNECTION_RECORD := \
  $(WIFI_CONNECTION_RECORD_OBJ)Module.o \
  $(WIFI_CONNECTION_RECORD_OBJ)Reader.o \
  $(WIFI_CONNECTION_RECORD_OBJ)Writer.o \
  $(WIFI_CONNECTION_RECORD_OBJ)Listener.o

WIFI_CONNECTION_SAVED_PREFIX := $(WIFI_CONNECTION_PREFIX)Saved_
WIFI_CONNECTION_SAVED_OBJ := $(WIFI_CONNECTION_OBJ)Saved_
OBJECTS_WIFI_CONNECTION_SAVED := \
  $(WIFI_CONNECTION_SAVED_OBJ)Module.o \
  $(WIFI_CONNECTION_SAVED_OBJ)Reader.o \
  $(WIFI_CONNECTION_SAVED_OBJ)Deleter.o

WIFI_CONNECTION_CONTROL_PREFIX := $(WIFI_CONNECTION_PREFIX)Control_
WIFI_CONNECTION_CONTROL_OBJ := $(WIFI_CONNECTION_OBJ)Control_
OBJECTS_WIFI_CONNECTION_CONTROL := \
  $(WIFI_CONNECTION_CONTROL_OBJ)Module.o \
  $(WIFI_CONNECTION_CONTROL_OBJ)Handler.o

OBJECTS_WIFI_CONNECTION := \
  $(WIFI_CONNECTION_OBJ)Event.o \
  $(OBJECTS_WIFI_CONNECTION_RECORD) \
  $(OBJECTS_WIFI_CONNECTION_SAVED) \
  $(OBJECTS_WIFI_CONNECTION_CONTROL) \

################ Device Submodule ################

WIFI_DEVICE_DIR := $(WIFI_DIR)/Device
WIFI_DEVICE_PREFIX := $(WIFI_PREFIX)Device_
WIFI_DEVICE_OBJ := $(WIFI_OBJ)Device_
OBJECTS_WIFI_DEVICE := \
  $(WIFI_DEVICE_OBJ)Controller.o \
  $(WIFI_DEVICE_OBJ)Module.o \
  $(WIFI_DEVICE_OBJ)Listener.o \
  $(WIFI_DEVICE_OBJ)Reader.o

################ Signal Handlers  ################

WIFI_SIGNAL_DIR := $(WIFI_DIR)/Signal
WIFI_SIGNAL_PREFIX := $(WIFI_PREFIX)Signal_
WIFI_SIGNAL_OBJ := $(WIFI_OBJ)Signal_
OBJECTS_WIFI_SIGNAL := \
  $(WIFI_SIGNAL_OBJ)APModule.o \
  $(WIFI_SIGNAL_OBJ)DeviceModule.o \
  $(WIFI_SIGNAL_OBJ)ClientModule.o

################ UI Components  ################

WIFI_COMPONENT_DIR := $(WIFI_DIR)/Component

WIFI_CONNECTION_LIST_DIR := $(WIFI_COMPONENT_DIR)/ConnectionList
WIFI_CONNECTION_LIST_PREFIX := $(WIFI_PREFIX)ConnectionList_
WIFI_CONNECTION_LIST_OBJ := $(WIFI_OBJ)ConnectionList_
OBJECTS_WIFI_CONNECTION_LIST := \
  $(WIFI_CONNECTION_LIST_OBJ)ListButton.o \
  $(WIFI_CONNECTION_LIST_OBJ)ControlComponent.o \
  $(WIFI_CONNECTION_LIST_OBJ)ListComponent.o

OBJECTS_WIFI_COMPONENT := \
  $(WIFI_OBJ)ControlWidget.o \
  $(WIFI_OBJ)SignalIcon.o \
  $(WIFI_OBJ)ConnectionIcon.o \
  $(OBJECTS_WIFI_CONNECTION_LIST)

################ Main Wifi Module  ################

OBJECTS_WIFI := \
  $(WIFI_OBJ)Resource.o \
  $(WIFI_OBJ)Module.o \
  $(WIFI_OBJ)AccessPoint.o \
  $(WIFI_OBJ)DebugOutput.o \
  $(OBJECTS_NM) \
  $(OBJECTS_WIFI_ACCESS_POINT) \
  $(OBJECTS_WIFI_ACCESS_POINT_LIST) \
  $(OBJECTS_WIFI_CONNECTION) \
  $(OBJECTS_WIFI_DEVICE) \
  $(OBJECTS_WIFI_SIGNAL) \
  $(OBJECTS_WIFI_COMPONENT)

WIFI_TEST_DIR = Tests/System/Wifi

WIFI_TESTUTILS_DIR := $(WIFI_TEST_DIR)/TestUtils
WIFI_TESTUTILS_PREFIX := $(WIFI_PREFIX)TestUtils_
WIFI_TESTUTILS_OBJ := $(WIFI_OBJ)TestUtils_

OBJECTS_WIFI_TESTUTILS := \
  $(WIFI_TESTUTILS_OBJ)ConnectionListener.o \
  $(WIFI_TESTUTILS_OBJ)Waiting.o
                          
OBJECTS_WIFI_TEST := \
  $(OBJECTS_WIFI_TESTUTILS) \
  $(WIFI_OBJ)APList_ListTest.o \
  $(WIFI_OBJ)Connection_Control_ControlTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIFI := $(OBJECTS_WIFI) $(OBJECTS_WIFI_TEST)
endif

MODULES := $(MODULES) wifi

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIFI)

wifi : $(OBJECTS_WIFI)
	@echo "Built Wifi module"

$(NM_BORROWED_OBJ)AccessPoint.o: \
    $(NM_BORROWED_DIR)/$(NM_PREFIX)AccessPoint.cpp
$(NM_BORROWED_OBJ)ActiveConnection.o: \
    $(NM_BORROWED_DIR)/$(NM_PREFIX)ActiveConnection.cpp
$(NM_BORROWED_OBJ)DeviceWifi.o: \
    $(NM_BORROWED_DIR)/$(NM_PREFIX)DeviceWifi.cpp

$(NM_OWNED_SETTINGS_OBJ)Object.o: \
    $(NM_OWNED_SETTINGS_DIR)/$(NM_OWNED_SETTINGS_PREFIX)Object.cpp
$(NM_OWNED_SETTINGS_OBJ)Connection.o: \
    $(NM_OWNED_SETTINGS_DIR)/$(NM_OWNED_SETTINGS_PREFIX)Connection.cpp
$(NM_OWNED_SETTINGS_OBJ)Wireless.o: \
    $(NM_OWNED_SETTINGS_DIR)/$(NM_OWNED_SETTINGS_PREFIX)Wireless.cpp
$(NM_OWNED_SETTINGS_OBJ)WirelessSecurity.o: \
    $(NM_OWNED_SETTINGS_DIR)/$(NM_OWNED_SETTINGS_PREFIX)WirelessSecurity.cpp

$(NM_OWNED_OBJ)Client.o: \
    $(NM_OWNED_DIR)/$(NM_PREFIX)Client.cpp
$(NM_OWNED_OBJ)Connection.o: \
    $(NM_OWNED_DIR)/$(NM_PREFIX)Connection.cpp

$(NM_THREAD_OBJ)Module.o: \
    $(NM_THREAD_DIR)/$(NM_THREAD_PREFIX)Module.cpp
$(NM_THREAD_OBJ)Handler.o: \
    $(NM_THREAD_DIR)/$(NM_THREAD_PREFIX)Handler.cpp

$(NM_DBUS_OBJ)SavedConnection.o: \
    $(NM_DBUS_DIR)/$(NM_DBUS_PREFIX)SavedConnection.cpp
$(NM_DBUS_OBJ)SavedConnectionLoader.o: \
    $(NM_DBUS_DIR)/$(NM_DBUS_PREFIX)SavedConnectionLoader.cpp

$(NM_SIGNAL_OBJ)APHandler.o: \
    $(NM_SIGNAL_DIR)/$(NM_SIGNAL_PREFIX)APHandler.cpp
$(NM_SIGNAL_OBJ)ClientHandler.o: \
    $(NM_SIGNAL_DIR)/$(NM_SIGNAL_PREFIX)ClientHandler.cpp
$(NM_SIGNAL_OBJ)DeviceHandler.o: \
    $(NM_SIGNAL_DIR)/$(NM_SIGNAL_PREFIX)DeviceHandler.cpp

$(NM_OBJ)APHash.o: \
    $(NM_DIR)/$(NM_PREFIX)APHash.cpp
$(NM_OBJ)SSID.o: \
    $(NM_DIR)/$(NM_PREFIX)SSID.cpp

$(WIFI_AP_OBJ)Data.o : \
    $(WIFI_AP_DIR)/$(WIFI_AP_PREFIX)Data.cpp
$(WIFI_AP_OBJ)StrengthListener.o : \
    $(WIFI_AP_DIR)/$(WIFI_AP_PREFIX)StrengthListener.cpp

$(WIFI_AP_LIST_OBJ)Module.o : \
    $(WIFI_AP_LIST_DIR)/$(WIFI_AP_LIST_PREFIX)Module.cpp
$(WIFI_AP_LIST_OBJ)Listener.o : \
    $(WIFI_AP_LIST_DIR)/$(WIFI_AP_LIST_PREFIX)Listener.cpp
$(WIFI_AP_LIST_OBJ)Reader.o : \
    $(WIFI_AP_LIST_DIR)/$(WIFI_AP_LIST_PREFIX)Reader.cpp
$(WIFI_AP_LIST_OBJ)NMReader.o : \
    $(WIFI_AP_LIST_DIR)/$(WIFI_AP_LIST_PREFIX)NMReader.cpp
$(WIFI_AP_LIST_OBJ)Writer.o : \
    $(WIFI_AP_LIST_DIR)/$(WIFI_AP_LIST_PREFIX)Writer.cpp

$(WIFI_CONNECTION_OBJ)Event.o : \
    $(WIFI_CONNECTION_DIR)/Wifi_Connection_Event.cpp

$(WIFI_CONNECTION_RECORD_OBJ)Module.o : \
    $(WIFI_CONNECTION_RECORD_DIR)/$(WIFI_CONNECTION_RECORD_PREFIX)Module.cpp
$(WIFI_CONNECTION_RECORD_OBJ)Reader.o : \
    $(WIFI_CONNECTION_RECORD_DIR)/$(WIFI_CONNECTION_RECORD_PREFIX)Reader.cpp
$(WIFI_CONNECTION_RECORD_OBJ)Writer.o : \
    $(WIFI_CONNECTION_RECORD_DIR)/$(WIFI_CONNECTION_RECORD_PREFIX)Writer.cpp
$(WIFI_CONNECTION_RECORD_OBJ)Listener.o : \
    $(WIFI_CONNECTION_RECORD_DIR)/$(WIFI_CONNECTION_RECORD_PREFIX)Listener.cpp

$(WIFI_CONNECTION_SAVED_OBJ)Module.o : \
    $(WIFI_CONNECTION_SAVED_DIR)/$(WIFI_CONNECTION_SAVED_PREFIX)Module.cpp
$(WIFI_CONNECTION_SAVED_OBJ)Reader.o : \
    $(WIFI_CONNECTION_SAVED_DIR)/$(WIFI_CONNECTION_SAVED_PREFIX)Reader.cpp
$(WIFI_CONNECTION_SAVED_OBJ)Deleter.o : \
    $(WIFI_CONNECTION_SAVED_DIR)/$(WIFI_CONNECTION_SAVED_PREFIX)Deleter.cpp

$(WIFI_CONNECTION_CONTROL_OBJ)Module.o : \
    $(WIFI_CONNECTION_CONTROL_DIR)/$(WIFI_CONNECTION_CONTROL_PREFIX)Module.cpp
$(WIFI_CONNECTION_CONTROL_OBJ)ModuleUpdater.o : \
    $(WIFI_CONNECTION_CONTROL_DIR)/$(WIFI_CONNECTION_CONTROL_PREFIX)ModuleUpdater.cpp
$(WIFI_CONNECTION_CONTROL_OBJ)Handler.o : \
    $(WIFI_CONNECTION_CONTROL_DIR)/$(WIFI_CONNECTION_CONTROL_PREFIX)Handler.cpp

$(WIFI_DEVICE_OBJ)Controller.o : \
    $(WIFI_DEVICE_DIR)/$(WIFI_DEVICE_PREFIX)Controller.cpp
$(WIFI_DEVICE_OBJ)Listener.o : \
    $(WIFI_DEVICE_DIR)/$(WIFI_DEVICE_PREFIX)Listener.cpp
$(WIFI_DEVICE_OBJ)Module.o : \
    $(WIFI_DEVICE_DIR)/$(WIFI_DEVICE_PREFIX)Module.cpp
$(WIFI_DEVICE_OBJ)Reader.o : \
    $(WIFI_DEVICE_DIR)/$(WIFI_DEVICE_PREFIX)Reader.cpp

$(WIFI_SIGNAL_OBJ)APModule.o : \
    $(WIFI_SIGNAL_DIR)/$(WIFI_SIGNAL_PREFIX)APModule.cpp
$(WIFI_SIGNAL_OBJ)DeviceModule.o : \
    $(WIFI_SIGNAL_DIR)/$(WIFI_SIGNAL_PREFIX)DeviceModule.cpp
$(WIFI_SIGNAL_OBJ)ClientModule.o : \
    $(WIFI_SIGNAL_DIR)/$(WIFI_SIGNAL_PREFIX)ClientModule.cpp

$(WIFI_CONNECTION_LIST_OBJ)ControlComponent.o : \
    $(WIFI_CONNECTION_LIST_DIR)/$(WIFI_CONNECTION_LIST_PREFIX)ControlComponent.cpp
$(WIFI_CONNECTION_LIST_OBJ)ListButton.o : \
    $(WIFI_CONNECTION_LIST_DIR)/$(WIFI_CONNECTION_LIST_PREFIX)ListButton.cpp
$(WIFI_CONNECTION_LIST_OBJ)ListComponent.o : \
    $(WIFI_CONNECTION_LIST_DIR)/$(WIFI_CONNECTION_LIST_PREFIX)ListComponent.cpp

$(WIFI_OBJ)ControlWidget.o : \
    $(WIFI_COMPONENT_DIR)/$(WIFI_PREFIX)ControlWidget.cpp
$(WIFI_OBJ)SignalIcon.o : \
    $(WIFI_COMPONENT_DIR)/$(WIFI_PREFIX)SignalIcon.cpp
$(WIFI_OBJ)ConnectionIcon.o : \
    $(WIFI_COMPONENT_DIR)/$(WIFI_PREFIX)ConnectionIcon.cpp

$(WIFI_TESTUTILS_OBJ)ConnectionListener.o : \
    $(WIFI_TESTUTILS_DIR)/$(WIFI_TESTUTILS_PREFIX)ConnectionListener.cpp
$(WIFI_TESTUTILS_OBJ)Waiting.o : \
    $(WIFI_TESTUTILS_DIR)/$(WIFI_TESTUTILS_PREFIX)Waiting.cpp

$(WIFI_OBJ)APList_ListTest.o : \
    $(WIFI_TEST_DIR)/$(WIFI_PREFIX)APList_ListTest.cpp
$(WIFI_OBJ)Connection_Control_ControlTest.o : \
    $(WIFI_TEST_DIR)/$(WIFI_PREFIX)Connection_Control_ControlTest.cpp

$(WIFI_OBJ)Resource.o : \
    $(WIFI_DIR)/$(WIFI_PREFIX)Resource.cpp
$(WIFI_OBJ)Module.o : \
    $(WIFI_DIR)/$(WIFI_PREFIX)Module.cpp
$(WIFI_OBJ)ThreadHandler.o : \
    $(WIFI_OBJ)/$(WIFI_PREFIX)ThreadHandler.cpp
$(WIFI_OBJ)AccessPoint.o : \
    $(WIFI_DIR)/$(WIFI_PREFIX)AccessPoint.cpp
$(WIFI_OBJ)DebugOutput.o : \
    $(WIFI_DIR)/$(WIFI_PREFIX)DebugOutput.cpp
