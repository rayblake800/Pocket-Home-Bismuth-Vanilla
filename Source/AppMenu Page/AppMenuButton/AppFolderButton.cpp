/*
  ==============================================================================

    AppFolderButton.cpp
    Created: 11 Jan 2018 6:58:52pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "AppFolderButton.h"
#include "../Popup Editor Components/FolderEditorPopup.h"

AppFolderButton::AppFolderButton(AppConfigFile& config,
        AppConfigFile::AppFolder appFolder,
        int index, int column, IconThread& iconThread) :
AppMenuButton(appFolder.name, index, column),
config(config),
appFolder(appFolder)
{
    iconThread.loadIcon(this, appFolder.icon);
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
    return new FolderEditorPopup(this,config,appFolder);
}