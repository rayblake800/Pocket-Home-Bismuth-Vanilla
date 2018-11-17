/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "Config/FileHandler.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Data/JSON/MenuJSON.h"
#include "AppMenu/AppMenu.h"

/**
 * @file MenuFile.h
 * 
 * @brief  Reads and edits the tree of pinned application shortcuts and folders 
 *         displayed in the AppMenu, and loads AppMenu layout preferences.
 *
 * The apps.json file defines a tree of menu items.  Each menu item can hold 
 * either an application launch command, or a list of other menu items and
 * application categories to show in a new menu folder.  All menu items have a
 * displayed title and icon.
 *
 * AppMenu::MenuFile uses this JSON menu data to construct the 
 * AppMenu::MenuItem objects used to build the AppMenuComponent, and it provides 
 * an interface for adding new menu items to the JSON file.  
 *
 * Along with the main menu structure, apps.json contains a few menu format
 * configuration options, as defined in AppMenu::ConfigKeys. 
 * AppMenu::MenuFile reads and writes these values using the standard 
 * Config::FileHandler interface.
 * 
 * @see  AppMenu/AppMenu.h
 *       AppMenu/Data/JSON/ConfigKeys.h
 *       AppMenu/Data/JSON/MenuJSON.h
 */
class AppMenu::MenuFile : public Config::FileHandler<AppMenu::MenuJSON>
{
public:
    MenuFile() { }

    virtual ~MenuFile() { }
    
    /**
     * @brief  Gets a menu item representing the root folder of the application
     *         menu.
     *
     * @return  A folder menu item holding the top level of the application
     *          menu tree.
     */
    MenuItem getRootFolderItem();

    /**
     * @brief  Adds a new menu item to the list of menu items.
     *
     * @param title            The title to print on the menu item.
     *
     * @param icon             The name or path of the menu item's icon.
     *
     * @param command          The menu item's application launch command, or 
     *                         the empty string if the menu item does not launch 
     *                         an application.
     *
     * @param launchInTerm     Whether the menu item launches an application
     *                         within a new terminal window.
     * 
     * @param categories       A list of application categories associated with
     *                         the menu item.
     * 
     * @param parentFolder     The parent folder item the new menu item will be
     *                         inserted into.
     *
     * @param index            The index where the menu item will be inserted
     *                         into the parent folder.
     *
     * @return                 The created menu item, or a null menu item if
     *                         creating the menu item failed.
     */
    MenuItem addMenuItem(
            const juce::String& title, 
            const juce::String& icon,
            const juce::String& command,
            const bool launchInTerm,
            const juce::StringArray& categories,
            MenuItem& parentFolder,
            const int index);

    /* Listens for changes to AppMenu::MenuFile's basic data values */
    class Listener : public Config::FileResource::Listener
    {
    public:
        Listener();

        virtual ~Listener() { }
    };
};

/* Only include this file directly in the AppMenu implementation! */
#endif
