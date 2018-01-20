/*
  ==============================================================================

    AppFolderButton.cpp
    Created: 11 Jan 2018 6:58:52pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "AppFolderButton.h"
#include "../AppMenuComponent.h"
#include "../Popup Editor Components/FolderEditorPopup.h"

AppFolderButton::AppFolderButton(AppConfigFile& config,
        AppConfigFile::AppFolder appFolder,
        int index, int column, IconThread& iconThread) :
AppMenuButton(appFolder.name, index, column,iconThread),
config(config),
appFolder(appFolder)
{
    loadIcon(appFolder.icon);
}

/**
 * Check if this button is for an application folder
 * @return true
 */
bool AppFolderButton::isFolder() const
{
    return true;
}

/**
 * @return the display name of the associated folder
 */
String AppFolderButton::getAppName() const
{
    return appFolder.name;
}

/**
 * @return the empty string, as AppFolderButtons don't need a
 * particular command
 */
String AppFolderButton::getCommand() const
{
    return "";
}

/**
 * @return all application categories linked to this button.
 */
Array<String> AppFolderButton::getCategories() const
{
    return appFolder.categories;
}

/**
 * Gets a PopupEditorComponent configured to edit this button
 * @return a new PopupEditorComponent, ready to be added to the screen.
 */
PopupEditorComponent* AppFolderButton::getEditor()
{
    return new FolderEditorPopup(this, config, appFolder,iconThread);
}

/**
 * Assign new properties to this folder button, changing configuration files
 * @param name folder display name
 * @param icon folder icon image name/path
 * @param categories list of folder application categories
 */
void AppFolderButton::editFolder
(String name, String icon, Array<String> categories)
{
    if(icon != appFolder.icon)
    {
        loadIcon(icon);
    }
    appFolder.name=name;
    appFolder.icon=icon;
    appFolder.categories=categories;
    loadIcon(icon);
    config.addAppFolder(appFolder,appFolder.index);
    config.removeAppFolder(appFolder.index+1);
}

/**
 * Remove this folder from config, and reload buttons.
 */
void AppFolderButton::deleteFolder()
{
    config.removeAppFolder(appFolder.index);
    AppMenuComponent* appMenu = 
            dynamic_cast<AppMenuComponent*>(getParentComponent());
    if(appMenu != nullptr){
        appMenu->loadButtons();
    }
}