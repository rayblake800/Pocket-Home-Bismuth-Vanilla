/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "Config/FileResource.h"
#include "Config/FileHandler.h"
#include "AppMenu/AppMenu.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Data/ConfigData.h"

/**
 * @file JSONResource.h
 *
 * @brief  Reads application menu settings from the apps.json configuration 
 *         file.
 *
 * JSONResource defines the class of the singleton SharedResource object that
 * accesses apps.json, the configuration file where application menu settings
 * are stored.  It reads in the JSON data used to create AppMenu::MenuItem
 * objects, and writes any changes to those objects back to apps.json
 * as JSON data.  Only AppMenu::ConfigFile objects are allowed to access the 
 * JSONResource object.
 *
 * @see AppMenu/ConfigFile.h
 */

class AppMenu::JSONResource : public Config::FileResource
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    JSONResource();

    /**
     * @brief  Destroys all menu data on destruction.
     */
    virtual ~JSONResource();

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

private:
    /**
     * @brief  Copies all menu data back to the JSON configuration file.
     */
    void writeDataToJSON() override final;

    /**
     * @brief   Gets all parameters with basic data types tracked by this
     *          ConfigFile.
     * 
     * @return  The menu setting keys defined in AppMenu/Data/ConfigKeys.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() 
        const final override;

    /* Holds the root folder item */
    MenuItem rootFolderItem;
    
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
         * @brief  Removes this menu item from JSONResource's config file.
         */
        virtual void deleteFromSource() override;

        /**
         * @brief  Creates an empty child menu item.
         *
         * @return  A pointer to an empty JSONResource::ConfigData object.
         */
        virtual ConfigData::Ptr createChildItem() override;

        /**
         * @brief  A private Config::FileHandler used only for writing menu 
         *         changes.
         */
        class JSONWriter : public Config::FileHandler<JSONResource>
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

/* Only include this file directly in the AppMenu implementation! */
#endif
