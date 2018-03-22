/* 
 * @file AppMenuItem.h
 *
 *  AppMenuItem handles the menu data for an AppMenuButton.  It provides
 * all custom data displayed by the button, including data needed for
 * handling button clicks and editing this data and its button. 
 * 
 *   Methods that edit this menu data are only accessible to AppMenuButton 
 * objects, as each menu item should only be directly modified by its 
 * AppMenuButton.  All methods for getting menu data return false, {}, or
 * String::empty by default. Inheriting classes are responsible for overriding
 * these to provide accurate data.
 */

#pragma once
#include "MainConfigFile.h"
#include "IconThread.h"
#include "AppMenuPopupEditor.h"

class AppMenuItem : public ReferenceCountedObject,
public ConfigFile::Listener
{
public:
    friend class AppMenuButton;
    typedef ReferenceCountedObjectPtr<AppMenuItem> Ptr;

    /**
     * @param mainConfig  Needed for loading the terminal launch command.
     */
    AppMenuItem(MainConfigFile& mainConfig);

    virtual ~AppMenuItem() { }

    /**
     * @return true if this menu item is an application folder.
     */
    virtual bool isFolder() const;

    /**
     * @return all menu items in this folder, or an empty array if this isn't
     * a folder.
     */
    virtual Array<AppMenuItem::Ptr> getFolderItems() const;

    /**
     * @return the display name of the associated application.
     */
    virtual String getAppName() const;

    /**
     * @return the application shell command or directory path.
     */
    virtual String getCommand() const;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal. 
     */
    virtual bool isTerminalApp() const;

    /**
     * @return true iff changing this menu item makes changes to .desktop or
     * .directory files.
     */
    virtual bool changesDesktopEntries() const;

    /**
     * @return all application categories linked to this menu item.
     */
    virtual StringArray getCategories() const;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual String getIconName() const;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * @param offset some value to add to the menu item index
     * @return true if this menu item has an index value i that can be changed 
     * to i+offset 
     */
    virtual bool canChangeIndex(int offset) const;

    /**
     * @param toCompare
     * @return true iff this menu item and another share the same
     * properties
     */
    bool operator==(const AppMenuItem& toCompare) const;
protected:

    /**
     * Get an appropriate title to use for a deletion confirmation window.
     */
    virtual String getConfirmDeleteTitle() const;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual String getConfirmDeleteMessage() const;

    /**
     * @return true iff this menu item has categories that can be edited,
     * defaults to false.
     */
    virtual bool hasEditableCategories() const;

    /**
     * @return true iff this menu item has a command that can be edited,
     * defaults to false.
     */
    virtual bool hasEditableCommand() const;

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual String getEditorTitle() const;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    virtual std::function<void(AppMenuPopupEditor*) > getEditorCallback();

    /**
     * Removes the source of this menu item's data
     * @return true iff the source was removed.
     */
    virtual bool removeMenuItemSource();

    /**
     * If possible, change the index of this menu item by some
     * offset amount.
     * @param offset will be added to the menu item's current index, if
     * possible.
     * @return true iff the operation succeeded.
     */
    virtual bool moveDataIndex(int offset);

    /**
     * Gets the string to add before a launch command to make it launch in the
     * terminal.
     */
    String getTermLaunchPrefix() const;

private:

    /**
     * Updates the termLaunchPrefix if it's changed in configuration.
     * 
     * @param config
     * @param propertyKey
     */
    void configValueChanged(ConfigFile* config, String propertyKey);

    String termLaunchPrefix;

};
