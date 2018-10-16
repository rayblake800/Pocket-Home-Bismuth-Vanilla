#pragma once

/**
 * @file  AppMenu.h
 *
 * @brief  Defines all classes within the application menu component
 *         module.
 */

namespace AppMenu
{
    /**
     * @brief  The public interface for the application menu component.
     *
     * This lists all available menu types, selects the displayed menu type,
     * and displays the menu.
     *
     * @see AppMenu/MainComponent.h
     */
    class MainComponent;

    // AppMenu implementation classes:
    // These may not be included or used in any classes outside of the 
    // AppMenu module.
    
    /**
     * @brief  Loads the menu structure from a JSON configuration file.
     *
     * @see AppMenu/ConfigFile.h
     */
    class ConfigFile;

    /**
     * @brief  Holds data describing a single menu item or folder in the
     *         menu.
     *
     * @see AppMenu/MenuItem.h
     */
    class MenuItem;

    /**
     * @brief  Controls the behavior of the main menu component, launching
     *         applications, opening/closing menu folders, and allowing the 
     *         user to edit the menu data.
     *
     * @see AppMenu/Controller.h
     */
    class Controller;

    /**
     * @brief  Represents the main menu view, creating and positioning folder
     *         components.
     * 
     * @see AppMenu/MenuComponent.h
     */
    class MenuComponent;

    /**
     * @brief  Represents an open folder in the menu, creating and positioning
     *         menu item components.
     *
     * @see AppMenu/FolderComponent.h
     */
    class FolderComponent;

    /**
     * @brief  Represents a single menu item as a button component.
     *
     * @see AppMenu/ItemButton.h
     */
    class ItemButton;

    /* Menu Types: */
    /**
     * @brief  Defines the available menu implementations.  Each one has a
     *         namespace of the same name within the AppMenu namespace where
     *         all of its classes are declared.
     */
    enum class Type
    {
        Scrolling,
        Paged
    };

    /**
     * @brief  Represents the menu using scrolling vertical folder components.
     *
     * @see AppMenu/Scrolling/
     */
    namespace Scrolling
    {
       class Controller;
       class MenuComponent;
       class FolderComponent;
       class ItemButton;
    }
    
    /**
     * @brief  Represents the menu using fullscreen folder pages that show
     *         application grids.
     *
     * @see AppMenu/Paged
     */
    namespace Paged
    {
       class Controller;
       class MenuComponent;
       class FolderComponent;
       class ItemButton;
    }

}
