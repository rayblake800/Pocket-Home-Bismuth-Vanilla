############################### LibNM Module ###################################
NM_PREFIX := $(JUCE_OBJDIR)/LibNM_
NM_ROOT = Source/LibNM

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

MODULES := $(MODULES) libNM

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_NM)

libNM : $(OBJECTS_NM)
	@echo "Built LibNM module"

$(NM_PREFIX)Client.o : \
    $(NM_ROOT)/NMPPClient.cpp
$(NM_PREFIX)Connection.o : \
    $(NM_ROOT)/NMPPConnection.cpp
$(NM_PREFIX)ActiveConnection.o : \
    $(NM_ROOT)/NMPPActiveConnection.cpp
$(NM_PREFIX)DeviceWifi.o : \
    $(NM_ROOT)/NMPPDeviceWifi.cpp
$(NM_PREFIX)AccessPoint.o : \
    $(NM_ROOT)/NMPPAccessPoint.cpp
$(NM_PREFIX)SavedConnection.o : \
    $(NM_ROOT)/SavedConnection.cpp
$(NM_PREFIX)SavedConnectionLoader.o : \
    $(NM_ROOT)/SavedConnections.cpp
