#pragma once
#include "Localized.h"
#include "IconThread.h"
#include "AppMenuItem.h"

/**
 * @file ConfigAppMenuItem.h
 * 
 * @brief ConfigAppMenuItem is an AppMenuItem that gets its data from a 
 * AppConfigFile::AppItem structure. 
 * 
 * It represents an application link that
 * was read from the apps.json config file.
 * 
 * @see AppMenuItem, AppMenuComponent, AppConfigFile
 */

class ConfigAppMenuItem : public AppMenuItem, private Localized {
public:
    /**
     * @param appItem      Defines all menu item data.
     */
    ConfigAppMenuItem(const AppShortcut& appItem);
    
    virtual ~ConfigAppMenuItem() { }

    /**
     * @return the application's display name
     */
    juce::String getAppName() const override;

    /**
     * @return the application's launch command
     */
    juce::String getCommand() const override;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal.
     */
    bool isTerminalApp() const override;

    /**
     * @return the name or path used to load the icon file. 
     */
    juce::String getIconName() const override;

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
    virtual juce::String getConfirmDeleteTitle() const override;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual juce::String getConfirmDeleteMessage() const override;

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual juce::String getEditorTitle() const override;

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
    void editApp(juce::String name, juce::String icon, juce::String command, bool terminal);

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
    AppShortcut appItem;
    
    //localized text keys: 
    static const constexpr char * remove_APP = "remove_APP";
    static const constexpr char * from_favorites = "from_favorites";
    static const constexpr char * will_remove_link = "will_remove_link";
    static const constexpr char * edit_app = "edit_app";
};


