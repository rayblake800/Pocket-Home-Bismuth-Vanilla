############################### Process Module #################################
PROCESS_PREFIX := $(JUCE_OBJDIR)/Process_
PROCESS_ROOT = Source/Process
PROCESS_TEST_ROOT = Tests/Process

OBJECTS_PROCESS := \
  $(CONFIG_PREFIX)Reader.o \
  $(CONFIG_PREFIX)Launched.o \
  $(CONFIG_PREFIX)Launcher.o  

OBJECTS_PROCESS_TEST := \
  $(CONFIG_PREFIX)Test_Launched.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PROCESS := $(OBJECTS_PROCESS) $(OBJECTS_PROCESS_TEST)
endif

MODULES := $(MODULES) process

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PROCESS)

process : $(OBJECTS_PROCESS)
	@echo "Built Process module"

$(CONFIG_PREFIX)Reader.o : \
    $(PROCESS_ROOT)/ProcessUtils.cpp
$(CONFIG_PREFIX)Launched.o : \
    $(PROCESS_ROOT)/LaunchedProcess.cpp
$(CONFIG_PREFIX)Launcher.o : \
    $(PROCESS_ROOT)/AppLauncher.cpp
$(CONFIG_PREFIX)Launched.o : \
    $(PROCESS_ROOT)/LaunchedProcess.cpp

# Tests:
$(CONFIG_PREFIX)Test_Launched.o : \
    $(PROCESS_TEST_ROOT)/LaunchedProcess.cpp
