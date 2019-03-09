############################ AppMenu Module ####################################
APPMENU_DIR = Source/GUI/AppMenu
APPMENU_COMPONENT_DIR := $(APPMENU_DIR)/Components
APPMENU_EDITOR_DIR := $(APPMENU_COMPONENT_DIR)/Editors
APPMENU_CONTROL_DIR := $(APPMENU_DIR)/Control
APPMENU_DATA_DIR := $(APPMENU_DIR)/Data
APPMENU_TEST_DIR = Tests/GUI/AppMenu

APPMENU_PREFIX = AppMenu_
APPMENU_OBJ := $(JUCE_OBJDIR)/$(APPMENU_PREFIX)

OBJECTS_APPMENU_DATA := \
  $(APPMENU_OBJ)MenuJSON.o \
  $(APPMENU_OBJ)ConfigJSON.o \
  $(APPMENU_OBJ)MenuFile.o \
  $(APPMENU_OBJ)MenuItem.o \
  $(APPMENU_OBJ)EntryData.o \
  $(APPMENU_OBJ)EntryActions.o \
  $(APPMENU_OBJ)EntryLoader.o \
  $(APPMENU_OBJ)EntryUpdater.o \
  $(APPMENU_OBJ)ConfigData.o \
  $(APPMENU_OBJ)ItemData.o

OBJECTS_APPMENU_COMPONENT := \
  $(APPMENU_OBJ)MenuButton.o \
  $(APPMENU_OBJ)FolderComponent.o \
  $(APPMENU_OBJ)MenuComponent.o \
  $(APPMENU_OBJ)MainComponent.o 

OBJECTS_APPMENU_CONTROL := \
  $(APPMENU_OBJ)Controller.o \
  $(APPMENU_OBJ)InputHandler.o 

OBJECTS_APPMENU_EDITOR := \
  $(APPMENU_OBJ)NewConfigItemEditor.o \
  $(APPMENU_OBJ)NewDesktopAppEditor.o \
  $(APPMENU_OBJ)ExistingItemEditor.o \
  $(APPMENU_OBJ)CategoryEditor.o \
  $(APPMENU_OBJ)PopupEditor.o

APPMENU_SCROLLING_DIR := $(APPMENU_DIR)/Formats/Scrolling
APPMENU_SCROLLING_PREFIX := $(APPMENU_PREFIX)Scrolling_
APPMENU_SCROLLING_OBJ := $(APPMENU_OBJ)Scrolling_
OBJECTS_APPMENU_SCROLLING := \
  $(APPMENU_SCROLLING_OBJ)Initializer.o \
  $(APPMENU_SCROLLING_OBJ)MenuButton.o \
  $(APPMENU_SCROLLING_OBJ)FolderComponent.o \
  $(APPMENU_SCROLLING_OBJ)MenuComponent.o \
  $(APPMENU_SCROLLING_OBJ)InputHandler.o 

APPMENU_PAGED_DIR := $(APPMENU_DIR)/Formats/Paged
APPMENU_PAGED_PREFIX := $(APPMENU_PREFIX)Paged_
APPMENU_PAGED_OBJ := $(APPMENU_OBJ)Paged_
OBJECTS_APPMENU_PAGED := \
  $(APPMENU_PAGED_OBJ)MenuButton.o \
  $(APPMENU_PAGED_OBJ)FolderComponent.o \
  $(APPMENU_PAGED_OBJ)MenuComponent.o \
  $(APPMENU_PAGED_OBJ)InputHandler.o \
  $(APPMENU_PAGED_OBJ)Initializer.o 

OBJECTS_APPMENU := \
  $(APPMENU_OBJ)AppMenu.o \
  $(APPMENU_OBJ)ConfigFile.o \
  $(APPMENU_OBJ)SettingsController.o \
  $(OBJECTS_APPMENU_DATA) \
  $(OBJECTS_APPMENU_COMPONENT) \
  $(OBJECTS_APPMENU_CONTROL) \
  $(OBJECTS_APPMENU_EDITOR) \
  $(OBJECTS_APPMENU_SCROLLING) \
  $(OBJECTS_APPMENU_PAGED)  

APPMENU_TEST_PREFIX := $(APPMENU_PREFIX)Test_
APPMENU_TEST_OBJ := $(APPMENU_OBJ)Test_
OBJECTS_APPMENU_TEST := \
  $(APPMENU_TEST_OBJ)AppMenu.o \
  $(APPMENU_TEST_OBJ)ConfigFile.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_APPMENU := $(OBJECTS_APPMENU) $(OBJECTS_APPMENU_TEST)
endif

MODULES := $(MODULES) appMenu

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_APPMENU)

appMenu : $(OBJECTS_APPMENU)
	@echo "Built AppMenu module"

$(APPMENU_SCROLLING_OBJ)MenuButton.o: \
	$(APPMENU_SCROLLING_DIR)/$(APPMENU_SCROLLING_PREFIX)MenuButton.cpp
$(APPMENU_SCROLLING_OBJ)FolderComponent.o: \
	$(APPMENU_SCROLLING_DIR)/$(APPMENU_SCROLLING_PREFIX)FolderComponent.cpp
$(APPMENU_SCROLLING_OBJ)MenuComponent.o: \
	$(APPMENU_SCROLLING_DIR)/$(APPMENU_SCROLLING_PREFIX)MenuComponent.cpp
$(APPMENU_SCROLLING_OBJ)InputHandler.o: \
	$(APPMENU_SCROLLING_DIR)/$(APPMENU_SCROLLING_PREFIX)InputHandler.cpp
$(APPMENU_SCROLLING_OBJ)Initializer.o: \
	$(APPMENU_SCROLLING_DIR)/$(APPMENU_SCROLLING_PREFIX)Initializer.cpp

$(APPMENU_PAGED_OBJ)MenuButton.o: \
	$(APPMENU_PAGED_DIR)/$(APPMENU_PAGED_PREFIX)MenuButton.cpp
$(APPMENU_PAGED_OBJ)FolderComponent.o: \
	$(APPMENU_PAGED_DIR)/$(APPMENU_PAGED_PREFIX)FolderComponent.cpp
$(APPMENU_PAGED_OBJ)MenuComponent.o: \
	$(APPMENU_PAGED_DIR)/$(APPMENU_PAGED_PREFIX)MenuComponent.cpp
$(APPMENU_PAGED_OBJ)InputHandler.o: \
	$(APPMENU_PAGED_DIR)/$(APPMENU_PAGED_PREFIX)InputHandler.cpp
$(APPMENU_PAGED_OBJ)Initializer.o: \
	$(APPMENU_PAGED_DIR)/$(APPMENU_PAGED_PREFIX)Initializer.cpp

$(APPMENU_OBJ)MenuButton.o: \
    $(APPMENU_COMPONENT_DIR)/$(APPMENU_PREFIX)MenuButton.cpp
$(APPMENU_OBJ)FolderComponent.o: \
    $(APPMENU_COMPONENT_DIR)/$(APPMENU_PREFIX)FolderComponent.cpp
$(APPMENU_OBJ)MenuComponent.o: \
    $(APPMENU_COMPONENT_DIR)/$(APPMENU_PREFIX)MenuComponent.cpp
$(APPMENU_OBJ)MainComponent.o: \
    $(APPMENU_COMPONENT_DIR)/$(APPMENU_PREFIX)MainComponent.cpp

$(APPMENU_OBJ)Controller.o: \
    $(APPMENU_CONTROL_DIR)/$(APPMENU_PREFIX)Controller.cpp
$(APPMENU_OBJ)InputHandler.o: \
    $(APPMENU_CONTROL_DIR)/$(APPMENU_PREFIX)InputHandler.cpp

$(APPMENU_OBJ)NewConfigItemEditor.o: \
    $(APPMENU_EDITOR_DIR)/$(APPMENU_PREFIX)NewConfigItemEditor.cpp
$(APPMENU_OBJ)NewDesktopAppEditor.o: \
    $(APPMENU_EDITOR_DIR)/$(APPMENU_PREFIX)NewDesktopAppEditor.cpp
$(APPMENU_OBJ)ExistingItemEditor.o: \
    $(APPMENU_EDITOR_DIR)/$(APPMENU_PREFIX)ExistingItemEditor.cpp
$(APPMENU_OBJ)PopupEditor.o: \
    $(APPMENU_EDITOR_DIR)/$(APPMENU_PREFIX)PopupEditor.cpp
$(APPMENU_OBJ)CategoryEditor.o: \
    $(APPMENU_EDITOR_DIR)/$(APPMENU_PREFIX)CategoryEditor.cpp

$(APPMENU_OBJ)MenuJSON.o: \
    $(APPMENU_DATA_DIR)/JSON/$(APPMENU_PREFIX)MenuJSON.cpp
$(APPMENU_OBJ)ConfigJSON.o: \
    $(APPMENU_DATA_DIR)/JSON/$(APPMENU_PREFIX)ConfigJSON.cpp
$(APPMENU_OBJ)MenuFile.o: \
    $(APPMENU_DATA_DIR)/JSON/$(APPMENU_PREFIX)MenuFile.cpp
$(APPMENU_OBJ)ConfigData.o: \
    $(APPMENU_DATA_DIR)/JSON/$(APPMENU_PREFIX)ConfigData.cpp

$(APPMENU_OBJ)EntryData.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/$(APPMENU_PREFIX)EntryData.cpp
$(APPMENU_OBJ)EntryActions.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/$(APPMENU_PREFIX)EntryActions.cpp
$(APPMENU_OBJ)EntryLoader.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/$(APPMENU_PREFIX)EntryLoader.cpp
$(APPMENU_OBJ)EntryUpdater.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/$(APPMENU_PREFIX)EntryUpdater.cpp

$(APPMENU_OBJ)MenuItem.o: \
    $(APPMENU_DATA_DIR)/$(APPMENU_PREFIX)MenuItem.cpp
$(APPMENU_OBJ)ItemData.o: \
    $(APPMENU_DATA_DIR)/$(APPMENU_PREFIX)ItemData.cpp

$(APPMENU_OBJ)AppMenu.o: \
    $(APPMENU_DIR)/AppMenu.cpp
$(APPMENU_OBJ)ConfigFile.o: \
    $(APPMENU_DIR)/$(APPMENU_PREFIX)ConfigFile.cpp
$(APPMENU_OBJ)SettingsController.o: \
    $(APPMENU_DIR)/$(APPMENU_PREFIX)SettingsController.cpp

# Tests:
$(APPMENU_TEST_OBJ)AppMenu.o: \
    $(APPMENU_TEST_DIR)/AppMenu.cpp
$(APPMENU_TEST_OBJ)ConfigFile.o: \
    $(APPMENU_TEST_DIR)/ConfigFile.cpp
