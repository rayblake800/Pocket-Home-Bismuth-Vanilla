/**
 * @file DesktopEntryButton.h
 * @author Anthony Brown
 * 
 * DesktopEntryButton is an AppMenuButton that gets its data from a 
 * DesktopEntry object, loaded from a .desktop file. It represents a shortcut
 * to an installed application. The object creating/managing this button is 
 * expected to launch that application using the stored command value when this 
 * button is selected and clicked.
 * @see AppMenuButton, AppMenuComponent, DesktopEntry
 */
#pragma once
#include "../DesktopEntry.h"
#include "../IconThread.h"
#include "AppMenuButton.h"

class DesktopEntryButton : public AppMenuButton {
public:
    typedef ReferenceCountedObjectPtr<DesktopEntryButton> Ptr;
    /**
     * Create a new button representing a DesktopEntry
     * @param desktopEntry defines the application/directory data
     * @param index button position in its column
     * @param column button's column in the AppMenu
     * @param a reference to the thread that loads button icons
     */
    DesktopEntryButton(DesktopEntry desktopEntry, int index, int column,
            IconThread& iconThread);

    /**
     * @return true if this button is for an application folder
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
     * @return all application categories linked to this button.
     */
    Array<String> getCategories() const;
        
    /**
     * @return the name or path used to load the icon file. 
     */
    String getIconName() const;
    
    /**
     * Gets a PopupEditorComponent configured to edit this button
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    AppMenuPopupEditor* getEditor();

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

    /**
     * Sets this button's desktopEntry to not display in pocket-home for the
     * current user, and removes the button from its parent component.
     */
    void removeButtonSource();

    /**
     * Return true if this button's data source has an index that can be
     * moved by a given amount.
     * @param offset some value to add to the button index
     * @return false, as DesktopEntryButtons don't have a variable order.
     */
    bool canChangeIndex(int offset) {
        return false;
    };

    /**
     * If possible, change the index of this button's data source by some
     * offset amount. This does nothing, because DesktopEntryButtons don't have
     * changing indices.
     */
    void moveDataIndex(int offset) {
    };
private:
    DesktopEntry desktopEntry;
};
