#############################  Info Module  ####################################
INFO_DIR = Source/GUI/Info
INFO_TEST_DIR = Tests/GUI/Info

INFO_PREFIX = Info_
INFO_OBJ := $(JUCE_OBJDIR)/$(INFO_PREFIX)

OBJECTS_INFO := \
  $(INFO_OBJ)BatteryIcon.o \
  $(INFO_OBJ)Clock.o \
  $(INFO_OBJ)IPLabel.o

ifeq ($(WIFI_SUPPORT), 1)
    OBJECTS_INFO := \
      $(OBJECTS_INFO) \
      $(INFO_OBJ)SignalIcon.o \
      $(INFO_OBJ)ConnectionIcon.o
endif


INFO_TEST_PREFIX := $(INFO_PREFIX)Test_
INFO_TEST_OBJ := $(INFO_OBJ)Test_
OBJECTS_INFO_TEST := \
  $(INFO_TEST_OBJ)IPLabelTest.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_INFO := $(OBJECTS_INFO) $(OBJECTS_INFO_TEST)
endif

GUI_MODULES := $(GUI_MODULES) info

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_INFO)

info : $(OBJECTS_INFO)
	@echo "    Built Info module"

$(INFO_OBJ)BatteryIcon.o : \
    $(INFO_DIR)/$(INFO_PREFIX)BatteryIcon.cpp

$(INFO_OBJ)Clock.o : \
    $(INFO_DIR)/$(INFO_PREFIX)Clock.cpp

$(INFO_OBJ)IPLabel.o : \
    $(INFO_DIR)/$(INFO_PREFIX)IPLabel.cpp

$(INFO_OBJ)SignalIcon.o : \
    $(INFO_DIR)/$(INFO_PREFIX)SignalIcon.cpp

$(INFO_OBJ)ConnectionIcon.o : \
    $(INFO_DIR)/$(INFO_PREFIX)ConnectionIcon.cpp

$(INFO_TEST_OBJ)IPLabelTest.o : \
    $(INFO_TEST_DIR)/$(INFO_TEST_PREFIX)IPLabelTest.cpp
