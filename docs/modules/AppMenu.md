# AppMenu Module Documentation
The AppMenu module displays an application launching menu. This menu displays application shortcuts loaded from standard desktop entry files, and from a JSON configuration file. The user can use this menu to launch applications, or to focus the window of an application that was already launched through this menu.

## Public Interface

#### [AppMenu](../../Source/GUI/AppMenu/AppMenu.h)
AppMenu creates the main menu component, returning it as a generic juce\::Component pointer.

#### [AppMenu\::ConfigFile](../../Source/GUI/AppMenu/AppMenu_ConfigFile.h)
ConfigFile objects get and set menu format and menu display properties.

#### [AppMenu\::SettingsController](../../Source/GUI/AppMenu/AppMenu_SettingsController.h)
SettingsController controls UI components that allow the user to view and change menu format options.

## Implementation

### Menu Behavior

#### [AppMenu\::Initializer](../../Source/GUI/AppMenu/Control/AppMenu_Initializer.h)
The Initializer object creates and initializes the main menu component.

#### [AppMenu\::Controller](../../Source/GUI/AppMenu/Control/AppMenu_Controller.h)
The Controller object handles all core menu functionality. This includes opening and closing folders, creating menu editors, handling popup context menus, and launching application shortcuts.

#### [AppMenu\::InputHandler](../../Source/GUI/AppMenu/Control/AppMenu_InputHandler.h)
The InputHandler object captures key, mouse, and window focus input events, and defines how they are applied to the menu.

### UI Components

#### [AppMenu\::MainComponent](../../Source/GUI/AppMenu/Components/AppMenu_MainComponent.h)
The MainComponent object is the main UI component used to hold all menu content.

#### [AppMenu\::MenuComponent](../../Source/GUI/AppMenu/Components/AppMenu_MenuComponent.h)
MenuComponent creates and displays the menu's folder components.

#### [AppMenu\::FolderComponent](../../Source/GUI/AppMenu/Components/AppMenu_FolderComponent.h)
FolderComponent objects create and display menu buttons for all menu items in a single menu folder.

#### [AppMenu\::MenuButton](../../Source/GUI/AppMenu/Components/AppMenu_MenuButton.h)
MenuButton displays a single item in the menu and allows the user to activate that menu item.

### Editors

#### [AppMenu\::PopupEditor](../../Source/GUI/AppMenu/Components/Editors/AppMenu_PopupEditor.h)
PopupEditor is an abstract basis for all menu item editor components.

#### [AppMenu\::CategoryEditor](../../Source/GUI/AppMenu/Components/Editors/AppMenu_CategoryEditor.h)
CategoryEditor is a UI component used to edit a list of application categories.

#### [AppMenu\::ExistingItemEditor](../../Source/GUI/AppMenu/Components/Editors/AppMenu_ExistingItemEditor.h)
ExistingItemEditor is a PopupEditor used to update an existing menu item.

#### [AppMenu\::NewDesktopAppEditor](../../Source/GUI/AppMenu/Components/Editors/AppMenu_NewDesktopAppEditor.h)
NewDesktopAppEditor is a PopupEditor that creates new .desktop application shortcut files for the current user.

#### [AppMenu\::NewConfigItemEditor](../../Source/GUI/AppMenu/Components/Editors/AppMenu_NewConfigItemEditor.h)
NewConfigItemEditor is a PopupEditor that adds new application shortcuts or folders to the active menu folder.

### Menu Data

#### [AppMenu\::ConfigJSON](../../Source/GUI/AppMenu/Data/JSON/AppMenu_ConfigJSON.h)
The shared ConfigJSON object reads and writes menu format settings in a configurable JSON file.

#### [AppMenu\::ConfigKeys](../../Source/GUI/AppMenu/Data/JSON/AppMenu_ConfigKeys.h)
ConfigKeys defines the JSON keys used within the ConfigJSON object's configuration file.

#### [AppMenu\::ItemData](../../Source/GUI/AppMenu/Data/AppMenu_ItemData.h)
ItemData defines a reference-counted internal data object that holds all menu data for a single item in the menu.

#### [AppMenu\::MenuItem](../../Source/GUI/AppMenu/Data/AppMenu_MenuItem.h)
MenuItem is a nullable interface used to share and interact with any type of ItemData object. 

#### [AppMenu\::MenuJSON](../../Source/GUI/AppMenu/Data/JSON/AppMenu_MenuJSON.h)
The shared MenuJSON object reads from and writes to the main menu structure within a configurable JSON file.

#### [AppMenu\::MenuFile](../../Source/GUI/AppMenu/Data/JSON/AppMenu_MenuFile.h)
MenuFile objects safely share access to the single MenuJSON object.

#### [AppMenu\::MenuKeys](../../Source/GUI/AppMenu/Data/JSON/AppMenu_MenuKeys.h)
MenuKeys defines the JSON keys used within the menu JSON file.

#### [AppMenu\::ConfigData](../../Source/GUI/AppMenu/Data/JSON/AppMenu_ConfigData.h)
ConfigData is an AppMenu\::ItemData subclass that gets its data from the MenuJSON object's configuration file.

#### [AppMenu\::EntryLoader](../../Source/GUI/AppMenu/Data/DesktopEntry/AppMenu_EntryLoader.h)
The EntryLoader object connects with the DesktopEntry module to load DesktopEntry objects for .desktop application shortcut files.

#### [AppMenu\::EntryActions](../../Source/GUI/AppMenu/Data/DesktopEntry/AppMenu_EntryActions.h)
EntryActions provides convenience functions for working with groups of DesktopEntry objects when constructing the menu.

#### [AppMenu\::EntryData](../../Source/GUI/AppMenu/Data/DesktopEntry/AppMenu_EntryData.h)
EntryData is an AppMenu\::ItemData subclass that gets its data from a .desktop application shortcut file, accessed through a DesktopEntry object.

#### [AppMenu\::EntryUpdater](../../Source/GUI/AppMenu/Data/DesktopEntry/AppMenu_EntryUpdater.h)
EntryUpdater objects listen for updates to loaded .desktop files and apply them to the application menu.

### Menu Formats

#### [AppMenu\::Format](../../Source/GUI/AppMenu/Formats/AppMenu_Format.h)
Format defines all available menu format options.

### Scrolling Menu Format
The scrolling menu format displays the application menu as scrolling vertical lists of applications.

#### [AppMenu\::Scrolling\::Initializer](../../Source/GUI/AppMenu/Formats/Scrolling/AppMenu_Scrolling_Initializer.h)
The Initializer object creates the AppMenu using the scrolling menu format.

#### [AppMenu\::Scrolling\::InputHandler](../../Source/GUI/AppMenu/Formats/Scrolling/AppMenu_Scrolling_InputHandler.h)
The InputHandler object controls how user input events are applied to the scrolling menu.

#### [AppMenu\::Scrolling\::MenuComponent](../../Source/GUI/AppMenu/Formats/Scrolling/AppMenu_Scrolling_MenuComponent.h)
The MenuComponent object controls how the scrolling menu format displays and arranges folder components.

#### [AppMenu\::Scrolling\::FolderComponent](../../Source/GUI/AppMenu/Formats/Scrolling/AppMenu_Scrolling_FolderComponent.h)
FolderComponent objects control how the scrolling menu format arranges menu items within each folder.

#### [AppMenu\::Scrolling\::MenuButton](../../Source/GUI/AppMenu/Formats/Scrolling/AppMenu_Scrolling_MenuButton.h)
MenuButton objects control how the scrolling menu format displays individual menu items.

### Paged Menu Format
The paged menu format displays the application menu as a grid of menu items divided into different pages.

#### [AppMenu\::Paged\::Initializer](../../Source/GUI/AppMenu/Formats/Paged/AppMenu_Paged_Initializer.h)
The Initializer object creates the AppMenu using the paged menu format.

#### [AppMenu\::Paged\::InputHandler](../../Source/GUI/AppMenu/Formats/Paged/AppMenu_Paged_InputHandler.h)
The InputHandler object controls how user input events are applied to the paged menu.

#### [AppMenu\::Paged\::MenuComponent](../../Source/GUI/AppMenu/Formats/Paged/AppMenu_Paged_MenuComponent.h)
The MenuComponent object controls how the paged menu format displays and arranges folder components.

#### [AppMenu\::Paged\::FolderComponent](../../Source/GUI/AppMenu/Formats/Paged/AppMenu_Paged_FolderComponent.h)
FolderComponent objects control how the paged menu format arranges menu items within each folder.

#### [AppMenu\::Paged\::MenuButton](../../Source/GUI/AppMenu/Formats/Paged/AppMenu_Paged_MenuButton.h)
MenuButton objects control how the paged menu format displays individual menu items.
