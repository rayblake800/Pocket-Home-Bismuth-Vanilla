/*
 * @file AppMenuItemFactory
 * 
 * Given a data source, AppMenuItemFactory will initialize and return an
 * AppMenuItem of the appropriate derived class.
 */
#pragma once
#include "../../../../../JuceLibraryCode/JuceHeader.h"
#include "../../DesktopEntries.h"
#include "AppMenuItem.h"

class AppMenuItemFactory {
public:
    
    /**
     * Create the folder menu item that defines the base menu folder
     * @param desktopEntries
     * @return 
     */
    static AppMenuItem::Ptr createBaseFolderItem
    (DesktopEntries& desktopEntries);

    /**
     * Get an AppMenuItem for an application link provided by the AppConfigFile
     * @param appItem defines all menu item data
     * @return a pointer to the new menu item
     */
    static AppMenuItem::Ptr create(const AppConfigFile::AppItem& appItem);

    /**
     * Get an AppMenuItem for an application link that was read from a desktop
     * entry file
     * @param desktopEntry defines the application/directory data
     * @return a pointer to the new menu item
     */
    static AppMenuItem::Ptr create(const DesktopEntry& desktopEntry);

    /**
     * Get an AppMenuItem for an application folder provided by the AppConfigFile
     * @param appFolder defines all menu item data
     * @param desktopEntries
     * @return a pointer to the new menu item
     */
    static AppMenuItem::Ptr create(const AppConfigFile::AppFolder& appFolder,
    DesktopEntries& desktopEntries);
private:
    AppMenuItemFactory();
    virtual ~AppMenuItemFactory();
};