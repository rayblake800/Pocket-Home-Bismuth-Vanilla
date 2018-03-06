/**
 * @file FolderMenuItem.h
 * 
 * FolderMenuItem is an AppMenuItem that gets its data from a 
 * AppConfigFile::AppFolder structure, representing a set of application 
 * categories.
 * @see AppMenuItem, AppConfigFile, AppMenuComponent
 * 
 */
#pragma once
#include "../../../../Configuration/AppConfigFile.h"
#include "../../IconThread.h"
#include "../../DesktopEntries.h"
#include "AppMenuItem.h"

class FolderMenuItem : public AppMenuItem {
public:
    /**
     * @param appFolder defines the folder data
     * @param desktopEntries is used to load folder items.
     */
    FolderMenuItem(const AppConfigFile::AppFolder& appFolder,
            DesktopEntries& desktopEntries);
    virtual ~FolderMenuItem();

    /**
     * Check if this button is for an application folder
     * @return true
     */
    bool isFolder() const override;

    /**
     * @return all menu items in this folder
     */
    virtual Array<AppMenuItem::Ptr> getFolderItems() const override;

    /**
     * @return the display name of the associated folder
     */
    virtual String getAppName() const override;

    /**
     * @return all application categories linked to this folder.
     */
    virtual Array<String> getCategories() const override;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual String getIconName() const override;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * @param offset some value to add to the menu item index
     * @return true if this menu item has an index value i that can be changed 
     * to i+offset 
     */
    virtual bool canChangeIndex(int offset) const override;
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
     * @return true, as folder categories can be edited.
     */
    virtual bool hasEditableCategories() const override {
        return true;
    };

    /**
     * @return false, as folders have no command.
     */
    virtual bool hasEditableCommand() const override {
        return false;
    };

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual String getEditorTitle() const override;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    std::function<void(AppMenuPopupEditor*) > getEditorCallback() override;

    /**
     * Removes the source of this menu item's data, deleting the folder from
     * apps.json
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

    /**
     * Assign new properties to this folder, changing configuration files
     * @param name folder display name
     * @param icon folder icon image name/path
     * @param categories list of folder application categories
     */
    void editFolder(String name, String icon, Array<String> categories);
private:
    //Source of this button's folder information
    AppConfigFile::AppFolder appFolder;
    DesktopEntries& desktopEntries;
};


