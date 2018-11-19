############################## Config Module ###################################
CONFIG_PREFIX := $(JUCE_OBJDIR)/Config_
CONFIG_ROOT = Source/Config
CONFIG_TEST_ROOT = Tests/Config

OBJECTS_CONFIG := \
  $(CONFIG_PREFIX)MainResource.o \
  $(CONFIG_PREFIX)FileResource.o \
  $(CONFIG_PREFIX)DataKey.o \
  $(CONFIG_PREFIX)AlertWindow.o 

OBJECTS_CONFIG_TEST := \
  $(CONFIG_PREFIX)Test_FileResource.o \
  $(CONFIG_PREFIX)Test_FileHandler.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_CONFIG := $(OBJECTS_CONFIG) $(OBJECTS_CONFIG_TEST)
endif

MODULES := $(MODULES) config

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_CONFIG)

config : $(OBJECTS_CONFIG)
	@echo "Built Config module"

$(CONFIG_PREFIX)MainResource.o: \
    $(CONFIG_ROOT)/MainResource.cpp
$(CONFIG_PREFIX)FileResource.o: \
    $(CONFIG_ROOT)/FileResource.cpp
$(CONFIG_PREFIX)DataKey.o: \
    $(CONFIG_ROOT)/DataKey.cpp
$(CONFIG_PREFIX)AlertWindow.o: \
    $(CONFIG_ROOT)/AlertWindow.cpp

# Tests:
$(CONFIG_PREFIX)Test_FileResource.o: \
    $(CONFIG_TEST_ROOT)/FileResource.cpp
$(CONFIG_PREFIX)Test_FileHandler.o: \
    $(CONFIG_TEST_ROOT)/FileHandler.cpp
