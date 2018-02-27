/**
 * @file ConfigAppMenuItem.h
 * 
 * ConfigAppMenuItem is an AppMenuItem that gets its data from a 
 * AppConfigFile::AppItem structure. It represents an application link that
 * was read from the apps.json config file.
 * @see AppMenuItem, AppMenuComponent, AppConfigFile
 */
#pragma once
#include "../../../../Configuration/AppConfigFile.h"
#include "../../IconThread.h"
#include "../AppMenuItem.h"

class ConfigAppMenuItem : public AppMenuItem {
public:
    /**
     * @param appItem defines all menu item data
     */
    ConfigAppMenuItem(const AppConfigFile::AppItem& appItem);
    virtual ~ConfigAppMenuItem();

    /**
     * @return the application's display name
     */
    String getAppName() const override;

    /**
     * @return the application's launch command
     */
    String getCommand() const override;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal.
     */
    bool isTerminalApp() const override;

    /**
     * @return the name or path used to load the icon file. 
     */
    String getIconName() const override;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * @param offset some value to add to the menu item index
     * @return true if this menu item has an index value i that can be changed 
     * to i+offset 
     */
    bool canChangeIndex(int offset) const override;

protected:
    /**
     * Get an appropriate title to use for a deletion confirmation window.
     */
    virtual String getConfirmDeleteTitle() const override;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual String getConfirmDeleteMessage() const override;

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual String getEditorTitle() const override;

    /**
     * returns true iff this menu item has categories that can be edited.
     * @return false, ConfigAppMenuItems don't have categories.
     */
    virtual bool hasEditableCategories() const override{
        return false;
    } ;

    /**
     * returns true iff this menu item has a command that can be edited.
     * @return true, all ConfigAppMenuItems have editable commands.
     */
    virtual bool hasEditableCommand() const override{
        return true;
    } ;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    std::function<void(AppMenuPopupEditor*) > getEditorCallback() override;

    /**
     * Edit this menu item's data in the config file.
     * @param name application display name
     * @param icon application icon
     * @param command application launch command
     * @param terminal sets whether this application launches in the terminal
     */
    void editApp(String name, String icon, String command, bool terminal);

    /**
     * Remove this menu item's application from the config file.
     * 
     * @return true iff the source was removed.
     */
    bool removeMenuItemSource() override;

    /**
     * If possible, change the index of this menu item by some
     * offset amount.
     * @param offset will be added to the menu item's current index, if
     * possible.
     * @return true iff the operation succeeded.
     */
    bool moveDataIndex(int offset) override;

private:
    //Application information structure
    AppConfigFile::AppItem appItem;
};


