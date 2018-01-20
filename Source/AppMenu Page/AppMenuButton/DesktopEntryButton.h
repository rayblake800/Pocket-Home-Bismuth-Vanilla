/**
 * @file DesktopEntryButton.h
 * @author Anthony Brown
 * 
 * DesktopEntryButton is an AppMenuButton that gets its data from a 
 * DesktopEntry object.
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
     * Gets a PopupEditorComponent configured to edit this button
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    PopupEditorComponent* getEditor();
    
    /**
     * Update this button's desktopEntry. This writes to 
     * ~/.local/share/applications, so changes will only affect the current user.
     * @param name application display name
     * @param icon application icon
     * @param categories application categories
     * @param command application launch command
     * @param useTerminal sets if this launches in a terminal window
     */
    void editEntry(String name,String icon,Array<String> categories,
            String command,bool useTerminal);
    
    /**
     * Sets this button's desktopEntry to not display in pocket-home for the
     * current user, and reload all AppMenuButtons.
     */
    void hideEntry();
private:
    DesktopEntry desktopEntry;
};
