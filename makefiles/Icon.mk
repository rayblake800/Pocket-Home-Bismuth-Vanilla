############################## Icon Module #####################################
ICON_PREFIX := $(JUCE_OBJDIR)/Icon_
ICON_ROOT = Source/Icon

OBJECTS_ICON := \
  $(ICON_PREFIX)IconCache.o \
  $(ICON_PREFIX)IconLoader.o \
  $(ICON_PREFIX)IconThemeIndex.o \
  $(ICON_PREFIX)IconThread.o 

OBJECTS_ICON_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_ICON := $(OBJECTS_ICON) $(OBJECTS_ICON_TEST)
endif

MODULES := $(MODULES) icon

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_ICON)

icon : $(OBJECTS_ICON)
	@echo "Built Icon module"

$(ICON_PREFIX)IconCache.o: \
	$(ICON_ROOT)/IconCache.cpp
$(ICON_PREFIX)IconLoader.o: \
	$(ICON_ROOT)/IconLoader.cpp
$(ICON_PREFIX)IconThemeIndex.o: \
	$(ICON_ROOT)/IconThemeIndex.cpp
$(ICON_PREFIX)IconThread.o: \
	$(ICON_ROOT)/IconThread.cpp
