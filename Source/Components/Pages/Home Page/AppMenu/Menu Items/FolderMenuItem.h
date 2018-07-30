#pragma once
#include "AppMenuItemFactory.h"
#include "IconThread.h"
#include "Localized.h"
#include "DesktopEntryLoader.h"
#include "AppMenuItem.h"

/**
 * @file FolderMenuItem.h
 * 
 * @brief FolderMenuItem is an AppMenuItem that gets its data from a 
 * AppConfigFile::AppFolder structure, representing a set of application 
 * categories.
 * 
 * @see AppMenuItem, AppConfigFile, AppMenuComponent
 */

class FolderMenuItem : public AppMenuItem, private Localized
{
public:
    /**
     */
    FolderMenuItem
    (const AppConfigFile::AppFolder& appFolder, DesktopEntryLoader& desktopEntries);

    virtual ~FolderMenuItem() { }

    /**
     * Check if this button is for an application folder
     * @return true
     */
    bool isFolder() const override;

    /**
     * @return all menu items in this folder
     */
    virtual juce::Array<AppMenuItem::Ptr> getFolderItems() const override;

    /**
     * @return the display name of the associated folder
     */
    virtual juce::String getAppName() const override;

    /**
     * @return all application categories linked to this folder.
     */
    virtual juce::StringArray getCategories() const override;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual juce::String getIconName() const override;

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
    virtual juce::String getConfirmDeleteTitle() const override;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual juce::String getConfirmDeleteMessage() const override;

    /**
     * @return true, as folder categories can be edited.
     */
    virtual bool hasEditableCategories() const override
    {
        return true;
    };

    /**
     * @return false, as folders have no command.
     */
    virtual bool hasEditableCommand() const override
    {
        return false;
    };

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual juce::String getEditorTitle() const override;

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
    void editFolder(juce::String name, juce::String icon, juce::StringArray categories);
private:
    
    AppConfigFile::AppFolder appFolder;
    DesktopEntryLoader& desktopEntries;

    //localized text keys: 
    static const constexpr char * delete_NAME = "delete_NAME";
    static const constexpr char * folder = "folder";
    static const constexpr char * will_remove_folder = "will_remove_folder";
    static const constexpr char * edit_folder = "edit_folder";
};


