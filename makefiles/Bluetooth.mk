########################## Bluetooth Module ####################################
BLUETOOTH_PREFIX := $(JUCE_OBJDIR)/Bluetooth_
BLUETOOTH_ROOT = Source/Bluetooth

OBJECTS_BLUETOOTH := \
  $(BLUETOOTH_PREFIX)BluezAdapter.o \
  $(BLUETOOTH_PREFIX)Status.o \
  $(BLUETOOTH_PREFIX)Device.o \
  $(BLUETOOTH_PREFIX)SettingsComponent.o \
  $(BLUETOOTH_PREFIX)SettingsPage.o 

OBJECTS_BLUETOOTH_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_BLUETOOTH := $(OBJECTS_BLUETOOTH) $(OBJECTS_BLUETOOTH_TEST)
endif

MODULES := $(MODULES) bluetooth

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_BLUETOOTH)

bluetooth : $(OBJECTS_BLUETOOTH)
	@echo "Built Bluetooth module"

$(BLUETOOTH_PREFIX)BluezAdapter.o : \
    $(BLUETOOTH_ROOT)/BluezAdapter.cpp
$(BLUETOOTH_PREFIX)Status.o : \
    $(BLUETOOTH_ROOT)/BluetoothStatus.cpp
$(BLUETOOTH_PREFIX)Device.o : \
    $(BLUETOOTH_ROOT)/BluetoothDevice.cpp
$(BLUETOOTH_PREFIX)SettingsComponent.o : \
    $(BLUETOOTH_ROOT)/BluetoothSettingsComponent.cpp
$(BLUETOOTH_PREFIX)SettingsPage.o : \
    $(BLUETOOTH_ROOT)/BluetoothSettingsPage.cpp
