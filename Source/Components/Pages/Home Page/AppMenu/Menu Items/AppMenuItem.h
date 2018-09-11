#pragma once
#include "DesktopEntry.h"
#include "IconThread.h"
#include "AppConfigFile.h"
#include "Localized.h"
#include "AppMenuPopupEditor.h"

/**
 * @file AppMenuItem.h
 *
 * @brief AppMenuItem handles the menu data for an AppMenuButton.  
 * 
 * It provides all custom data displayed by the button, including data needed 
 * for handling button clicks and editing this data and its button. 
 * 
 *  Methods that edit this menu data are only accessible to AppMenuButton 
 * objects, as each menu item should only be directly modified by its 
 * AppMenuButton.  All methods for getting menu data return false, {}, or
 * String() by default. Inheriting classes are responsible for overriding
 * these to provide accurate data.
 */

class AppMenuItem : public juce::ReferenceCountedObject,
public AppConfigFile::Listener
{
public:
    friend class AppMenuButton;
    typedef juce::ReferenceCountedObjectPtr<AppMenuItem> Ptr;

    AppMenuItem();

    virtual ~AppMenuItem() { }

    /**
     * @return true if this menu item is an application folder.
     */
    virtual bool isFolder() const;

    /**
     * @return all menu items in this folder, or an empty array if this isn't
     *          a folder.
     */
    virtual juce::Array<AppMenuItem::Ptr> getFolderItems() const;

    /**
     * @return the display name of the associated application.
     */
    virtual juce::String getAppName() const;

    /**
     * @return the application shell command or directory path.
     */
    virtual juce::String getCommand() const;

    /**
     * @return true iff this menu item is an application that launches in
     *          the terminal. 
     */
    virtual bool isTerminalApp() const;

    /**
     * @return true iff changing this menu item makes changes to .desktop or
     *          .directory files.
     */
    virtual bool changesDesktopEntries() const;

    /**
     * @return all application categories linked to this menu item.
     */
    virtual juce::StringArray getCategories() const;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual juce::String getIconName() const;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * 
     * @param offset  The value to add to the menu item index.
     * 
     * @return true if this menu item has an index value i that can be changed 
     *          to i+offset. 
     */
    virtual bool canChangeIndex(int offset) const;

    /**
     * @param toCompare
     * 
     * @return true iff this menu item and another share the same properties
     */
    bool operator==(const AppMenuItem& toCompare) const;

    /**
     * FactoryInterface is used by AppMenuItems to create more AppMenuItems.
     * Any folder menu item will have a FactoryInterface it can use to create
     * more menu items.
     */
    class FactoryInterface
    {
        friend class AppMenuItem;

    public:

        /**
         * FactoryInterface doesn't remove itself from AppMenuItems when it is
         * destroyed, as the factory interface object should never be destroyed
         * while any menu items still exist.
         */
        virtual ~FactoryInterface() { }

        /**
         * Get an AppMenuItem for an application link provided by the 
         * AppConfigFile
         * 
         * @param appItem  Defines all menu item data.
         * 
         * @return a pointer to the new menu item
         */
        virtual AppMenuItem::Ptr create
        (const AppShortcut& appItem) = 0;

        /**
         * Get an AppMenuItem for an application link that was read from a 
         * desktop entry file.
         * 
         * @param desktopEntry  Defines the application/directory data.
         * 
         * @return a pointer to the new menu item.
         */
        virtual AppMenuItem::Ptr create(const DesktopEntry& desktopEntry) = 0;

        /**
         * Get an AppMenuItem for an application folder provided by the 
         * AppConfigFile. 
         * 
         * @param appFolder  Defines all menu item data.
         * 
         * @return a pointer to the new menu item/
         */
        virtual AppMenuItem::Ptr create
        (const AppFolder& appFolder) = 0;

    protected:

        FactoryInterface() { }

        /**
         * Assigns this FactoryInterface to a folder menu item.  This does 
         * nothing if the menu item is not a folder.
         * 
         * @param menuItem  This should be a folder menu item created by the 
         *                   factory interface.
         * 
         * @return the same menu item. 
         */
        AppMenuItem::Ptr setFactory(AppMenuItem::Ptr menuItem);

    };
protected:

    /**
     * Get an appropriate title to use for a deletion confirmation window.
     */
    virtual juce::String getConfirmDeleteTitle() const;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual juce::String getConfirmDeleteMessage() const;

    /**
     * @return true iff this menu item has categories that can be edited,
     *          defaults to false.
     */
    virtual bool hasEditableCategories() const;

    /**
     * @return true iff this menu item has a command that can be edited,
     *          defaults to false.
     */
    virtual bool hasEditableCommand() const;

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual juce::String getEditorTitle() const;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    virtual std::function<void(AppMenuPopupEditor*) > getEditorCallback();

    /**
     * Removes the source of this menu item's data
     * 
     * @return true iff the source was removed.
     */
    virtual bool removeMenuItemSource();

    /**
     * If possible, change the index of this menu item by some
     * offset amount.
     * 
     * @param offset  This will be added to the menu item's current index, if
     *                 possible.
     * 
     * @return true iff the operation succeeded.
     */
    virtual bool moveDataIndex(int offset);

    /**
     * Gets the string to add before a launch command to make it launch in the
     * terminal.
     */
    juce::String getTermLaunchPrefix() const;
    
    
    /**
     * Get an AppMenuItem for an application link provided by the 
     * AppConfigFile.
     * 
     * @param appItem  Defines all menu item data.
     * 
     * @return a pointer to the new menu item, or nullptr if the
     *          factoryInterface isn't set.
     */
    AppMenuItem::Ptr create(const AppShortcut& appItem) const;

    /**
     * Get an AppMenuItem for an application link that was read from a 
     * desktop entry file.
     * 
     * @param desktopEntry  Defines the application/directory data.
     * 
     * @return a pointer to the new menu item, or nullptr if the
     *          factoryInterface isn't set.
     */
    AppMenuItem::Ptr create(const DesktopEntry& desktopEntry) const;

    /**
     * Get an AppMenuItem for an application folder provided by the 
     * AppConfigFile. 
     * 
     * @param appFolder  Defines all menu item data.
     * 
     * @return a pointer to the new menu item, or nullptr if the
     *          factoryInterface isn't set.
     */
    AppMenuItem::Ptr create(const AppFolder& appFolder) const;

private:

    /**
     * Updates the termLaunchPrefix if it's changed in configuration.
     * 
     * @param propertyKey
     */
    virtual void configValueChanged(const juce::Identifier& propertyKey);



    //folders will have a factory interface here to create new menu items.
    FactoryInterface* factoryInterface = nullptr;

    //prefix to add before commands that launch in a terminal window.
    juce::String termLaunchPrefix;

    /**
     * Using a private Localized member is necessary to allow AppMenuItem
     * subclasses to inherit Localized. 
     */
    class LocaleText : public Localized{
        public:
            LocaleText() : Localized("AppMenuItem") {}
            virtual ~LocaleText() {}
    };
    LocaleText txt;
    //localized text keys;
    static const constexpr char * delete_APP = "delete_APP";
    static const constexpr char * question_mark = "question_mark";
    static const constexpr char * really_delete = "really_delete";
    static const constexpr char * edit_menu_item = "edit_menu_item";

};
