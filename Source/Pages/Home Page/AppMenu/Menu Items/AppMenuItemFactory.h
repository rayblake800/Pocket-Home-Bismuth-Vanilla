/*
 * @file AppMenuItemFactory
 * 
 * Given a data source, AppMenuItemFactory will initialize and return an
 * AppMenuItem of the appropriate derived class.
 */
#pragma once
#include "JuceHeader.h"
#include "DesktopEntries.h"
#include "AppConfigFile.h"
#include "MainConfigFile.h"
#include "AppMenuItem.h"

class AppMenuItemFactory : public AppMenuItem::FactoryInterface{
public:
    
    AppMenuItemFactory(AppConfigFile& appConfigFile,
            MainConfigFile& mainConfigFile,
            DesktopEntries& desktopEntries);
    
    virtual ~AppMenuItemFactory() {}
    
    /**
     * Create the folder menu item that defines the base menu folder
     * @return 
     */
    AppMenuItem::Ptr createBaseFolderItem();

    /**
     * Get an AppMenuItem for an application link provided by the AppConfigFile.
     * 
     * @param appItem  Defines all menu item data.
     * 
     * @return a pointer to the new menu item.
     */
    AppMenuItem::Ptr create(const AppConfigFile::AppItem& appItem) override;

    /**
     * Get an AppMenuItem for an application link that was read from a desktop
     * entry file/
     * 
     * @param desktopEntry   This defines the application/directory data.
     * 
     * @return  a pointer to the new menu item.
     */
    AppMenuItem::Ptr create(const DesktopEntry& desktopEntry) override;

    /**
     * Get an AppMenuItem for an application folder provided by the 
     * AppConfigFile.
     * 
     * @param appFolder  This defines all menu item data.
     * 
     * @return a pointer to the new menu item.
     */
    AppMenuItem::Ptr create(const AppConfigFile::AppFolder& appFolder) override;
private:
    AppConfigFile& appConfig;
    MainConfigFile& mainConfig;
    DesktopEntries& desktopEntries;
};