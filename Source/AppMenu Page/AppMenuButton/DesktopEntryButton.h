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
private:
    DesktopEntry desktopEntry;
};
