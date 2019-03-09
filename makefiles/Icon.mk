############################## Icon Module #####################################
ICON_DIR = Source/Files/Icon
ICON_TEST_DIR = Tests/Files/Icon

ICON_PREFIX := Icon_
ICON_OBJ := $(JUCE_OBJDIR)/$(ICON_PREFIX)

OBJECTS_ICON := \
  $(ICON_OBJ)Cache.o \
  $(ICON_OBJ)Loader.o \
  $(ICON_OBJ)ThemeIndex.o \
  $(ICON_OBJ)ThreadResource.o 

OBJECTS_ICON_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_ICON := $(OBJECTS_ICON) $(OBJECTS_ICON_TEST)
endif

MODULES := $(MODULES) icon

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_ICON)

icon : $(OBJECTS_ICON)
	@echo "Built Icon module"

$(ICON_OBJ)Cache.o: \
	$(ICON_DIR)/$(ICON_PREFIX)Cache.cpp
$(ICON_OBJ)Loader.o: \
	$(ICON_DIR)/$(ICON_PREFIX)Loader.cpp
$(ICON_OBJ)ThemeIndex.o: \
	$(ICON_DIR)/$(ICON_PREFIX)ThemeIndex.cpp
$(ICON_OBJ)ThreadResource.o: \
	$(ICON_DIR)/$(ICON_PREFIX)ThreadResource.cpp
