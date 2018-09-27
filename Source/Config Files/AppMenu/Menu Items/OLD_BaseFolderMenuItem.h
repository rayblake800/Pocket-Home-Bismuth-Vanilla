#pragma once
#include "AppMenuItemFactory.h"
#include "AppMenuItem.h"

/**
 * @file BaseFolderMenuItem.h
 * 
 * @brief BaseFolderMenuItem is an AppMenuItem that represents the base list
 * of favorite application shortcuts and folders defined in AppConfigFile.
 * 
 * @see AppMenuItem, AppConfigFile
 * 
 */

class BaseFolderMenuItem : public AppMenuItem
{
public:
    
    BaseFolderMenuItem() { }

    virtual ~BaseFolderMenuItem() { }

    /**
     * Check if this button is for an application folder
     * @return true
     */
    bool isFolder() const override;

    /**
     * @return all menu items in this folder
     */
    virtual juce::Array<AppMenuItem::Ptr> getFolderItems() const override;
};


