############################ Theme Module ######################################
THEME_PREFIX := $(JUCE_OBJDIR)/Theme_
THEME_ROOT = Source/Theme

OBJECTS_THEME := \
  $(THEME_PREFIX)ColourJSON.o \
  $(THEME_PREFIX)ColourConfigFile.o \
  $(THEME_PREFIX)ColourConfigKeys.o \
  $(THEME_PREFIX)ColourPage.o \
  $(THEME_PREFIX)LookAndFeel.o  

OBJECTS_THEME_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_THEME := $(OBJECTS_THEME) $(OBJECTS_THEME_TEST)
endif

MODULES := $(MODULES) theme

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_THEME)

theme : $(OBJECTS_THEME)
	@echo "Built Theme module"

$(THEME_PREFIX)ColourJSON.o : \
    $(THEME_ROOT)/ColourJSON.cpp
$(THEME_PREFIX)ColourConfigFile.o : \
    $(THEME_ROOT)/ColourConfigFile.cpp
$(THEME_PREFIX)ColourConfigKeys.o : \
    $(THEME_ROOT)/ColourConfigKeys.cpp
$(THEME_PREFIX)ColourPage.o : \
    $(THEME_ROOT)/ColourPage.cpp
$(THEME_PREFIX)LookAndFeel.o : \
    $(THEME_ROOT)/PokeLookAndFeel.cpp
