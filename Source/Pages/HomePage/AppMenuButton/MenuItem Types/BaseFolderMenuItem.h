/**
 * @file BaseFolderMenuItem.h
 * 
 * BaseFolderMenuItem is an AppMenuItem that represents the base list
 * of favorite application shortcuts and folders defined in AppConfigFile.
 * 
 * @see AppMenuItem, AppConfigFile
 * 
 */
#pragma once
#include "../../../../Configuration/AppConfigFile.h"
#include "../../DesktopEntries.h"
#include "../AppMenuItem.h"

class BaseFolderMenuItem : public AppMenuItem {
public:
    /**
     * 
     * @param desktopEntries
     */
    BaseFolderMenuItem(DesktopEntries& desktopEntries);
    virtual ~BaseFolderMenuItem();

    /**
     * Check if this button is for an application folder
     * @return true
     */
    bool isFolder() const override;
    
    /**
     * @return all menu items in this folder
     */
    virtual Array<AppMenuItem::Ptr> getFolderItems() const override;
private:
    DesktopEntries& desktopEntries;
};


