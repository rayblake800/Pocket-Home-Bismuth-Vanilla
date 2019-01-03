############################### LibNM Module ###################################
NM_PREFIX := $(JUCE_OBJDIR)/LibNM_
NM_ROOT = Source/LibNM

NM_BORROWED_ROOT := $(NM_ROOT)/BorrowedObjects
OBJECTS_NM_BORROWED := \
  $(NM_PREFIX)AccessPoint.o \
  $(NM_PREFIX)ActiveConnection.o \
  $(NM_PREFIX)DeviceWifi.o

NM_OWNED_ROOT := $(NM_ROOT)/OwnedObjects
OBJECTS_NM_OWNED := \
  $(NM_PREFIX)Client.o \
  $(NM_PREFIX)Connection.o

NM_SETTING_ROOT := $(NM_OWNED_ROOT)/Settings
OBJECTS_NM_SETTING := \
  $(NM_PREFIX)Settings.o \
  $(NM_PREFIX)ConnectionSettings.o \
  $(NM_PREFIX)WifiSettings.o \
  $(NM_PREFIX)WifiSecuritySettings.o

NM_DATA_ROOT := $(NM_ROOT)/Data
OBJECTS_NM_DATA := \
  $(NM_PREFIX)APHash.o \
  $(NM_PREFIX)SSID.o \

NM_DBUS_ROOT := $(NM_ROOT)/DBus
OBJECTS_NM_DBUS := \
  $(NM_PREFIX)SavedConnection.o \
  $(NM_PREFIX)SavedConnectionLoader.o  

OBJECTS_NM := \
  $(OBJECTS_NM_BORROWED) \
  $(OBJECTS_NM_OWNED) \
  $(OBJECTS_NM_SETTING) \
  $(OBJECTS_NM_DATA) \
  $(OBJECTS_NM_DBUS) \
  $(NM_PREFIX)ThreadResource.o \
  $(NM_PREFIX)ThreadHandler.o

OBJECTS_NM_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_NM := $(OBJECTS_NM) $(OBJECTS_NM_TEST)
endif

MODULES := $(MODULES) libNM

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_NM)

libNM : $(OBJECTS_NM)
	@echo "Built LibNM module"

$(NM_PREFIX)ThreadResource.o : \
    $(NM_ROOT)/ThreadResource.cpp
$(NM_PREFIX)ThreadHandler.o : \
    $(NM_ROOT)/ThreadHandler.cpp

$(NM_PREFIX)AccessPoint.o : \
    $(NM_BORROWED_ROOT)/AccessPoint.cpp
$(NM_PREFIX)ActiveConnection.o : \
    $(NM_BORROWED_ROOT)/ActiveConnection.cpp
$(NM_PREFIX)DeviceWifi.o : \
    $(NM_BORROWED_ROOT)/DeviceWifi.cpp

$(NM_PREFIX)Client.o : \
    $(NM_OWNED_ROOT)/Client.cpp
$(NM_PREFIX)Connection.o : \
    $(NM_OWNED_ROOT)/Connection.cpp

$(NM_PREFIX)Settings.o : \
    $(NM_SETTING_ROOT)/Settings.cpp
$(NM_PREFIX)ConnectionSettings.o : \
    $(NM_SETTING_ROOT)/ConnectionSettings.cpp
$(NM_PREFIX)WifiSettings.o : \
    $(NM_SETTING_ROOT)/WifiSettings.cpp
$(NM_PREFIX)WifiSecuritySettings.o : \
    $(NM_SETTING_ROOT)/WifiSecuritySettings.cpp

$(NM_PREFIX)APHash.o : \
    $(NM_DATA_ROOT)/APHash.cpp
$(NM_PREFIX)SSID.o : \
    $(NM_DATA_ROOT)/SSID.cpp

$(NM_PREFIX)SavedConnection.o : \
    $(NM_DBUS_ROOT)/SavedConnection.cpp
$(NM_PREFIX)SavedConnectionLoader.o : \
    $(NM_DBUS_ROOT)/SavedConnectionLoader.cpp
