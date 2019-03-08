############################## Page Module #####################################
PAGE_ROOT = Source/Page
PAGE_PREFIX := Page_
PAGE_OBJ := $(JUCE_OBJDIR)/$(PAGE_PREFIX)

PAGE_TYPES_ROOT := $(PAGE_ROOT)/PageTypes
OBJECTS_PAGE_TYPES := \
  $(PAGE_OBJ)HomeSettings.o \
  $(PAGE_OBJ)InputSettings.o \
  $(PAGE_OBJ)PasswordEditor.o \
  $(PAGE_OBJ)PasswordRemover.o \
  $(PAGE_OBJ)QuickSettings.o \
  $(PAGE_OBJ)SettingsList.o \
  $(PAGE_OBJ)WifiConnection.o

PAGE_INTERFACE_ROOT := $(PAGE_ROOT)/Interface
PAGE_INTERFACE_PREFIX := $(PAGE_PREFIX)Interface_
PAGE_INTERFACE_OBJ := $(PAGE_OBJ)Interface_

OBJECTS_PAGE := \
  $(PAGE_INTERFACE_OBJ)Component.o \
  $(PAGE_INTERFACE_OBJ)FactoryHolder.o \
  $(PAGE_OBJ)Component.o \
  $(PAGE_OBJ)Factory.o \
  $(PAGE_OBJ)StackComponent.o \
  $(OBJECTS_PAGE_TYPES)

OBJECTS_PAGE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PAGE := $(OBJECTS_PAGE) $(OBJECTS_PAGE_TEST)
endif

MODULES := $(MODULES) page

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PAGE)

page : $(OBJECTS_PAGE)
	@echo "Built Page module"

$(PAGE_OBJ)HomeSettings.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)HomeSettings.cpp
$(PAGE_OBJ)InputSettings.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)InputSettings.cpp
$(PAGE_OBJ)PasswordEditor.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)PasswordEditor.cpp
$(PAGE_OBJ)PasswordRemover.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)PasswordRemover.cpp
$(PAGE_OBJ)QuickSettings.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)QuickSettings.cpp
$(PAGE_OBJ)SettingsList.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)SettingsList.cpp
$(PAGE_OBJ)WifiConnection.o : \
    $(PAGE_TYPES_ROOT)/$(PAGE_PREFIX)WifiConnection.cpp

$(PAGE_INTERFACE_OBJ)Component.o : \
    $(PAGE_INTERFACE_ROOT)/$(PAGE_INTERFACE_PREFIX)Component.cpp
$(PAGE_INTERFACE_OBJ)FactoryHolder.o : \
    $(PAGE_INTERFACE_ROOT)/$(PAGE_INTERFACE_PREFIX)FactoryHolder.cpp

$(PAGE_OBJ)Component.o : \
    $(PAGE_ROOT)/$(PAGE_PREFIX)Component.cpp
$(PAGE_OBJ)Factory.o : \
    $(PAGE_ROOT)/$(PAGE_PREFIX)Factory.cpp
$(PAGE_OBJ)StackComponent.o : \
    $(PAGE_ROOT)/$(PAGE_PREFIX)StackComponent.cpp

