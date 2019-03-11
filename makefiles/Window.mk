############################  Window Module  ####################################
WINDOW_DIR = Source/System/Window
WINDOW_TEST_DIR = Tests/System/Window

WINDOW_PREFIX = Window_
WINDOW_OBJ := $(JUCE_OBJDIR)/$(WINDOW_PREFIX)
OBJECTS_WINDOW := \
  $(WINDOW_OBJ)Info.o \
  $(WINDOW_OBJ)Alert.o \
  $(WINDOW_OBJ)XWindowInterface.o \
  $(WINDOW_OBJ)WindowFocus.o \
  $(WINDOW_OBJ)WindowFocusedTimer.o 

WINDOW_TEST_PREFIX := $(WINDOW_PREFIX)Test_
WINDOW_TEST_OBJ := $(WINDOW_OBJ)Test_
OBJECTS_WINDOW_TEST := \
  $(WINDOW_TEST_OBJ)XWindowInterface.o  

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WINDOW := $(OBJECTS_WINDOW) $(OBJECTS_WINDOW_TEST)
endif

MODULES := $(MODULES) window

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WINDOW)

window : $(OBJECTS_WINDOW)
	@echo "Built Window module"

$(WINDOW_OBJ)Info.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)Info.cpp
$(WINDOW_OBJ)Alert.o : \
    $(WINDOW_DIR)/$(WINDOW_PREFIX)Alert.cpp
$(WINDOW_OBJ)XWindowInterface.o : \
    $(WINDOW_DIR)/XWindowInterface.cpp
$(WINDOW_OBJ)WindowFocus.o : \
    $(WINDOW_DIR)/WindowFocus.cpp
$(WINDOW_OBJ)WindowFocusedTimer.o : \
    $(WINDOW_DIR)/WindowFocusedTimer.cpp

$(WINDOW_TEST_OBJ)XWindowInterface.o : \
    $(WINDOW_TEST_DIR)/$(WINDOW_TEST_PREFIX)XWindowInterface.cpp
