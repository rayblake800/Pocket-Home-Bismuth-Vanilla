#pragma once
#include "DesktopEntry.h"
#include "Localized.h"
#include "IconThread.h"
#include "AppMenuItem.h"

/**
 * @file DesktopEntryMenuItem.h
 * 
 * @brief DesktopEntryMenuItem is an AppMenuItem that gets its data from a 
 * DesktopEntry object, loaded from a .desktop file. It represents a shortcut
 * to an installed application.
 * 
 * @see AppMenuItem, AppMenuComponent, DesktopEntry
 */

class DesktopEntryMenuItem : public AppMenuItem, private Localized
{
public:
    /**
     * Create a new menu item representing a DesktopEntry
     * @param desktopEntry defines the application/directory data
     */
    DesktopEntryMenuItem(const DesktopEntry& desktopEntry);

    virtual ~DesktopEntryMenuItem() { }

    /**
     * @return true if this menu item is an application folder
     */
    bool isFolder() const override;

    //TODO: load items from .Directory files!

    /**
     * @return the display name of the associated application
     */
    juce::String getAppName() const override;

    /**
     * @return application shell command or directory path.
     */
    juce::String getCommand() const override;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal.
     */
    bool isTerminalApp() const override;

    /**
     * @return all application categories linked to this menu item.
     */
    juce::StringArray getCategories() const override;

    /**
     * @return the name or path used to load the icon file. 
     */
    juce::String getIconName() const override;

    /**
     * @return true, changes to this menu item change the current user's
     * .Desktop files
     */
    bool changesDesktopEntries() const override;


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
     * @return true iff this menu item has categories that can be edited.
     */
    virtual bool hasEditableCategories() const override
    {
        return true;
    };

    /**
     * @return true iff this menu item has a command that can be edited.
     */
    virtual bool hasEditableCommand() const override
    {
        return true;
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
     * Removes the source of this menu item's data.  This will hide the desktop
     * entry from pocket-home, but leave it otherwise unchanged and accessible
     * to other programs.
     * 
     * @return true iff the source was removed.
     */
    bool removeMenuItemSource() override;

    /**
     * Update this button's desktopEntry. This writes to 
     * ~/.local/share/applications, so changes will only affect the current user.
     * @param name application display name
     * @param icon application icon
     * @param categories application categories
     * @param command application launch command
     * @param useTerminal sets if this launches in a terminal window
     */
    void editEntry(juce::String name, juce::String icon, juce::StringArray categories,
            juce::String command, bool useTerminal);

private:
    //localized text keys: 
    static const constexpr char * remove_link_to = "remove_link_to";
    static const constexpr char * question_mark = "question_mark";
    static const constexpr char * will_hide = "will_hide";
    static const constexpr char * edit_app = "edit_app";
    
    //application data source, set on construction
    DesktopEntry desktopEntry;
};
