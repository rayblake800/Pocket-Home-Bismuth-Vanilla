########################## Bluetooth Module ####################################
BLUETOOTH_DIR = Source/System/Bluetooth
BLUETOOTH_TEST_DIR = Tests/System/Bluetooth

BLUETOOTH_PREFIX = Bluetooth_
BLUETOOTH_OBJ := $(JUCE_OBJDIR)/$(BLUETOOTH_PREFIX)

OBJECTS_BLUETOOTH := \
  $(BLUETOOTH_OBJ)BluezAdapter.o 

OBJECTS_BLUETOOTH_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_BLUETOOTH := $(OBJECTS_BLUETOOTH) $(OBJECTS_BLUETOOTH_TEST)
endif

SYSTEM_MODULES := $(SYSTEM_MODULES) bluetooth

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_BLUETOOTH)

bluetooth : $(OBJECTS_BLUETOOTH)
	@echo "    Built Bluetooth module"

$(BLUETOOTH_OBJ)BluezAdapter.o : \
    $(BLUETOOTH_DIR)/$(BLUETOOTH_PREFIX)BluezAdapter.cpp
