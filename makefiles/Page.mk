############################## Page Module #####################################
PAGE_PREFIX := $(JUCE_OBJDIR)/Page_
PAGE_ROOT = Source/Page

OBJECTS_PAGE := \
  $(PAGE_PREFIX)Component.o \
  $(PAGE_PREFIX)FocusingList.o \
  $(PAGE_PREFIX)Factory.o \
  $(PAGE_PREFIX)Stack.o 

OBJECTS_PAGE_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_PAGE := $(OBJECTS_PAGE) $(OBJECTS_PAGE_TEST)
endif

MODULES := $(MODULES) page

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_PAGE)

page : $(OBJECTS_PAGE)
	@echo "Built Page module"

$(PAGE_PREFIX)Component.o : \
    $(PAGE_ROOT)/PageComponent.cpp
$(PAGE_PREFIX)FocusingList.o : \
    $(PAGE_ROOT)/FocusingListPage.cpp
$(PAGE_PREFIX)Factory.o : \
    $(PAGE_ROOT)/PageFactory.cpp
$(PAGE_PREFIX)Stack.o : \
    $(PAGE_ROOT)/PageStackComponent.cpp
