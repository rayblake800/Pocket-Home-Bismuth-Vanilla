# Application Menu Structure Configuration
The [appMenu.json](../../assets/configuration/appMenu.json) configuration file stores the structure of the Pocket-Home application menu. It defines the order, names, and properties of menu folders and pinned application shortcuts.

The menu file stores all menu items in the form of nested JSON objects. The values defined within each menu item control how it will appear and behave. Menu items can either be application shortcuts used to run a launch command, or folders that contain other menu items.

The configuration file's root JSON object is treated as one of these JSON objects, used to load the menu's main folder. Only the "folder items" key is used when loading the root object, all other menu item keys are ignored.

## Menu Item Keys
Key                  | Description
-------------------- | ---
"name"               | A name describing the menu item, to be printed by the item when it is drawn in the application menu.
"icon"               | The name or path of an icon image drawn to represent the menu item. See the [Icon module documentation](../modules/Icon.md) for more information on how icon values are used.
"command"            | A launch command to run when the menu item is clicked. If the menu item also defines folder items or categories, this value will be ignored.
"launch in terminal" | Whether the menu items command should run normally, or within a new terminal window. If the menu item also defines folder items or categories, this value will be ignored.
"folder items"       | A list of other menu items to be added to the start of the folder this menu item opens.
"categories"         | A list of menu item categories used to select desktop entry application shortcuts to add to this folder menu item. See the [desktop entry specification](https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html) for more information on how categories are used to load menu shortcuts.
