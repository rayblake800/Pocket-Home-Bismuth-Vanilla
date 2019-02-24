############################## Widget Module ###################################
WIDGET_PREFIX := $(JUCE_OBJDIR)/Widget_
WIDGET_ROOT = Source/Widgets

OBJECTS_WIDGET := \
  $(WIDGET_PREFIX)ScalingLabel.o \
  $(WIDGET_PREFIX)ScalingTextButton.o \
  $(WIDGET_PREFIX)DrawableImage.o \
  $(WIDGET_PREFIX)DrawableImageButton.o \
  $(WIDGET_PREFIX)PagedList.o \
  $(WIDGET_PREFIX)FocusingPagedList.o \
  $(WIDGET_PREFIX)NavButton.o \
  $(WIDGET_PREFIX)IconSlider.o \
  $(WIDGET_PREFIX)ListEditor.o \
  $(WIDGET_PREFIX)Spinner.o \
  $(WIDGET_PREFIX)OverlaySpinner.o \
  $(WIDGET_PREFIX)Switch.o \
  $(WIDGET_PREFIX)ColourPicker.o \
  $(WIDGET_PREFIX)ClockLabel.o \
  $(WIDGET_PREFIX)PopupEditor.o \
  $(WIDGET_PREFIX)FileSelectTextEditor.o \
  $(WIDGET_PREFIX)Counter.o 

OBJECTS_WIDGET_TEST :=

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_WIDGET := $(OBJECTS_WIDGET) $(OBJECTS_WIDGET_TEST)
endif

MODULES := $(MODULES) widget

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_WIDGET)

widget : $(OBJECTS_WIDGET)
	@echo "Built Widget module"

$(WIDGET_PREFIX)ScalingLabel.o : \
    $(WIDGET_ROOT)/ScalingLabel.cpp
$(WIDGET_PREFIX)ScalingTextButton.o : \
    $(WIDGET_ROOT)/ScalingTextButton.cpp
$(WIDGET_PREFIX)DrawableImage.o : \
    $(WIDGET_ROOT)/DrawableImageComponent.cpp
$(WIDGET_PREFIX)DrawableImageButton.o : \
    $(WIDGET_ROOT)/DrawableImageButton.cpp
$(WIDGET_PREFIX)PagedList.o : \
    $(WIDGET_ROOT)/PagedList.cpp
$(WIDGET_PREFIX)FocusingPagedList.o : \
    $(WIDGET_ROOT)/FocusingPagedList.cpp
$(WIDGET_PREFIX)NavButton.o : \
    $(WIDGET_ROOT)/NavButton.cpp
$(WIDGET_PREFIX)IconSlider.o : \
    $(WIDGET_ROOT)/IconSliderComponent.cpp
$(WIDGET_PREFIX)ListEditor.o : \
    $(WIDGET_ROOT)/ListEditor.cpp
$(WIDGET_PREFIX)Spinner.o : \
    $(WIDGET_ROOT)/Spinner.cpp
$(WIDGET_PREFIX)OverlaySpinner.o : \
    $(WIDGET_ROOT)/OverlaySpinner.cpp
$(WIDGET_PREFIX)Switch.o : \
    $(WIDGET_ROOT)/SwitchComponent.cpp
$(WIDGET_PREFIX)ColourPicker.o : \
    $(WIDGET_ROOT)/ColourPicker.cpp
$(WIDGET_PREFIX)ClockLabel.o : \
    $(WIDGET_ROOT)/ClockLabel.cpp
$(WIDGET_PREFIX)PopupEditor.o : \
    $(WIDGET_ROOT)/PopupEditorComponent.cpp
$(WIDGET_PREFIX)FileSelectTextEditor.o : \
    $(WIDGET_ROOT)/FileSelectTextEditor.cpp
$(WIDGET_PREFIX)Counter.o : \
    $(WIDGET_ROOT)/CounterComponent.cpp
