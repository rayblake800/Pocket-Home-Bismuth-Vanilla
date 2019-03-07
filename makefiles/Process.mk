############################### Process Module #################################
PROCESS_ROOT = Source/Process
PROCESS_PREFIX := Process_
PROCESS_OBJ := $(JUCE_OBJDIR)/$(PROCESS_PREFIX)

OBJECTS_PROCESS := \
  $(PROCESS_OBJ)State.o \
  $(PROCESS_OBJ)Data.o \
  $(PROCESS_OBJ)Launched.o \
  $(PROCESS_OBJ)Launcher.o  

PROCESS_TEST_ROOT = Tests/Process
OBJECTS_PROCESS_TEST := \
  $(PROCESS_OBJ)LaunchedTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PROCESS := $(OBJECTS_PROCESS) $(OBJECTS_PROCESS_TEST)
endif

MODULES := $(MODULES) process

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PROCESS)

process : $(OBJECTS_PROCESS)
	@echo "Built Process module"

$(PROCESS_OBJ)State.o : \
    $(PROCESS_ROOT)/$(PROCESS_PREFIX)State.cpp
$(PROCESS_OBJ)Data.o : \
    $(PROCESS_ROOT)/$(PROCESS_PREFIX)Data.cpp
$(PROCESS_OBJ)Launched.o : \
    $(PROCESS_ROOT)/$(PROCESS_PREFIX)Launched.cpp
$(PROCESS_OBJ)Launcher.o : \
    $(PROCESS_ROOT)/$(PROCESS_PREFIX)Launcher.cpp

# Tests:
$(PROCESS_OBJ)LaunchedTest.o : \
    $(PROCESS_TEST_ROOT)/$(PROCESS_PREFIX)LaunchedTest.cpp
