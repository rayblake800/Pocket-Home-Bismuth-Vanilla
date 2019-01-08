########################### Wifi Module ########################################
WIFI_PREFIX := $(JUCE_OBJDIR)/Wifi_

WIFI_ROOT = Source/Wifi
WIFI_TEST_ROOT = Tests/Wifi

WIFI_AP_ROOT := $(WIFI_ROOT)/AccessPoint
OBJECTS_WIFI_ACCESS_POINT := \
  $(WIFI_PREFIX)APData.o \
  $(WIFI_PREFIX)AccessPoint.o \
  $(WIFI_PREFIX)SignalStrengthListener.o

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
  $(WIFI_DEVICE_PREFIX)DeviceListener.o \
  $(WIFI_DEVICE_PREFIX)DeviceTracker.o \
  $(WIFI_DEVICE_PREFIX)DeviceViewer.o

WIFI_SIGNAL_HANDLER_ROOT := $(WIFI_ROOT)/NMSignalHandlers
WIFI_SIGNAL_HANDLER_PREFIX := $(WIFI_PREFIX)Signal_Handler_
OBJECTS_WIFI_SIGNAL_HANDLER := \
  $(WIFI_SIGNAL_HANDLER_PREFIX)AccessPoint.o \
  $(WIFI_SIGNAL_HANDLER_PREFIX)Client.o \
  $(WIFI_SIGNAL_HANDLER_PREFIX)Device.o

WIFI_COMPONENT_ROOT := $(WIFI_ROOT)/Component
OBJECTS_WIFI_COMPONENT := \
  $(WIFI_PREFIX)ControlWidget.o \
  $(WIFI_PREFIX)StatusIcon.o \
  $(WIFI_PREFIX)SettingsPage.o

OBJECTS_WIFI := \
  $(WIFI_PREFIX)Manager.o \
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
    $(WIFI_ROOT)/Manager.cpp

$(WIFI_PREFIX)APData.o : \
    $(WIFI_AP_ROOT)/APData.cpp
$(WIFI_PREFIX)AccessPoint.o : \
    $(WIFI_AP_ROOT)/AccessPoint.cpp
$(WIFI_PREFIX)SignalStrengthListener.o : \
    $(WIFI_AP_ROOT)/SignalStrengthListener.cpp

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
    $(WIFI_CONNECTION_ROOT)/Controller.cpp
$(WIFI_CONNECTION_PREFIX)Event.o : \
    $(WIFI_CONNECTION_ROOT)/Event.cpp
$(WIFI_CONNECTION_PREFIX)RecordResource.o : \
    $(WIFI_CONNECTION_ROOT)/RecordResource.cpp
$(WIFI_CONNECTION_PREFIX)RecordReader.o : \
    $(WIFI_CONNECTION_ROOT)/RecordReader.cpp
$(WIFI_CONNECTION_PREFIX)RecordWriter.o : \
    $(WIFI_CONNECTION_ROOT)/RecordWriter.cpp
$(WIFI_CONNECTION_PREFIX)Listener.o : \
    $(WIFI_CONNECTION_ROOT)/Listener.cpp

$(WIFI_DEVICE_PREFIX)Controller.o : \
    $(WIFI_DEVICE_ROOT)/Controller.cpp
$(WIFI_DEVICE_PREFIX)DeviceListener.o : \
    $(WIFI_DEVICE_ROOT)/DeviceListener.cpp
$(WIFI_DEVICE_PREFIX)DeviceTracker.o : \
    $(WIFI_DEVICE_ROOT)/DeviceTracker.cpp
$(WIFI_DEVICE_PREFIX)DeviceViewer.o : \
    $(WIFI_DEVICE_ROOT)/DeviceViewer.cpp

$(WIFI_SIGNAL_HANDLER_PREFIX)AccessPoint.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/APSignalHandler.cpp
$(WIFI_SIGNAL_HANDLER_PREFIX)Client.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/ClientSignalHandler.cpp
$(WIFI_SIGNAL_HANDLER_PREFIX)Device.o : \
    $(WIFI_SIGNAL_HANDLER_ROOT)/DeviceSignalHandler.cpp

$(WIFI_PREFIX)ControlWidget.o : \
    $(WIFI_COMPONENT_ROOT)/ControlWidget.cpp
$(WIFI_PREFIX)SettingsPage.o : \
    $(WIFI_COMPONENT_ROOT)/SettingsPage.cpp
$(WIFI_PREFIX)StatusIcon.o : \
    $(WIFI_COMPONENT_ROOT)/StatusIcon.cpp
