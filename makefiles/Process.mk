############################### Process Module #################################
PROCESS_PREFIX := $(JUCE_OBJDIR)/Process_
PROCESS_ROOT = Source/Process
PROCESS_TEST_ROOT = Tests/Process

OBJECTS_PROCESS := \
  $(PROCESS_PREFIX)Reader.o \
  $(PROCESS_PREFIX)Launched.o \
  $(PROCESS_PREFIX)Launcher.o  

OBJECTS_PROCESS_TEST := \
  $(PROCESS_PREFIX)Test_Launched.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PROCESS := $(OBJECTS_PROCESS) $(OBJECTS_PROCESS_TEST)
endif

MODULES := $(MODULES) process

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PROCESS)

process : $(OBJECTS_PROCESS)
	@echo "Built Process module"

$(PROCESS_PREFIX)Reader.o : \
    $(PROCESS_ROOT)/ProcessUtils.cpp
$(PROCESS_PREFIX)Launched.o : \
    $(PROCESS_ROOT)/LaunchedProcess.cpp
$(PROCESS_PREFIX)Launcher.o : \
    $(PROCESS_ROOT)/AppLauncher.cpp
$(PROCESS_PREFIX)Launched.o : \
    $(PROCESS_ROOT)/LaunchedProcess.cpp

# Tests:
$(PROCESS_PREFIX)Test_Launched.o : \
    $(PROCESS_TEST_ROOT)/LaunchedProcess.cpp
