########################### Wifi Module ########################################
WIFI_PREFIX := $(JUCE_OBJDIR)/Wifi_

WIFI_ROOT = Source/Wifi
WIFI_TEST_ROOT = Tests/Wifi

WIFI_AP_ROOT := $(WIFI_ROOT)/AP
WIFI_AP_PREFIX := $(WIFI_PREFIX)AP_
OBJECTS_WIFI_ACCESS_POINT := \
  $(WIFI_AP_PREFIX)Data.o \
  $(WIFI_AP_PREFIX)StrengthListener.o

WIFI_AP_LIST_ROOT := $(WIFI_ROOT)/APList
WIFI_AP_LIST_PREFIX := $(WIFI_PREFIX)APList_
OBJECTS_WIFI_ACCESS_POINT_LIST := \
  $(WIFI_AP_LIST_PREFIX)ListResource.o \
  $(WIFI_AP_LIST_PREFIX)Listener.o \
  $(WIFI_AP_LIST_PREFIX)Reader.o \
  $(WIFI_AP_LIST_PREFIX)NMReader.o \
  $(WIFI_AP_LIST_PREFIX)Writer.o

WIFI_CONNECTION_ROOT := $(WIFI_ROOT)/Connection
WIFI_CONNECTION_PREFIX := $(WIFI_PREFIX)Connection_
OBJECTS_WIFI_CONNECTION := \
  $(WIFI_CONNECTION_PREFIX)Controller.o \
  $(WIFI_CONNECTION_PREFIX)Event.o \
  $(WIFI_CONNECTION_PREFIX)RecordResource.o \
  $(WIFI_CONNECTION_PREFIX)RecordReader.o \
  $(WIFI_CONNECTION_PREFIX)RecordWriter.o \
  $(WIFI_CONNECTION_PREFIX)Listener.o

WIFI_DEVICE_ROOT := $(WIFI_ROOT)/Device
WIFI_DEVICE_PREFIX := $(WIFI_PREFIX)Device_
OBJECTS_WIFI_DEVICE := \
  $(WIFI_DEVICE_PREFIX)Controller.o \
  $(WIFI_DEVICE_PREFIX)Listener.o \
  $(WIFI_DEVICE_PREFIX)Tracker.o \
  $(WIFI_DEVICE_PREFIX)Reader.o

WIFI_SIGNAL_HANDLER_ROOT := $(WIFI_ROOT)/NMSignals
WIFI_SIGNAL_HANDLER_PREFIX := $(WIFI_PREFIX)NMSignals_
OBJECTS_WIFI_SIGNAL_HANDLER := \
  $(WIFI_SIGNAL_HANDLER_PREFIX)APHandler.o \
  $(WIFI_SIGNAL_HANDLER_PREFIX)ClientHandler.o \
  $(WIFI_SIGNAL_HANDLER_PREFIX)DeviceHandler.o

WIFI_COMPONENT_ROOT := $(WIFI_ROOT)/Component
OBJECTS_WIFI_COMPONENT := \
  $(WIFI_PREFIX)ControlWidget.o \
  $(WIFI_PREFIX)StatusIcon.o \
  $(WIFI_PREFIX)SettingsPage.o

OBJECTS_WIFI := \
  $(WIFI_PREFIX)Manager.o \
  $(WIFI_PREFIX)AccessPoint.o \
  $(OBJECTS_WIFI_ACCESS_POINT) \
  $(OBJECTS_WIFI_ACCESS_POINT_LIST) \
  $(OBJECTS_WIFI_CONNECTION) \
  $(OBJECTS_WIFI_DEVICE) \
  $(OBJECTS_WIFI_SIGNAL_HANDLER) \
  $(OBJECTS_WIFI_COMPONENT)

OBJECTS_WIFI_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIFI := $(OBJECTS_WIFI) $(OBJECTS_WIFI_TEST)
endif

MODULES := $(MODULES) wifi

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIFI)

wifi : $(OBJECTS_WIFI)
	@echo "Built Wifi module"

$(WIFI_PREFIX)Manager.o : \
    $(WIFI_ROOT)/Wifi_Manager.cpp
$(WIFI_PREFIX)AccessPoint.o : \
    $(WIFI_ROOT)/Wifi_AccessPoint.cpp

$(WIFI_AP_PREFIX)Data.o : \
    $(WIFI_AP_ROOT)/Wifi_AP_Data.cpp
$(WIFI_AP_PREFIX)StrengthListener.o : \
    $(WIFI_AP_ROOT)/Wifi_AP_StrengthListener.cpp

$(WIFI_AP_LIST_PREFIX)ListResource.o : \
    $(WIFI_AP_LIST_ROOT)/Wifi_APList_ListResource.cpp
$(WIFI_AP_LIST_PREFIX)Listener.o : \
    $(WIFI_AP_LIST_ROOT)/Wifi_APList_Listener.cpp
$(WIFI_AP_LIST_PREFIX)Reader.o : \
    $(WIFI_AP_LIST_ROOT)/Wifi_APList_Reader.cpp
$(WIFI_AP_LIST_PREFIX)NMReader.o : \
    $(WIFI_AP_LIST_ROOT)/Wifi_APList_NMReader.cpp
$(WIFI_AP_LIST_PREFIX)Writer.o : \
    $(WIFI_AP_LIST_ROOT)/Wifi_APList_Writer.cpp

$(WIFI_CONNECTION_PREFIX)Controller.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_Controller.cpp
$(WIFI_CONNECTION_PREFIX)Event.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_Event.cpp
$(WIFI_CONNECTION_PREFIX)RecordResource.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_RecordResource.cpp
$(WIFI_CONNECTION_PREFIX)RecordReader.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_RecordReader.cpp
$(WIFI_CONNECTION_PREFIX)RecordWriter.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_RecordWriter.cpp
$(WIFI_CONNECTION_PREFIX)Listener.o : \
    $(WIFI_CONNECTION_ROOT)/Wifi_Connection_Listener.cpp

$(WIFI_DEVICE_PREFIX)Controller.o : \
    $(WIFI_DEVICE_ROOT)/Wifi_Device_Controller.cpp
$(WIFI_DEVICE_PREFIX)Listener.o : \
    $(WIFI_DEVICE_ROOT)/Wifi_Device_Listener.cpp
$(WIFI_DEVICE_PREFIX)Tracker.o : \
    $(WIFI_DEVICE_ROOT)/Wifi_Device_Tracker.cpp
$(WIFI_DEVICE_PREFIX)Reader.o : \
    $(WIFI_DEVICE_ROOT)/Wifi_Device_Reader.cpp

$(WIFI_SIGNAL_HANDLER_PREFIX)APHandler.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/Wifi_NMSignals_APHandler.cpp
$(WIFI_SIGNAL_HANDLER_PREFIX)ClientHandler.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/Wifi_NMSignals_ClientHandler.cpp
$(WIFI_SIGNAL_HANDLER_PREFIX)DeviceHandler.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/Wifi_NMSignals_DeviceHandler.cpp

$(WIFI_PREFIX)ControlWidget.o : \
    $(WIFI_COMPONENT_ROOT)/Wifi_ControlWidget.cpp
$(WIFI_PREFIX)SettingsPage.o : \
    $(WIFI_COMPONENT_ROOT)/Wifi_SettingsPage.cpp
$(WIFI_PREFIX)StatusIcon.o : \
    $(WIFI_COMPONENT_ROOT)/Wifi_StatusIcon.cpp
