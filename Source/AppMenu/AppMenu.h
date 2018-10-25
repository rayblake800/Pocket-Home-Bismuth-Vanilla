#pragma once

/**
 * @file  AppMenu.h
 *
 * @brief  Defines all classes within the application menu component
 *         module.
 */

namespace AppMenu
{
    /* ####################  Public Interface ########################### */
    /* ## All outside interaction with the AppMenu module should be    ## */
    /* ## through these classes and functions.                         ## */
    /* ################################################################## */
    /**
     * @brief  Defines the available menu implementations.  Each one (besides
     *         Invalid) has a namespace of the same name within the AppMenu 
     *         namespace where all of its classes are declared.
     */
    enum class Format
    {
        /* Displays the menu as vertical scrolling menu folder panels: */
        Scrolling,
        /* Displays the menu as pages of menu item grids: */
        Paged,
        /* The Format value returned if the menu is somehow null or invalid: */
        Invalid
    };


    /**
     * @brief  A container component that initializes and holds any format of
     *         AppMenu.
     *
     * This selects the displayed menu format, initializes and displays the 
     * menu, and holds the menu's loading spinner component.  It can also be
     * used to dynamically change the menu format.
     *
     * @see  MainComponent.h
     */
    class MainComponent;
    
    /**
     * @brief  Creates an AppMenu::MainComponent with a specific initial format.
     *
     * @param menuFormat   The initial format of the Component's menu.
     *
     * @return             A pointer to a newly allocated and initialized
     *                     AppMenu::MainComponent.
     */
    MainComponent* createAppMenu(const Format menuFormat);

    /**
     * @brief   Changes the format of an existing AppMenu::MainComponent.
     *
     * @param appMenu     The AppMenu::MainComponent to update.  If this pointer
     *                    is null, or the new menu format is the same as the
     *                    existing format, no action will be taken.
     *
     * @param newFormat   The new menu format to apply to the menu.
     */
    void changeMenuFormat(MainComponent* appMenu, const Format newFormat);
    
    /* ####################    Menu Control   ########################### */

    /**
     * @brief  Creates menu objects for a single menu Format type.
     *
     * @see  Control/Initializer.h
     */
    class Initializer;

    /**
     * @brief  Controls the behavior of the main menu component, launching
     *         applications, opening/closing menu folders, and allowing the 
     *         user to edit the menu data.
     *
     * @see  Controller/Controller.h
     */
    class Controller;


    /**
     * @brief  Captures and handles key and mouse events for the menu component
     *         and all its child components.
     *
     * @see  Controller/InputHandler.h
     */
    class InputHandler;

    /**
     * @brief  A context menu that provides different options for editing the
     *         menu depending on which menu item was selected or clicked.
     *
     * @see  Controller/ContextMenu.h
     */
    class ContextMenu;

    /* ####################     Menu Data     ########################### */
    /**
     * @brief  Holds data describing a single menu item or folder in the
     *         menu, and provides access to the menu item's parent folder and
     *         child folder items.
     *
     * MenuItem also provides the MenuItem::Editor interface used to make
     * changes to menu data.
     *
     * @see  MenuData/MenuItem.h
     */
    class MenuItem;

    /**
     * @brief  The abstract internal data object held within a MenuItem, 
     *         allowing menu items to interact with varying data sources through
     *         the same interface.
     *
     * @see  MenuData/ItemData.h
     */
    class ItemData;

    /**
     * @brief  An ItemData subclass that handles menu data read from desktop
     *         entry files.
     *
     * @see  MenuData/DesktopEntryData
     */
    class DesktopEntryData;
    
    /**
     * @brief  An ItemData subclass that handles menu data read from the JSON
     *         configuration file.
     *
     * @see  MenuData/ConfigData
     */
    class ConfigData;

    /**
     * @brief  Reads the entire menu structure from a JSON configuration file,
     *         initializes the entire menu as linked ItemData objects, and
     *         provides an interface for adding new menu items.
     *
     * @see  MenuData/ConfigFile.h
     */
    class ConfigFile;

    /**
     * @brief  The private SharedResource singleton used to read, store, and
     *         write data in the JSON configuration file.
     *
     * @see  MenuData/JSONResource.h
     */
    class JSONResource;

    /* ####################  Menu Components  ########################### */
    /**
     * @brief  Represents the main menu view, creating and positioning folder
     *         components.
     * 
     * @see  MenuComponents/MenuComponent.h
     */
    class MenuComponent;

    /**
     * @brief  Represents an open folder in the menu, creating and positioning
     *         menu item button components.
     *
     * @see  MenuComponents/FolderComponent.h
     */
    class FolderComponent;

    /**
     * @brief  Represents a single menu item as a button component.
     *
     * @see  MenuComponents/ItemButton.h
     */
    class MenuButton;
    
    /* ####################    Menu Editors   ########################### */
    /**
     * @brief  A component that allows the user to edit menu item data.
     *
     * @see  Editors/PopupEditor.h
     */
    class PopupEditor;

    /**
     * @brief  A component that allows the user to edit a menu item's list of
     *         category strings.
     *
     * @see  Editors/CategoryEditor.h
     */
    class CategoryEditor;

    /**
     * @brief  A PopupEditor that allows the user to edit an existing menu item.
     *
     * @see  Editors/ExistingItemEditor.h
     */
    class ExistingItemEditor;

    /**
     * @brief  A PopupEditor that allows the user to add a new menu item defined
     *         in the JSON menu configuration file.
     *
     * @see  Editors/NewConfigItemEditor.h
     */
    class NewConfigItemEditor;
    /**
     * @brief  A PopupEditor that allows the user to add a new menu item defined
     *         in a desktop entry file.
     *
     * @see  Editors/NewDesktopAppEditor.h
     */
    class NewDesktopAppEditor;

    /* ####################    Menu Formats   ########################### */
    /**
     * @brief  Displays the menu as vertical scrolling menu folder panels.
     *
     * @see  MenuFormats/Scrolling/
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
     * @see  MenuFormats/Paged
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
