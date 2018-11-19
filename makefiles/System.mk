############################ System Module ##################################### 
SYSTEM_PREFIX := $(JUCE_OBJDIR)/System_
SYSTEM_ROOT = Source/System
SYSTEM_TEST_ROOT = Tests/System

OBJECTS_SYSTEM := \
  $(SYSTEM_PREFIX)Commands.o \
  $(SYSTEM_PREFIX)XWindowInterface.o \
  $(SYSTEM_PREFIX)Audio.o \
  $(SYSTEM_PREFIX)Display.o \
  $(SYSTEM_PREFIX)I2CBus.o \
  $(SYSTEM_PREFIX)BatteryMonitor.o \
  $(SYSTEM_PREFIX)BatteryIcon.o \
  $(SYSTEM_PREFIX)PowerPage.o \
  $(SYSTEM_PREFIX)FelPage.o  
  
OBJECTS_SYSTEM_TEST := \
  $(SYSTEM_PREFIX)Test_XWindowInterface.o  

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SYSTEM := $(OBJECTS_SYSTEM) $(OBJECTS_SYSTEM_TEST)
endif

MODULES := $(MODULES) system

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SYSTEM)

system : $(OBJECTS_SYSTEM)
	@echo "Built System module"

$(SYSTEM_PREFIX)Commands.o : \
    $(SYSTEM_ROOT)/SystemCommands.cpp
$(SYSTEM_PREFIX)XWindowInterface.o : \
    $(SYSTEM_ROOT)/XWindowInterface.cpp
$(SYSTEM_PREFIX)Audio.o : \
    $(SYSTEM_ROOT)/Audio.cpp
$(SYSTEM_PREFIX)Display.o : \
    $(SYSTEM_ROOT)/Display.cpp
$(SYSTEM_PREFIX)I2CBus.o : \
    $(SYSTEM_ROOT)/I2CBus.cpp
$(SYSTEM_PREFIX)BatteryMonitor.o : \
    $(SYSTEM_ROOT)/BatteryMonitor.cpp
$(SYSTEM_PREFIX)BatteryIcon.o : \
    $(SYSTEM_ROOT)/BatteryIcon.cpp
$(SYSTEM_PREFIX)PowerPage.o : \
    $(SYSTEM_ROOT)/PowerPage/PowerPage.cpp
$(SYSTEM_PREFIX)FelPage.o : \
    $(SYSTEM_ROOT)/PowerPage/FelPage.cpp

# Tests:
$(SYSTEM_PREFIX)Test_XWindowInterface.o : \
    $(SYSTEM_TEST_ROOT)/XWindowInterface.cpp
