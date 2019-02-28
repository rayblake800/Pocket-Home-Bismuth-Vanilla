############################## Widget Module ###################################
WIDGET_ROOT   = Source/Widgets
WIDGET_PREFIX = Widgets_
WIDGET_OBJ   := $(JUCE_OBJDIR)/$(WIDGET_PREFIX)

OBJECTS_WIDGET := \
  $(WIDGET_OBJ)BoundedLabel.o \
  $(WIDGET_OBJ)DrawableImage.o \
  $(WIDGET_OBJ)DrawableImageButton.o \
  $(WIDGET_OBJ)PagedList.o \
  $(WIDGET_OBJ)FocusingPagedList.o \
  $(WIDGET_OBJ)NavButton.o \
  $(WIDGET_OBJ)IconSlider.o \
  $(WIDGET_OBJ)ListEditor.o \
  $(WIDGET_OBJ)Spinner.o \
  $(WIDGET_OBJ)OverlaySpinner.o \
  $(WIDGET_OBJ)Switch.o \
  $(WIDGET_OBJ)ColourPicker.o \
  $(WIDGET_OBJ)Clock.o \
  $(WIDGET_OBJ)PopupEditor.o \
  $(WIDGET_OBJ)FilePathEditor.o \
  $(WIDGET_OBJ)Counter.o 

OBJECTS_WIDGET_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIDGET := $(OBJECTS_WIDGET) $(OBJECTS_WIDGET_TEST)
endif

MODULES := $(MODULES) widget

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIDGET)

widget : $(OBJECTS_WIDGET)
	@echo "Built Widget module"

$(WIDGET_OBJ)BoundedLabel.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)BoundedLabel.cpp
$(WIDGET_OBJ)DrawableImage.o : \
    $(WIDGET_ROOT)/DrawableImageComponent.cpp
$(WIDGET_OBJ)DrawableImageButton.o : \
    $(WIDGET_ROOT)/DrawableImageButton.cpp
$(WIDGET_OBJ)PagedList.o : \
    $(WIDGET_ROOT)/PagedList.cpp
$(WIDGET_OBJ)FocusingPagedList.o : \
    $(WIDGET_ROOT)/FocusingPagedList.cpp
$(WIDGET_OBJ)NavButton.o : \
    $(WIDGET_ROOT)/NavButton.cpp
$(WIDGET_OBJ)IconSlider.o : \
    $(WIDGET_ROOT)/IconSliderComponent.cpp
$(WIDGET_OBJ)ListEditor.o : \
    $(WIDGET_ROOT)/ListEditor.cpp
$(WIDGET_OBJ)Spinner.o : \
    $(WIDGET_ROOT)/Spinner.cpp
$(WIDGET_OBJ)OverlaySpinner.o : \
    $(WIDGET_ROOT)/OverlaySpinner.cpp
$(WIDGET_OBJ)Switch.o : \
    $(WIDGET_ROOT)/SwitchComponent.cpp
$(WIDGET_OBJ)ColourPicker.o : \
    $(WIDGET_ROOT)/ColourPicker.cpp
$(WIDGET_OBJ)Clock.o : \
    $(WIDGET_ROOT)/$(WIDGET_PREFIX)Clock.cpp
$(WIDGET_OBJ)PopupEditor.o : \
    $(WIDGET_ROOT)/PopupEditorComponent.cpp
$(WIDGET_OBJ)FilePathEditor.o : \
    $(WIDGET_ROOT)/FileSelectTextEditor.cpp
$(WIDGET_OBJ)Counter.o : \
    $(WIDGET_ROOT)/CounterComponent.cpp
