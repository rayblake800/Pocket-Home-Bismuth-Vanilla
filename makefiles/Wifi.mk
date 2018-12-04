########################### Wifi Module ########################################
WIFI_PREFIX := $(JUCE_OBJDIR)/Wifi_
WIFI_ROOT = Source/Wifi
WIFI_TEST_ROOT = Tests/Wifi

SAVED_STATE_ROOT := $(WIFI_ROOT)/SavedState
OBJECTS_WIFI_SAVED_STATE := \
  $(WIFI_PREFIX)DeviceTracker.o \
  $(WIFI_PREFIX)APData.o \
  $(WIFI_PREFIX)AccessPoint.o

CONTROL_ROOT := $(WIFI_ROOT)/Control
OBJECTS_WIFI_CONTROL := \
  $(WIFI_PREFIX)Controller.o

LISTENER_ROOT := $(WIFI_ROOT)/Listeners
OBJECTS_WIFI_LISTENER := \
  $(WIFI_PREFIX)DeviceListener.o

COMPONENT_ROOT := $(WIFI_ROOT)/Component
OBJECTS_WIFI_COMPONENT := \
  $(WIFI_PREFIX)ControlWidget.o \
  $(WIFI_PREFIX)StatusIcon.o \
  $(WIFI_PREFIX)SettingsPage.o

OBJECTS_WIFI := \
  $(OBJECTS_WIFI_SAVED_STATE) \
  $(OBJECTS_WIFI_CONTROL) \
  $(OBJECTS_WIFI_LISTENER) \
  $(OBJECTS_WIFI_COMPONENT)

OBJECTS_WIFI_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIFI := $(OBJECTS_WIFI) $(OBJECTS_WIFI_TEST)
endif

MODULES := $(MODULES) wifi

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIFI)

wifi : $(OBJECTS_WIFI)
	@echo "Built Wifi module"

$(WIFI_PREFIX)DeviceTracker.o : \
    $(SAVED_STATE_ROOT)/DeviceTracker.cpp
$(WIFI_PREFIX)APData.o : \
    $(SAVED_STATE_ROOT)/APData.cpp
$(WIFI_PREFIX)AccessPoint.o : \
    $(SAVED_STATE_ROOT)/AccessPoint.cpp

$(WIFI_PREFIX)Controller.o : \
    $(CONTROL_ROOT)/Controller.cpp

$(WIFI_PREFIX)DeviceListener.o : \
    $(LISTENER_ROOT)/DeviceListener.cpp

$(WIFI_PREFIX)ControlWidget.o : \
    $(COMPONENT_ROOT)/ControlWidget.cpp
$(WIFI_PREFIX)StatusIcon.o : \
    $(COMPONENT_ROOT)/StatusIcon.cpp
$(WIFI_PREFIX)SettingsPage.o : \
    $(COMPONENT_ROOT)/SettingsPage.cpp
