######################## Configurable Module ###################################
CONFIGURABLE_PREFIX := $(JUCE_OBJDIR)/Configurable_
CONFIGURABLE_ROOT = Source/Configurable

OBJECTS_CONFIGURABLE := \
  $(CONFIGURABLE_PREFIX)JSONResource.o \
  $(CONFIGURABLE_PREFIX)ConfigFile.o \
  $(CONFIGURABLE_PREFIX)Component.o \
  $(CONFIGURABLE_PREFIX)ComponentSettings.o \
  $(CONFIGURABLE_PREFIX)ImageButton.o \
  $(CONFIGURABLE_PREFIX)ImageComponent.o \
  $(CONFIGURABLE_PREFIX)Label.o 

OBJECTS_CONFIGURABLE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_CONFIGURABLE := $(OBJECTS_CONFIGURABLE) $(OBJECTS_CONFIGURABLE_TEST)
endif

MODULES := $(MODULES) configurable

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_CONFIGURABLE)

configurable : $(OBJECTS_CONFIGURABLE)
	@echo "Built Configurable module"

$(CONFIGURABLE_PREFIX)JSONResource.o : \
    $(CONFIGURABLE_ROOT)/ComponentJSON.cpp
$(CONFIGURABLE_PREFIX)ConfigFile.o : \
    $(CONFIGURABLE_ROOT)/ComponentConfigFile.cpp
$(CONFIGURABLE_PREFIX)ComponentSettings.o : \
    $(CONFIGURABLE_ROOT)/ComponentSettings.cpp
$(CONFIGURABLE_PREFIX)Component.o : \
    $(CONFIGURABLE_ROOT)/ConfigurableComponent.cpp
$(CONFIGURABLE_PREFIX)ImageButton.o : \
    $(CONFIGURABLE_ROOT)/ConfigurableImageButton.cpp
$(CONFIGURABLE_PREFIX)ImageComponent.o : \
    $(CONFIGURABLE_ROOT)/ConfigurableImageComponent.cpp
$(CONFIGURABLE_PREFIX)Label.o : \
    $(CONFIGURABLE_ROOT)/ConfigurableLabel.cpp
