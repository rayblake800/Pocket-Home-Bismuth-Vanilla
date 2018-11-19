########################## Settings Module #####################################
SETTINGS_PREFIX := $(JUCE_OBJDIR)/Settings_
SETTINGS_ROOT = Source/Settings

OBJECTS_SETTINGS := \
  $(SETTINGS_PREFIX)QuickPage.o \
  $(SETTINGS_PREFIX)DateTimePage.o \
  $(SETTINGS_PREFIX)HomePage.o \
  $(SETTINGS_PREFIX)InputPage.o \
  $(SETTINGS_PREFIX)ConnectionComponent.o \
  $(SETTINGS_PREFIX)ListPage.o 

OBJECTS_SETTINGS_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_SETTINGS := $(OBJECTS_SETTINGS) $(OBJECTS_SETTINGS_TEST)
endif

MODULES := $(MODULES) settings

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_SETTINGS)

settings : $(OBJECTS_SETTINGS)
	@echo "Built Settings module"

$(SETTINGS_PREFIX)QuickPage.o : \
    $(SETTINGS_ROOT)/QuickSettingsPage/QuickSettingsPage.cpp
$(SETTINGS_PREFIX)DateTimePage.o : \
    $(SETTINGS_ROOT)/DateTimePage/DateTimePage.cpp
$(SETTINGS_PREFIX)HomePage.o : \
    $(SETTINGS_ROOT)/HomeSettingsPage/HomeSettingsPage.cpp
$(SETTINGS_PREFIX)InputPage.o : \
    $(SETTINGS_ROOT)/InputSettingsPage/InputSettingsPage.cpp
$(SETTINGS_PREFIX)ConnectionComponent.o : \
    $(SETTINGS_ROOT)/QuickSettingsPage/ConnectionSettingsComponent.cpp
$(SETTINGS_PREFIX)ListPage.o : \
    $(SETTINGS_ROOT)/SettingsListPage/SettingsListPage.cpp
