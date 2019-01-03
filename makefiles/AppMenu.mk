############################ AppMenu Module ####################################
APPMENU_PREFIX := $(JUCE_OBJDIR)/AppMenu_
APPMENU_ROOT = Source/AppMenu
APPMENU_TEST_ROOT = Tests/AppMenu

APPMENU_COMPONENT_DIR := $(APPMENU_ROOT)/Components
APPMENU_EDITOR_DIR := $(APPMENU_COMPONENT_DIR)/Editors
APPMENU_CONTROL_DIR := $(APPMENU_ROOT)/Control
APPMENU_DATA_DIR := $(APPMENU_ROOT)/Data
APPMENU_PAGED_DIR := $(APPMENU_ROOT)/Formats/Paged
APPMENU_SCROLLING_DIR := $(APPMENU_ROOT)/Formats/Scrolling

OBJECTS_APPMENU_DATA := \
  $(APPMENU_PREFIX)MenuJSON.o \
  $(APPMENU_PREFIX)ConfigJSON.o \
  $(APPMENU_PREFIX)MenuFile.o \
  $(APPMENU_PREFIX)MenuItem.o \
  $(APPMENU_PREFIX)EntryData.o \
  $(APPMENU_PREFIX)EntryActions.o \
  $(APPMENU_PREFIX)EntryLoader.o \
  $(APPMENU_PREFIX)EntryUpdater.o \
  $(APPMENU_PREFIX)ConfigData.o \
  $(APPMENU_PREFIX)ItemData.o

OBJECTS_APPMENU_COMPONENT := \
  $(APPMENU_PREFIX)MenuButton.o \
  $(APPMENU_PREFIX)FolderComponent.o \
  $(APPMENU_PREFIX)MenuComponent.o \
  $(APPMENU_PREFIX)MainComponent.o 

OBJECTS_APPMENU_CONTROL := \
  $(APPMENU_PREFIX)Controller.o \
  $(APPMENU_PREFIX)InputHandler.o 

OBJECTS_APPMENU_EDITOR := \
  $(APPMENU_PREFIX)NewConfigItemEditor.o \
  $(APPMENU_PREFIX)NewDesktopAppEditor.o \
  $(APPMENU_PREFIX)ExistingItemEditor.o \
  $(APPMENU_PREFIX)CategoryEditor.o \
  $(APPMENU_PREFIX)PopupEditor.o

SCROLLING_MENU_PREFIX := $(APPMENU_PREFIX)Scrolling_
OBJECTS_APPMENU_SCROLLING := \
  $(SCROLLING_MENU_PREFIX)Initializer.o \
  $(SCROLLING_MENU_PREFIX)MenuButton.o \
  $(SCROLLING_MENU_PREFIX)FolderComponent.o \
  $(SCROLLING_MENU_PREFIX)MenuComponent.o \
  $(SCROLLING_MENU_PREFIX)InputHandler.o 

PAGED_MENU_PREFIX := $(APPMENU_PREFIX)Paged_
OBJECTS_APPMENU_PAGED := \
  $(PAGED_MENU_PREFIX)MenuButton.o \
  $(PAGED_MENU_PREFIX)FolderComponent.o \
  $(PAGED_MENU_PREFIX)MenuComponent.o \
  $(PAGED_MENU_PREFIX)InputHandler.o \
  $(PAGED_MENU_PREFIX)Initializer.o 

OBJECTS_APPMENU := \
  $(APPMENU_PREFIX)AppMenu.o \
  $(APPMENU_PREFIX)ConfigFile.o \
  $(OBJECTS_APPMENU_DATA) \
  $(OBJECTS_APPMENU_COMPONENT) \
  $(OBJECTS_APPMENU_CONTROL) \
  $(OBJECTS_APPMENU_EDITOR) \
  $(OBJECTS_APPMENU_SCROLLING) \
  $(OBJECTS_APPMENU_PAGED)  

OBJECTS_APPMENU_TEST := \
  $(APPMENU_PREFIX)Test_AppMenu.o \
  $(APPMENU_PREFIX)Test_ConfigFile.o \
  $(APPMENU_PREFIX)Test_Controller.o \
  $(APPMENU_PREFIX)Test_InputHandler.o \
  $(APPMENU_PREFIX)Test_MenuComponent.o \
  $(APPMENU_PREFIX)Test_PagedMenu.o \
  $(APPMENU_PREFIX)Test_ScrollingMenu.o

ifeq ($(BUILD_TESTS), 1)
    OBJECTS_APPMENU := $(OBJECTS_APPMENU) $(OBJECTS_APPMENU_TEST)
endif

MODULES := $(MODULES) appMenu

OBJECTS_APP := $(OBJECTS_APP) $(OBJECTS_APPMENU)

appMenu : $(OBJECTS_APPMENU)
	@echo "Built AppMenu module"

$(SCROLLING_MENU_PREFIX)MenuButton.o: \
	$(APPMENU_SCROLLING_DIR)/AppMenu_Scrolling_MenuButton.cpp
$(SCROLLING_MENU_PREFIX)FolderComponent.o: \
	$(APPMENU_SCROLLING_DIR)/AppMenu_Scrolling_FolderComponent.cpp
$(SCROLLING_MENU_PREFIX)MenuComponent.o: \
	$(APPMENU_SCROLLING_DIR)/AppMenu_Scrolling_MenuComponent.cpp
$(SCROLLING_MENU_PREFIX)InputHandler.o: \
	$(APPMENU_SCROLLING_DIR)/AppMenu_Scrolling_InputHandler.cpp
$(SCROLLING_MENU_PREFIX)Initializer.o: \
	$(APPMENU_SCROLLING_DIR)/AppMenu_Scrolling_Initializer.cpp

$(PAGED_MENU_PREFIX)MenuButton.o: \
	$(APPMENU_PAGED_DIR)/AppMenu_Paged_MenuButton.cpp
$(PAGED_MENU_PREFIX)FolderComponent.o: \
	$(APPMENU_PAGED_DIR)/AppMenu_Paged_FolderComponent.cpp
$(PAGED_MENU_PREFIX)MenuComponent.o: \
	$(APPMENU_PAGED_DIR)/AppMenu_Paged_MenuComponent.cpp
$(PAGED_MENU_PREFIX)InputHandler.o: \
	$(APPMENU_PAGED_DIR)/AppMenu_Paged_InputHandler.cpp
$(PAGED_MENU_PREFIX)Initializer.o: \
	$(APPMENU_PAGED_DIR)/AppMenu_Paged_Initializer.cpp

$(APPMENU_PREFIX)MenuButton.o: \
    $(APPMENU_COMPONENT_DIR)/AppMenu_MenuButton.cpp
$(APPMENU_PREFIX)FolderComponent.o: \
    $(APPMENU_COMPONENT_DIR)/AppMenu_FolderComponent.cpp
$(APPMENU_PREFIX)MenuComponent.o: \
    $(APPMENU_COMPONENT_DIR)/AppMenu_MenuComponent.cpp
$(APPMENU_PREFIX)MainComponent.o: \
    $(APPMENU_COMPONENT_DIR)/AppMenu_MainComponent.cpp

$(APPMENU_PREFIX)Controller.o: \
    $(APPMENU_CONTROL_DIR)/AppMenu_Controller.cpp
$(APPMENU_PREFIX)InputHandler.o: \
    $(APPMENU_CONTROL_DIR)/AppMenu_InputHandler.cpp

$(APPMENU_PREFIX)NewConfigItemEditor.o: \
    $(APPMENU_EDITOR_DIR)/AppMenu_NewConfigItemEditor.cpp
$(APPMENU_PREFIX)NewDesktopAppEditor.o: \
    $(APPMENU_EDITOR_DIR)/AppMenu_NewDesktopAppEditor.cpp
$(APPMENU_PREFIX)ExistingItemEditor.o: \
    $(APPMENU_EDITOR_DIR)/AppMenu_ExistingItemEditor.cpp
$(APPMENU_PREFIX)PopupEditor.o: \
    $(APPMENU_EDITOR_DIR)/AppMenu_PopupEditor.cpp
$(APPMENU_PREFIX)CategoryEditor.o: \
    $(APPMENU_EDITOR_DIR)/AppMenu_CategoryEditor.cpp

$(APPMENU_PREFIX)MenuJSON.o: \
    $(APPMENU_DATA_DIR)/JSON/AppMenu_MenuJSON.cpp
$(APPMENU_PREFIX)ConfigJSON.o: \
    $(APPMENU_DATA_DIR)/JSON/AppMenu_ConfigJSON.cpp
$(APPMENU_PREFIX)MenuFile.o: \
    $(APPMENU_DATA_DIR)/JSON/AppMenu_MenuFile.cpp
$(APPMENU_PREFIX)ConfigData.o: \
    $(APPMENU_DATA_DIR)/JSON/AppMenu_ConfigData.cpp

$(APPMENU_PREFIX)EntryData.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/AppMenu_EntryData.cpp
$(APPMENU_PREFIX)EntryActions.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/AppMenu_EntryActions.cpp
$(APPMENU_PREFIX)EntryLoader.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/AppMenu_EntryLoader.cpp
$(APPMENU_PREFIX)EntryUpdater.o: \
    $(APPMENU_DATA_DIR)/DesktopEntry/AppMenu_EntryUpdater.cpp

$(APPMENU_PREFIX)MenuItem.o: \
    $(APPMENU_DATA_DIR)/AppMenu_MenuItem.cpp
$(APPMENU_PREFIX)ItemData.o: \
    $(APPMENU_DATA_DIR)/AppMenu_ItemData.cpp

$(APPMENU_PREFIX)AppMenu.o: \
    $(APPMENU_ROOT)/AppMenu.cpp
$(APPMENU_PREFIX)ConfigFile.o: \
    $(APPMENU_ROOT)/AppMenu_ConfigFile.cpp

# Tests:
$(APPMENU_PREFIX)Test_AppMenu.o: \
    $(APPMENU_TEST_ROOT)/AppMenu.cpp
$(APPMENU_PREFIX)Test_ConfigFile.o: \
    $(APPMENU_TEST_ROOT)/ConfigFile.cpp
$(APPMENU_PREFIX)Test_Controller.o: \
    $(APPMENU_TEST_ROOT)/Controller.cpp
$(APPMENU_PREFIX)Test_InputHandler.o: \
    $(APPMENU_TEST_ROOT)/InputHandler.cpp
$(APPMENU_PREFIX)Test_MenuComponent.o: \
    $(APPMENU_TEST_ROOT)/MenuComponent.cpp
$(APPMENU_PREFIX)Test_PagedMenu.o: \
    $(APPMENU_TEST_ROOT)/PagedMenu.cpp
$(APPMENU_PREFIX)Test_ScrollingMenu.o: \
    $(APPMENU_TEST_ROOT)/ScrollingMenu.cpp
