############################## Icon Module #####################################
ICON_PREFIX := $(JUCE_OBJDIR)/Icon_
ICON_ROOT = Source/Icon

OBJECTS_ICON := \
  $(ICON_PREFIX)Cache.o \
  $(ICON_PREFIX)Loader.o \
  $(ICON_PREFIX)ThemeIndex.o \
  $(ICON_PREFIX)ThreadResource.o 

OBJECTS_ICON_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_ICON := $(OBJECTS_ICON) $(OBJECTS_ICON_TEST)
endif

MODULES := $(MODULES) icon

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_ICON)

icon : $(OBJECTS_ICON)
	@echo "Built Icon module"

$(ICON_PREFIX)Cache.o: \
	$(ICON_ROOT)/Icon_Cache.cpp
$(ICON_PREFIX)Loader.o: \
	$(ICON_ROOT)/Icon_Loader.cpp
$(ICON_PREFIX)ThemeIndex.o: \
	$(ICON_ROOT)/Icon_ThemeIndex.cpp
$(ICON_PREFIX)ThreadResource.o: \
	$(ICON_ROOT)/Icon_ThreadResource.cpp
