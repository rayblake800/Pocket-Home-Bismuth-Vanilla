############################ Theme Module ######################################
THEME_PREFIX := $(JUCE_OBJDIR)/Theme_
THEME_COLOUR_PREFIX := $(THEME_PREFIX)Colour_
THEME_IMAGE_PREFIX := $(THEME_PREFIX)Image_

THEME_ROOT = Source/Theme
THEME_COLOUR_ROOT := $(THEME_ROOT)/Colour
THEME_IMAGE_ROOT := $(THEME_ROOT)/Image

OBJECTS_THEME_COLOUR := \
  $(THEME_COLOUR_PREFIX)JSONKeys.o \
  $(THEME_COLOUR_PREFIX)JSONResource.o \
  $(THEME_COLOUR_PREFIX)ConfigFile.o \
  $(THEME_COLOUR_PREFIX)ConfigListener.o \
  $(THEME_COLOUR_PREFIX)ConfigPage.o

OBJECTS_THEME_IMAGE := \
  $(THEME_IMAGE_PREFIX)AssetList.o \
  $(THEME_IMAGE_PREFIX)JSONResource.o \
  $(THEME_IMAGE_PREFIX)ConfigFile.o

OBJECTS_THEME := \
  $(OBJECTS_THEME_COLOUR) \
  $(OBJECTS_THEME_IMAGE) \
  $(THEME_PREFIX)LookAndFeel.o  

OBJECTS_THEME_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_THEME := $(OBJECTS_THEME) $(OBJECTS_THEME_TEST)
endif

MODULES := $(MODULES) theme

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_THEME)

theme : $(OBJECTS_THEME)
	@echo "Built Theme module"

$(THEME_COLOUR_PREFIX)JSONKeys.o : \
    $(THEME_COLOUR_ROOT)/Theme_Colour_JSONKeys.cpp
$(THEME_COLOUR_PREFIX)JSONResource.o : \
    $(THEME_COLOUR_ROOT)/Theme_Colour_JSONResource.cpp
$(THEME_COLOUR_PREFIX)ConfigFile.o : \
    $(THEME_COLOUR_ROOT)/Theme_Colour_ConfigFile.cpp
$(THEME_COLOUR_PREFIX)ConfigListener.o : \
    $(THEME_COLOUR_ROOT)/Theme_Colour_ConfigListener.cpp
$(THEME_COLOUR_PREFIX)ConfigPage.o : \
    $(THEME_COLOUR_ROOT)/Theme_Colour_ConfigPage.cpp

$(THEME_IMAGE_PREFIX)AssetList.o : \
    $(THEME_IMAGE_ROOT)/Theme_Image_AssetList.cpp
$(THEME_IMAGE_PREFIX)JSONResource.o : \
    $(THEME_IMAGE_ROOT)/Theme_Image_JSONResource.cpp
$(THEME_IMAGE_PREFIX)ConfigFile.o : \
    $(THEME_IMAGE_ROOT)/Theme_Image_ConfigFile.cpp
$(THEME_IMAGE_PREFIX)ConfigPage.o : \
    $(THEME_IMAGE_ROOT)/Theme_Image_ConfigPage.cpp

$(THEME_PREFIX)LookAndFeel.o : \
    $(THEME_ROOT)/Theme_LookAndFeel.cpp
