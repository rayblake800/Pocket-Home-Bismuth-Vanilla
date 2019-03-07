########################## Utility Module ######################################
UTIL_PREFIX := $(JUCE_OBJDIR)/Util_
UTIL_ROOT = Source/Util
UTIL_TEST_ROOT = Tests/Util

OBJECTS_UTIL := \
  $(UTIL_PREFIX)Utils.o \
  $(UTIL_PREFIX)TempTimer.o \
  $(UTIL_PREFIX)ShutdownListener.o \
  $(UTIL_PREFIX)JSONFile.o \
  $(UTIL_PREFIX)XDGDirectories.o \
  $(UTIL_PREFIX)AssetFiles.o \
  $(UTIL_PREFIX)XPMLoader.o \
  $(UTIL_PREFIX)WindowFocus.o \
  $(UTIL_PREFIX)WindowFocusedTimer.o 

OBJECTS_UTIL_TEST := \
  $(UTIL_PREFIX)ShutdownListenerTest.o 


ifeq ($(BUILD_TESTS), 1)
    OBJECTS_UTIL := $(OBJECTS_UTIL) $(OBJECTS_UTIL_TEST)
endif

MODULES := $(MODULES) util

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_UTIL)

util : $(OBJECTS_UTIL)
	@echo "Built Utility module"

$(UTIL_PREFIX)Utils.o : \
    $(UTIL_ROOT)/Utils.cpp
$(UTIL_PREFIX)TempTimer.o : \
    $(UTIL_ROOT)/TempTimer.cpp
$(UTIL_PREFIX)ShutdownListener.o : \
    $(UTIL_ROOT)/ShutdownListener.cpp
$(UTIL_PREFIX)JSONFile.o : \
    $(UTIL_ROOT)/Files/JSONFile.cpp
$(UTIL_PREFIX)XDGDirectories.o : \
    $(UTIL_ROOT)/Files/XDGDirectories.cpp
$(UTIL_PREFIX)AssetFiles.o : \
    $(UTIL_ROOT)/Files/AssetFiles.cpp
$(UTIL_PREFIX)XPMLoader.o : \
    $(UTIL_ROOT)/Files/XPMLoader.cpp
$(UTIL_PREFIX)WindowFocus.o : \
    $(UTIL_ROOT)/WindowFocus/WindowFocus.cpp
$(UTIL_PREFIX)WindowFocusedTimer.o : \
    $(UTIL_ROOT)/WindowFocus/WindowFocusedTimer.cpp

$(UTIL_PREFIX)ShutdownListenerTest.o : \
    $(UTIL_TEST_ROOT)/ShutdownListenerTest.cpp
