#ifndef APPMENU_IMPLEMENTATION
  #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file AppMenu_MenuJSON.h
 *
 * @brief  Reads from and writes to the menu tree defined in the appMenu.json 
 *         configuration file
 */

#include "Config_FileResource.h"
#include "Config_FileHandler.h"
#include "AppMenu_EntryLoader.h"
#include "AppMenu_MenuItem.h"
#include "AppMenu_ConfigData.h"

namespace AppMenu { class MenuJSON; }

/**
 *  MenuJSON is the class of the singleton SharedResource object that accesses 
 * appMenu.json, the configuration file where the application menu structure is
 * defined. Only AppMenu::ConfigFile objects are allowed to directly access the 
 * MenuJSON object.
 *
 *  MenuJSON reads in the JSON data used to create AppMenu::MenuItem objects,
 * and writes any changes to those objects back to appMenu.json as JSON data. 
 */
class AppMenu::MenuJSON : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    /**
     * @brief  Initializes the menu data tree.
     */
    MenuJSON();

    /**
     * @brief  Destroys all menu data on destruction.
     */
    virtual ~MenuJSON();

    /**
     * @brief  Gets a menu item representing the root folder of the application
     *         menu.
     *
     * @return  The root folder menu item.
     */
    MenuItem getRootFolderItem() const;

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

    /**
     * @brief  Copies all menu data back to the resource's cached JSON data.
     */
    void writeDataToJSON() override final;
    
    /**
     * @brief  Writes all menu changes back to the menu JSON file.
     */
    void writeMenuChanges();

private:
    /**
     * @brief   Gets all parameters with basic data types tracked by this
     *          ConfigFile.
     * 
     * @return  The empty list, as MenuJSON only reads the JSON object used to
     *          create the menu.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() const final 
        override;
    
    /**
     * @brief  Recursively copies a menu item and all of its child folder items
     *         into a juce::var object.
     *
     * @param menuItem  A non-null item located in the application menu.
     *
     * @return          All menu item data packaged in an object var. 
     */
    static juce::var itemToVar(const AppMenu::MenuItem& menuItem);

    /* Holds the root folder item */
    MenuItem rootFolderItem;

    /* Loads desktop entry folder items. */
    EntryLoader entryLoader;
    
    /* Private JSON menu data class */
    class ConfigData : public AppMenu::ConfigData
    {
    public:
        ConfigData() { }

        virtual ~ConfigData() { }
    
        /**
         * @brief  Writes all changes to this menu item back to its data source.
         */
        virtual void saveChanges() override;

    private:
        /**
         * @brief  Removes this menu item from MenuJSON's config file.
         */
        virtual void deleteFromSource() override;

        /**
         * @brief  Creates an empty child menu item.
         *
         * @return  A pointer to an empty MenuJSON::ConfigData object.
         */
        virtual ConfigData::Ptr createChildItem() override;

        /**
         * @brief  A private Config::FileHandler used only for writing menu 
         *         changes.
         */
        class JSONWriter : public Config::FileHandler<MenuJSON>
        {
        public:
            JSONWriter() { }
            virtual ~JSONWriter() { }

            /**
             * @brief  Writes all config-defined menu data back to the JSON 
             *         file.  
             */
            void writeChanges();
        };
    };
};
