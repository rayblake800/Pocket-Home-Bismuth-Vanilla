########################### Layout Module ######################################
LAYOUT_PREFIX := $(JUCE_OBJDIR)/Layout_
LAYOUT_COMPONENT_PREFIX := $(LAYOUT_PREFIX)Component_
LAYOUT_GROUP_PREFIX := $(LAYOUT_PREFIX)Group_
LAYOUT_ROOT = Source/Layout

OBJECTS_LAYOUT_COMPONENT := \
  $(LAYOUT_COMPONENT_PREFIX)ConfigLayout.o \
  $(LAYOUT_COMPONENT_PREFIX)JSONResource.o \
  $(LAYOUT_COMPONENT_PREFIX)ConfigFile.o \
  $(LAYOUT_COMPONENT_PREFIX)Manager.o

OBJECTS_LAYOUT_GROUP := \
  $(LAYOUT_GROUP_PREFIX)RelativeLayout.o \
  $(LAYOUT_GROUP_PREFIX)Manager.o

OBJECTS_LAYOUT := \
  $(OBJECTS_LAYOUT_COMPONENT) \
  $(OBJECTS_LAYOUT_GROUP) \
  $(LAYOUT_PREFIX)Transition_Animator.o 

OBJECTS_LAYOUT_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_LAYOUT := $(OBJECTS_LAYOUT) $(OBJECTS_LAYOUT_TEST)
endif

MODULES := $(MODULES) layout

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_LAYOUT)

layout : $(OBJECTS_LAYOUT)
	@echo "Built Layout module"

$(LAYOUT_COMPONENT_PREFIX)ConfigLayout.o : \
    $(LAYOUT_ROOT)/Component/Layout_Component_ConfigLayout.cpp
$(LAYOUT_COMPONENT_PREFIX)JSONResource.o : \
    $(LAYOUT_ROOT)/Component/Layout_Component_JSONResource.cpp
$(LAYOUT_COMPONENT_PREFIX)ConfigFile.o : \
    $(LAYOUT_ROOT)/Component/Layout_Component_ConfigFile.cpp
$(LAYOUT_COMPONENT_PREFIX)Manager.o : \
    $(LAYOUT_ROOT)/Component/Layout_Component_Manager.cpp

$(LAYOUT_GROUP_PREFIX)RelativeLayout.o : \
    $(LAYOUT_ROOT)/Group/Layout_Group_RelativeLayout.cpp
$(LAYOUT_GROUP_PREFIX)Manager.o : \
    $(LAYOUT_ROOT)/Group/Layout_Group_Manager.cpp

$(LAYOUT_PREFIX)Transition_Animator.o : \
    $(LAYOUT_ROOT)/Transition/Layout_Transition_Animator.cpp
