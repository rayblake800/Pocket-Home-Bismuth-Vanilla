# Home Page and Application Menu

![HomePage Guide](../images/screenshots/pages/home.png?raw=true "HomePage Guide")
1. The battery icon shows the system's remaining battery percentage, and whether the system is charging.
    * [System Command Options](../configuration/commands.md): The command used to read the current battery percentage is stored under the **"get battery percent"** key.
    * [Image Asset Configuration](../configuration/imageAssets.md): All battery level icons and colors are stored under the **"battery"** key.
    * [Layout Configuration](../configuration/layout.md): The relative bounds of the icon are stored under the **"home page battery icon"** key, and the relative bounds of the percentage label are stored under the **"battery percent text"** key.
2. The clock label displays the current time.
   * [General Configuration](../configuration/config.md): Whether the clock uses twelve hour or twenty four hour mode is stored under the **"Use 24h mode"** key, and whether the clock is shown at all is stored under the **"Show Clock"** key. These options may also be changed on the [home settings page](./HomeSettings.md).
    * [Layout Configuration](../configuration/layout.md): The relative bounds of the clock are stored under the **"home page clock label"** key.
3. The signal icon shows the strength of the current network connection.
    * [Image Asset Configuration](../configuration/imageAssets.md): All signal strength icons and colors are stored under the **"signal strength"** key.
    * [Layout Configuration](../configuration/layout.md): The relative bounds of the icon are stored under the **"home page Wifi icon"** key.
4. The power button opens the [power page](./PowerPage.md).
    * [Image Asset Configuration](../configuration/imageAssets.md): The button icon and colors are stored under the **"power button"** key.
    * [Layout Configuration](../configuration/layout.md): The relative bounds of the button are stored under the **"power page button"** key.
5. The settings button opens the [quick settings page](./QuickSettings.md).
    * [Image Asset Configuration](../configuration/imageAssets.md): The button icon and colors are stored under the **"settings button"** key.
    * [Layout Configuration](../configuration/layout.md): The relative bounds of the button are stored under the **"settings page button"** key.

---

## Application Menu Controls

#### Keyboard Controls
Menu Action       | Key Bindings
----------------- | ---
navigate up       | Up
navigate down     | Down
navigate left     | Left
navigate right    | Right
select/confirm    | Enter
deselect/cancel   | Escape
select next item  | Tab
open context menu | Ctrl+E

In all menu formats, right clicking or control-clicking menu items or empty space in the menu will open a context menu.

#### Paged Menu Format:
![Paged Menu](../images/paged.gif?raw=true "Paged Menu Format")

Click on menu items to launch applications and open folders. Click the navigation arrow buttons on the left and right side of the page to scroll through the open folder. If not within the base folder, the navigation arrow button on the top of the page closes the current folder.

Use the arrow keys or the tab key to select menu items, and use the enter key to open them. The active folder page will automatically move to follow the current selection. When a menu item is selected, the escape key deselects it. When no item is selected and the folder is not on its first page, the escape key scrolls to the first page of the folder. When on the first page of a menu folder with no items selected, the escape key closes the folder.

#### Scrolling Menu Format:
![Scrolling Menu](../images/scrolling.gif?raw=true "Scrolling Menu Format")

Clicking on unselected menu items selects them, clicking on selected menu items opens them. When multiple folders are open, clicking on menu items in an inactive folder closes all folders above that folder.

Use the up and down keys or the tab key to scroll through the menu. Left and escape deselect the selected menu item, or close the active folder if no item is selected and multiple folders are open. Right and enter open the selected menu item, or select the first item in the active folder if no menu item is selected.

## Context Menus:
Context menus allow you to edit the menu structure from within the application. The menu structure can also be edited directly in the [application menu configuration file](../configuration/appMenu.md).

#### Main Context Menu:
![Main Context Menu](../images/screenshots/menus/main.png?raw=true "Main context menu")

This context menu appears when you right-click or control-click an empty spot in the menu, or press Ctrl+E when no menu items are selected.
1. Open an editor to create a new application shortcut to use only in this application.
2. Open an editor to create a new subfolder.
3. Open an editor to create a new desktop entry application shortcut which may be used by other applications.

#### Shortcut Menu:
![Shortcut Menu](../images/screenshots/menus/shortcut.png?raw=true "Shortcut menu")

This context menu appears when you right-click or control-click an application shortcut in the menu, or press Ctrl+E when a shortcut is selected.
1. Open an editor to update the shortcut.
2. Permanently remove the shortcut from the menu.
3. Swap the shortcut with the menu item before it.
4. Swap the shortcut with the menu item after it.

#### Folder Menu:
![Folder Menu](../images/screenshots/menus/folder.png?raw=true "Folder menu")

This context menu appears when you right-click or control-click a folder menu item, or press Ctrl+E when a folder is selected.
1. Open an editor to update the folder.
2. Permanently remove the folder from the menu.
3. Open an editor to create a new shortcut within the folder.
4. Swap the folder with the menu item before it.
5. Swap the shortcut with the menu item after it.

#### Desktop Entry Menu:
![Desktop Entry Menu](../images/screenshots/menus/desktopEntry.png?raw=true "Desktop entry menu")

This context menu appears when you right-click or control-click a desktop entry menu item, or press Ctrl+E when a desktop entry item is selected.
1. Open an editor to update the desktop entry.
2. Hide the desktop entry from the current user.
3. Copy this menu item to the main menu folder as a shortcut.

## Menu Editors:
Menu editors are opened from context menus, and are used to create and edit items in the application menu.

#### Application Shortcut Editor:
![Shortcut Editor](../images/screenshots/editors/shortcut.png?raw=true "Shortcut editor")

This editor creates and edits application shortcuts stored within the [application menu configuration file](../configuration/appMenu.md).
1. Sets the shortcut's title within the menu.
2. Sets the shortcut's menu icon. Icons can be selected using either full image file paths or using icon names to look up within standard icon theme directories.
3. Opens a file selection window to pick a new icon path.
4. Sets the command used to launch the shortcut's application.
5. Sets if the application should run within a terminal window. The command prefix used to run applications within terminal windows is set in the [main configuration file](../configuration/config.md) under the **"Terminal launch command"** key.
6. Closes the editor and discards all changes.
7. Closes the editor and saves all changes.

#### Folder Editor:
![Folder Editor](../images/screenshots/editors/folder.png?raw=true "Folder editor")

This editor creates and edits subfolders within the application menu.
1. Sets the folder's title within the menu.
2. Sets the folder's menu icon. Icons can be selected using either full image file paths or using icon names to look up within standard icon theme directories.
3. Opens a file selection window to pick a new icon path.
4. Opens a category editor to update the folder's application categories. The folder will use these categories to select the desktop entry menu items it should contain.
5. Closes the editor and discards all changes.
6. Closes the editor and saves all changes.

#### Desktop Entry Editor:
![Desktop Entry Editor](../images/screenshots/editors/desktopEntry.png?raw=true "Desktop entry editor")

This editor creates and edits desktop entries, standard application shortcut files used by many different applications. Changes made through this editor will apply to only the current user.
1. Sets the desktop entry's title.
2. Sets the desktop entry's menu icon. Icons can be selected using either full image file paths or using icon names to look up within standard icon theme directories.
3. Opens a file selection window to pick a new icon path.
4. Opens a category editor to update the desktop entry's application categories. These categories will be used by menu folders to select appropriate application entries.
5. Sets if the application should run within a terminal window. The command prefix used to run applications within terminal windows is set in the [main configuration file](../configuration/config.md) under the **"Terminal launch command"** key.
6. Closes the editor and discards all changes.
7. Closes the editor and saves all changes.

#### Category Editor:
![Category Editor](../images/screenshots/editors/category.png?raw=true "Category editor")

This editor is used to edit lists of application categories.
1. The list of categories. Click on menu items to select them, and double click on them to make them editable. Scroll through the list with the arrow keys when a menu item is selected, or using the scrollbar on the right. Clicking the X to the right of a category deletes it from the list.
2. The text editor used to add new categories. Type new category names in this field, then press enter or click on the '+' button to the right to add the new category to the list.
3. Closes the editor and discards all changes.
4. Closes the editor and saves all changes.
