##########################  Hardware Module  ###################################
HARDWARE_DIR = Source/System/Hardware
HARDWARE_TEST_DIR = Tests/System/Hardware

HARDWARE_PREFIX = Hardware_
HARDWARE_OBJ := $(JUCE_OBJDIR)/$(HARDWARE_PREFIX)

OBJECTS_HARDWARE := \
  $(HARDWARE_OBJ)Audio.o \
  $(HARDWARE_OBJ)BatteryMonitor.o \
  $(HARDWARE_OBJ)Display.o \
  $(HARDWARE_OBJ)I2CBus.o

HARDWARE_TEST_PREFIX := $(HARDWARE_PREFIX)Test_
HARDWARE_TEST_OBJ := $(HARDWARE_OBJ)Test_
OBJECTS_HARDWARE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_HARDWARE := $(OBJECTS_HARDWARE) $(OBJECTS_HARDWARE_TEST)
endif

MODULES := $(MODULES) hardware

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_HARDWARE)

hardware : $(OBJECTS_HARDWARE)
	@echo "Built Hardware module"

$(HARDWARE_OBJ)Audio.o : \
    $(HARDWARE_DIR)/Audio.cpp
$(HARDWARE_OBJ)BatteryMonitor.o : \
    $(HARDWARE_DIR)/BatteryMonitor.cpp
$(HARDWARE_OBJ)Display.o : \
    $(HARDWARE_DIR)/Display.cpp
$(HARDWARE_OBJ)I2CBus.o : \
    $(HARDWARE_DIR)/I2CBus.cpp
