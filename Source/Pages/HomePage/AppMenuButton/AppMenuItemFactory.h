/*
 * @file AppMenuItemFactory
 * 
 * Given a data source, AppMenuItemFactory will initialize and return an
 * AppMenuItem of the appropriate derived class.
 */
#pragma once
#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "MenuItem Types/ConfigAppMenuItem.h"
#include "MenuItem Types/DesktopEntryMenuItem.h"
#include "MenuItem Types/FolderMenuItem.h"
#include "AppMenuItem.h"

class AppMenuItemFactory {
public:

    /**
     * Get an AppMenuItem for an application link provided by the AppConfigFile
     * @param appItem defines all menu item data
     * @param config is the object needed to save and load changes to appItem
     * @return a pointer to the new menu item
     */
    static AppMenuItem* create
    (AppConfigFile::AppItem appItem, AppConfigFile& config);

    /**
     * Get an AppMenuItem for an application link that was read from a desktop
     * entry file
     * @param desktopEntry defines the application/directory data
     * @return a pointer to the new menu item
     */
    static AppMenuItem* create(DesktopEntry desktopEntry);

    /**
     * Get an AppMenuItem for an application folder provided by the AppConfigFile
     * @param appFolder defines all menu item data
     * @param config is the object needed to save and load changes to appFolder
     * @return a pointer to the new menu item
     */
    static AppMenuItem* create
    (AppConfigFile::AppFolder appFolder, AppConfigFile& config);
private:
    AppMenuItemFactory();
    virtual ~AppMenuItemFactory();
};