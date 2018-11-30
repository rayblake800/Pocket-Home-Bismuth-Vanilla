########################### Wifi Module ########################################
WIFI_PREFIX := $(JUCE_OBJDIR)/Wifi_
WIFI_ROOT = Source/Wifi
WIFI_TEST_ROOT = Tests/Wifi

OBJECTS_WIFI := \
  $(WIFI_PREFIX)APHash.o \
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

MODULES := $(MODULES) wifi

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIFI)

wifi : $(OBJECTS_WIFI)
	@echo "Built Wifi module"

$(WIFI_PREFIX)APHash.o : \
    $(WIFI_ROOT)/APHash.cpp
$(WIFI_PREFIX)StateManager.o : \
    $(WIFI_ROOT)/WifiStateManager.cpp
$(WIFI_PREFIX)AccessPoint.o : \
    $(WIFI_ROOT)/WifiAccessPoint.cpp
$(WIFI_PREFIX)NetworkInterface.o : \
    $(WIFI_ROOT)/NetworkInterface.cpp
$(WIFI_PREFIX)LibNMInterface.o : \
    $(WIFI_ROOT)/LibNMInterface.cpp
$(WIFI_PREFIX)JsonInterface.o : \
    $(WIFI_ROOT)/JsonWifiInterface.cpp
$(WIFI_PREFIX)SettingsComponent.o : \
    $(WIFI_ROOT)/WifiSettingsComponent.cpp
$(WIFI_PREFIX)SettingsPage.o : \
    $(WIFI_ROOT)/WifiSettingsPage.cpp
$(WIFI_PREFIX)Icon.o : \
    $(WIFI_ROOT)/WifiIcon.cpp

# Tests:
$(WIFI_PREFIX)Test_StateManager.o : \
    $(WIFI_TEST_ROOT)/WifiStateManager.cpp
