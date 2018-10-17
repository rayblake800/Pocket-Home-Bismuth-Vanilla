#pragma once
#include "DesktopEntryLoader.h"
#include "Config/FileResource.h"
#include "Config/FileHandler.h"
#include "MenuItem.h"
#include "AppMenu.h"

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
 * as JSON data.  Only AppConfigFile objects are allowed to access the JSONResource
 * object.
 *
 * @see AppConfigFile.h
 */

class AppMenu::JSONResource : public Config::FileResource, 
    private MenuItem::Editor
{
public:
    /* SharedResource object key */
    static const juce::Identifier resourceKey;

    JSONResource();

    virtual ~JSONResource() { }

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
     * @return  The empty list, as AppConfigFile doesn't track any ConfigKey
     *          variables.
     */
    virtual const std::vector<Config::DataKey>& getConfigKeys() 
        const final override
    {
        static const std::vector<Config::DataKey> keys = {};
        return keys;
    }

    /* Holds the root folder item */
    MenuItem rootFolderItem;

    /* Private JSON menu data class */
    class ConfigItemData : public ItemData, private Localized
    {
    public:
        /**
         * @brief  Recursively creates a menu item and all its child folder 
         *         items.
         *
         * @param menuData  A JSON data object defining a menu item.
         */
        ConfigItemData(juce::var& menuData);

        /**
         * @brief  Cancels any desktop entry menu items waiting to load before
         *         destroying this menu item.
         */
        virtual ~ConfigItemData();

        /**
         * @brief  Gets the menu item's displayed title.
         *
         * @return  The displayed title string.
         */
        virtual juce::String getTitle() const override;

        /**
         * @brief  Gets the name or path used to load the menu item's icon file.
         *
         * @return  The name or path of the icon.
         */
        virtual juce::String getIconName() const override;

        /**
         * @brief  Gets the menu item's application launch command.
         *
         * @return  The launch command string, or the empty string if the menu 
         *          item does not launch an application.
         */
        virtual juce::String getCommand() const override;

        /**
         * @brief  Checks if this menu item launches an application in a new
         *         terminal window.
         *
         * @return  True if and only if the menu item has a launch command it 
         *          should run in a new terminal window
         */
        virtual bool getLaunchedInTerm() const override;

        /**
         * @brief  Gets the application categories used to load this item's
         *         desktop entry child folder items.
         *
         * @return  Any category strings assigned to this menu item.
         */
        virtual juce::StringArray getCategories() const override;

        /**
         * @brief  Sets the menu item's displayed title.
         *
         * @param title  The new title string to display.
         */
        virtual void setTitle(const juce::String& title) override;

        /**
         * @brief  Sets the name or path used to load the menu item's icon file.
         *
         * @param iconName  The new icon name or path.
         */
        virtual void setIconName(const juce::String& iconName) override;

        /**
         * @brief  Sets the menu item's application launch command.
         *
         * @param newCommand  The new command string to run when this menu item 
         *                    is clicked.
         */
        virtual void setCommand(const juce::String& newCommand) override;

        /**
         * @brief  Sets if this menu item runs its command in a new terminal 
         *         window.
         *
         * @param termLaunch  True to run any launch command assigned to this
         *                    menu item within a new terminal window, false to
         *                    run commands normally.
         */
        virtual void setLaunchedInTerm(const bool termLaunch) override;

        /**
         * @brief  Sets the application categories used to load this item's
         *         desktop entry child folder items.
         *
         * @param categories  The new set of category strings to assign to this 
         *                    menu item.
         */
        virtual void setCategories(const juce::StringArray& categories) 
            override;

        /**
         * @brief  Gets the number of folder items held by this menu item that 
         *         can be reordered.
         *
         * @return  The number of child folder items held by this menu item that
         *          are directly defined by JSON data.
         */
        virtual int getMovableChildCount() const override;
        
        /**
         * @brief  Writes all changes to this menu item back to its data source.
         */
        virtual void saveChanges() override;
        
        /**
         * @brief  Gets an appropriate title to use for a deletion confirmation 
         *         window.
         *
         * @return  A localized confirmation title string.
         */
        virtual juce::String getConfirmDeleteTitle() const override;

        /**
         * @brief  Gets appropriate descriptive text for a deletion confirmation 
         *         window.
         *
         * @return  A localized confirmation description string.
         */
        virtual juce::String getConfirmDeleteMessage() const override;

        /**
         * @brief  Gets an appropriate title to use for a menu item editor.
         *
         * @return  A localized editor title string.
         */
        virtual juce::String getEditorTitle() const override;

        /**
         * @brief  Checks if a data field within this menu item can be edited.
         *
         * @param dataField  The type of data being checked.
         *
         * @return           True if and only if the data field is editable.
         */
        virtual bool isEditable(const DataField dataField) const override;

        /**
         * @brief  Loads all desktop entry child menu items defined by the menu
         *         item's category list.
         *
         * This will do nothing if this menu item has already started loading
         * desktop entry menu items.
         */
        void loadDesktopEntryItems();

    private:
        /**
         * @brief  Removes this menu item from JSONResource's config file.
         */
        virtual void deleteFromSource() override;

        /* The displayed title */
        juce::String title;

        /* The icon name or path */
        juce::String iconName;

        /* An application launch command, or the empty string. */
        juce::String command;

        /* Whether the menu item launches an application in a terminal window. */
        bool launchInTerm = false;

        /* Application categories used to load desktop entry folder items. */
        juce::StringArray categories;

        /* The number of movable child folder items defined in apps.json. */
        int movableChildCount = 0;

        /* Holds the ID of a pending callback function that will load 
           DesktopEntry menu items. */
        juce::Atomic<DesktopEntryLoader::CallbackID> pendingCallbackID;

        /**
         * @brief  Private ConfigFile class only used by ConfigMenuItem to 
         *         update its data.
         */
        class JSONWriter : public Config::FileHandler<JSONResource>
        {
        public:
           JSONWriter() { }
           virtual ~JSONWriter() { }

           /**
            * @brief  Writes all config-defined menu data back to the JSON file.
            */
           void writeChanges();
        };
    };
};
