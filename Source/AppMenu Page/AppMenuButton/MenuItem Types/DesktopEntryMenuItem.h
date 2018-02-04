/**
 * @file DesktopEntryMenuItem.h
 * 
 * DesktopEntryMenuItem is an AppMenuItem that gets its data from a 
 * DesktopEntry object, loaded from a .desktop file. It represents a shortcut
 * to an installed application.
 * @see AppMenuItem, AppMenuComponent, DesktopEntry
 */
#pragma once
#include "../../DesktopEntry.h"
#include "../../IconThread.h"
#include "../AppMenuItem.h"

class DesktopEntryMenuItem : public AppMenuItem {
public:
    /**
     * Create a new menu item representing a DesktopEntry
     * @param desktopEntry defines the application/directory data
     */
    DesktopEntryMenuItem(DesktopEntry desktopEntry);

    /**
     * @return true if this menu item is an application folder
     */
    bool isFolder() const;

    /**
     * @return the display name of the associated application
     */
    String getAppName() const;

    /**
     * @return application shell command or directory path.
     */
    String getCommand() const;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal.
     */
    bool isTerminalApp() const;

    /**
     * @return all application categories linked to this menu item.
     */
    Array<String> getCategories() const;

    /**
     * @return the name or path used to load the icon file. 
     */
    String getIconName() const;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * @param offset some value to add to the menu item index
     * @return false, as DesktopEntry objects don't have a variable order.
     */
    bool canChangeIndex(int offset) const{
        return false;
    };
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
     * @return true iff this menu item has categories that can be edited.
     */
    virtual bool hasEditableCategories() const override {
        return true;
    };

    /**
     * @return true iff this menu item has a command that can be edited.
     */
    virtual bool hasEditableCommand() const override {
        return true;
    };

    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual String getEditorTitle() const override;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    std::function<void(AppMenuPopupEditor*) > getEditorCallback();

    /**
     * Removes the source of this menu item's data.  This will hide the desktop
     * entry from pocket-home, but leave it otherwise unchanged and accessible
     * to other programs.
     */
    void removeMenuItemSource();

    /**
     * If possible, change the index of this button's data source by some
     * offset amount. This only returns false, because DesktopEntryButtons 
     * don't have changing indices.
     */
    bool moveDataIndex(int offset) {
        return false;
    };

    /**
     * Update this button's desktopEntry. This writes to 
     * ~/.local/share/applications, so changes will only affect the current user.
     * @param name application display name
     * @param icon application icon
     * @param categories application categories
     * @param command application launch command
     * @param useTerminal sets if this launches in a terminal window
     */
    void editEntry(String name, String icon, Array<String> categories,
            String command, bool useTerminal);

private:
    //application data source, set on construction
    DesktopEntry desktopEntry;
};
