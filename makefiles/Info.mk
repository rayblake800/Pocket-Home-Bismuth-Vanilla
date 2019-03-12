#############################  Info Module  ####################################
INFO_DIR = Source/GUI/Info
INFO_TEST_DIR = Tests/GUI/Info

INFO_PREFIX = Info_
INFO_OBJ := $(JUCE_OBJDIR)/$(INFO_PREFIX)

OBJECTS_INFO := \
  $(INFO_OBJ)BatteryIcon.o \
  $(INFO_OBJ)Clock.o \

INFO_TEST_PREFIX := $(INFO_PREFIX)Test_
INFO_TEST_OBJ := $(INFO_OBJ)Test_
OBJECTS_INFO_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_INFO := $(OBJECTS_INFO) $(OBJECTS_INFO_TEST)
endif

MODULES := $(MODULES) info

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_INFO)

info : $(OBJECTS_INFO)
	@echo "Built Info module"

$(INFO_OBJ)BatteryIcon.o : \
    $(INFO_DIR)/BatteryIcon.cpp

$(INFO_OBJ)Clock.o : \
    $(INFO_DIR)/$(INFO_PREFIX)Clock.cpp
