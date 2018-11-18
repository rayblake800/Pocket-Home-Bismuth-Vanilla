/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "JuceHeader.h"

/**
 * @file AppMenu/Implementation.h
 *
 * @brief  Defines all classes used to implement the AppMenu module.
 *
 * This file and all classes it declares may only be included when implementing
 * the AppMenu module. For all other purposes, use the public interface in 
 * AppMenu/AppMenu.h to interact with the AppMenu module.
 */

namespace AppMenu
{
    /* ####################  Implementation   ########################### */
    /* ## These classes are used to implement AppMenu functionality,   ## */
    /* ## and cannot be referenced outside of the AppMenu module.      ## */
    /* ################################################################## */

    /* ####################    Menu Control   ########################### */
    /**
     * @brief  Creates menu objects for a single menu Format type.
     *
     * @see  AppMenu/Control/Initializer.h
     */
    class Initializer;

    /**
     * @brief  Controls the behavior of the main menu component, launching
     *         applications, opening/closing menu folders, and allowing the 
     *         user to edit the menu data.
     *
     * @see  AppMenu/Control/Controller.h
     */
    class Controller;

    /**
     * @brief  Captures and handles key, mouse, and window focus events for the
     *         menu component and all its child components.
     *
     * @see  AppMenu/Control/InputHandler.h
     */
    class InputHandler;

    /* ####################     Menu Data     ########################### */
    /**
     * @brief  Holds data describing a single menu item or folder in the
     *         menu, and provides access to the menu item's parent folder and
     *         child folder items.
     *
     * @see  AppMenu/Data/MenuItem.h
     */
    class MenuItem;

    /**
     * @brief  The abstract internal data object held within a MenuItem, 
     *         allowing menu items to interact with varying data sources through
     *         the same interface.
     *
     * @see  AppMenu/Data/ItemData.h
     */
    class ItemData;

    /**
     * @brief  An ItemData subclass that handles menu data read from desktop
     *         entry files.
     *
     * @see  AppMenu/Data/DesktopEntry/EntryData.h     
     */
    class EntryData;

    /**
     * @brief  Adds initial menu items loaded from desktop entry data when
     *         menu folders are created or loaded.
     *
     * @see  AppMenu/Data/DesktopEntry/EntryLoader.h
     */
    class EntryLoader;

    /**
     * @brief  Listens for changes to desktop entries, copying all changes over
     *         to the menu.
     *
     * @see  AppMenu/Data/DesktopEntry/EntryUpdater.h     
     */
    class EntryUpdater;    

    /**
     * @brief  Provides functions for updating menu folders with desktop entries
     *         that are needed by the EntryLoader and the EntryUpdater.
     *
     * @see  AppMenu/Data/DesktopEntry/EntryActions.h
     */
    namespace EntryActions { }

    /**
     * @brief  An ItemData subclass that handles menu data read from the JSON
     *         configuration file.
     *
     * @see  AppMenu/Data/JSON/ConfigData.h     
     */
    class ConfigData;

    /**
     * @brief  Reads the entire menu structure from a JSON configuration file,
     *         initializes the entire menu as linked ItemData objects, and
     *         provides an interface for adding new menu items.
     *
     * @see  AppMenu/Data/JSON/MenuFile.h     
     */
    class MenuFile;

    /**
     * @brief  The private SharedResource singleton used to read, store, and
     *         write menu data in the JSON menu file.
     *
     * @see  AppMenu/Data/JSON/MenuJSON.h     
     */
    class MenuJSON;

    /* ####################  Menu Components  ########################### */
    /**
     * @brief  A container component that initializes and holds any format of
     *         AppMenu.
     *
     * This selects the displayed menu format, initializes and displays the 
     * menu, and holds the menu's loading spinner component.  Its format may be
     * dynamically changed using AppMenu::changeMenuFormat.
     *
     * @see  AppMenu/Components/MainComponent.h
     */
    class MainComponent;

    /**
     * @brief  Represents the main menu view, creating and positioning folder
     *         components.
     * 
     * @see  AppMenu/Components/MenuComponent.h
     */
    class MenuComponent;

    /**
     * @brief  Represents an open folder in the menu, creating and positioning
     *         menu item button components.
     *
     * @see  AppMenu/Components/FolderComponent.h
     */
    class FolderComponent;

    /**
     * @brief  Represents a single menu item as a button component.
     *
     * @see  AppMenu/omponents/ItemButton.h
     */
    class MenuButton;
    
    /* ####################    Menu Editors   ########################### */
    /**
     * @brief  A component that allows the user to edit menu item data.
     *
     * @see  AppMenu/Components/Editors/PopupEditor.h
     */
    class PopupEditor;

    /**
     * @brief  A component that allows the user to edit a menu item's list of
     *         category strings.
     *
     * @see  AppMenu/Components/Editors/CategoryEditor.h
     */
    class CategoryEditor;

    /**
     * @brief  A PopupEditor that allows the user to edit an existing menu item.
     *
     * @see  AppMenu/Components/Editors/ExistingItemEditor.h
     */
    class ExistingItemEditor;

    /**
     * @brief  A PopupEditor that allows the user to add a new menu item defined
     *         in the JSON menu configuration file.
     *
     * @see  AppMenu/Components/Editors/NewConfigItemEditor.h
     */
    class NewConfigItemEditor;

    /**
     * @brief  A PopupEditor that allows the user to add a new menu item defined
     *         in a desktop entry file.
     *
     * @see  AppMenu/Components/Editors/NewDesktopAppEditor.h
     */
    class NewDesktopAppEditor;

    /* ####################    Menu Formats   ########################### */
    /**
     * @brief  Displays the menu as vertical scrolling menu folder panels.
     *
     * @see  AppMenu/Formats/Scrolling/
     */
    namespace Scrolling
    {
       class Initializer;
       class InputHandler;
       class MenuComponent;
       class FolderComponent;
       class MenuButton;
    }
    
    /**
     * @brief  Displays the menu as pages of menu item grids.
     *
     * @see  AppMenu/Formats/Paged
     */
    namespace Paged
    {
       class Initializer;
       class InputHandler;
       class MenuComponent;
       class FolderComponent;
       class MenuButton;
    }

}
/* Only include this file directly in the AppMenu implementation! */
#endif
