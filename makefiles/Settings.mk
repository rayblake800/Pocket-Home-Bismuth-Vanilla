########################## Settings Module #####################################
SETTINGS_DIR = Source/GUI/Settings
SETTINGS_WIFI_LIST_DIR := $(SETTINGS_DIR)/WifiList

SETTINGS_PREFIX = Settings_
SETTINGS_OBJ := $(JUCE_OBJDIR)/$(SETTINGS_PREFIX)

SETTINGS_WIFI_LIST_PREFIX := $(SETTINGS_PREFIX)WifiList_
SETTINGS_WIFI_LIST_OBJ := $(SETTINGS_OBJ)WifiList_
OBJECTS_WIFI_LIST_SETTINGS := \
  $(SETTINGS_WIFI_LIST_OBJ)ListButton.o \
  $(SETTINGS_WIFI_LIST_OBJ)ControlComponent.o \
  $(SETTINGS_WIFI_LIST_OBJ)ListComponent.o

OBJECTS_SETTINGS := \
  $(SETTINGS_OBJ)ClockModePicker.o \
  $(SETTINGS_OBJ)BrightnessSlider.o \
  $(SETTINGS_OBJ)VolumeSlider.o \
  $(SETTINGS_OBJ)CursorModePicker.o \
  $(SETTINGS_OBJ)HomeBackgroundPicker.o \
  $(SETTINGS_OBJ)CalibrationButton.o \
  $(SETTINGS_OBJ)ConnectionComponent.o \
  $(SETTINGS_OBJ)WifiControl.o \
  $(OBJECTS_WIFI_LIST_SETTINGS)

OBJECTS_SETTINGS_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SETTINGS := $(OBJECTS_SETTINGS) $(OBJECTS_SETTINGS_TEST)
endif

GUI_MODULES := $(GUI_MODULES) settings

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SETTINGS)

settings : $(OBJECTS_SETTINGS)
	@echo "    Built Settings module"

$(SETTINGS_OBJ)ClockModePicker.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)ClockModePicker.cpp
$(SETTINGS_OBJ)BrightnessSlider.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)BrightnessSlider.cpp
$(SETTINGS_OBJ)VolumeSlider.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)VolumeSlider.cpp
$(SETTINGS_OBJ)CursorModePicker.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)CursorModePicker.cpp
$(SETTINGS_OBJ)HomeBackgroundPicker.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)HomeBackgroundPicker.cpp
$(SETTINGS_OBJ)CalibrationButton.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)CalibrationButton.cpp
$(SETTINGS_OBJ)ConnectionComponent.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)ConnectionComponent.cpp
$(SETTINGS_OBJ)WifiControl.o : \
    $(SETTINGS_DIR)/$(SETTINGS_PREFIX)WifiControl.cpp

$(SETTINGS_WIFI_LIST_OBJ)ListButton.o : \
    $(SETTINGS_WIFI_LIST_DIR)/$(SETTINGS_WIFI_LIST_PREFIX)ListButton.cpp
$(SETTINGS_WIFI_LIST_OBJ)ControlComponent.o : \
    $(SETTINGS_WIFI_LIST_DIR)/$(SETTINGS_WIFI_LIST_PREFIX)ControlComponent.cpp
$(SETTINGS_WIFI_LIST_OBJ)ListComponent.o : \
    $(SETTINGS_WIFI_LIST_DIR)/$(SETTINGS_WIFI_LIST_PREFIX)ListComponent.cpp
