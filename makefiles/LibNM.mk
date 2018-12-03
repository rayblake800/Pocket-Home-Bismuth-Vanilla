############################### LibNM Module ###################################
NM_PREFIX := $(JUCE_OBJDIR)/LibNM_
NM_ROOT = Source/LibNM

OBJECTS_NM := \
  $(NM_PREFIX)ThreadResource.o \
  $(NM_PREFIX)ThreadHandler.o \
  $(NM_PREFIX)Object.o \
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

$(NM_PREFIX)ThreadResource.o : \
    $(NM_ROOT)/ThreadResource.cpp
$(NM_PREFIX)ThreadHandler.o : \
    $(NM_ROOT)/ThreadHandler.cpp
$(NM_PREFIX)Object.o : \
    $(NM_ROOT)/NMObjects/Object.cpp
$(NM_PREFIX)Client.o : \
    $(NM_ROOT)/NMObjects/Client.cpp
$(NM_PREFIX)Connection.o : \
    $(NM_ROOT)/NMObjects/Connection.cpp
$(NM_PREFIX)ActiveConnection.o : \
    $(NM_ROOT)/NMObjects/ActiveConnection.cpp
$(NM_PREFIX)DeviceWifi.o : \
    $(NM_ROOT)/NMObjects/DeviceWifi.cpp
$(NM_PREFIX)AccessPoint.o : \
    $(NM_ROOT)/NMObjects/AccessPoint.cpp
$(NM_PREFIX)SavedConnection.o : \
    $(NM_ROOT)/DBus/SavedConnection.cpp
$(NM_PREFIX)SavedConnectionLoader.o : \
    $(NM_ROOT)/DBus/SavedConnectionLoader.cpp
