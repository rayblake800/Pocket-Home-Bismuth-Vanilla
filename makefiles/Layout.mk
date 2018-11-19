########################### Layout Module ######################################
LAYOUT_PREFIX := $(JUCE_OBJDIR)/Layout_
LAYOUT_ROOT = Source/Layout

OBJECTS_LAYOUT := \
  $(LAYOUT_PREFIX)Manager.o \
  $(LAYOUT_PREFIX)TransitionAnimator.o 

OBJECTS_LAYOUT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LAYOUT := $(OBJECTS_LAYOUT) $(OBJECTS_LAYOUT_TEST)
endif

MODULES := $(MODULES) layout

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_LAYOUT)

layout : $(OBJECTS_LAYOUT)
	@echo "Built Layout module"

$(LAYOUT_PREFIX)Manager.o : \
    $(LAYOUT_ROOT)/LayoutManager.cpp
$(LAYOUT_PREFIX)TransitionAnimator.o : \
    $(LAYOUT_ROOT)/TransitionAnimator.cpp
