/**
 * @file AppFolderButton.h
 * @author Anthony Brown
 * 
 * AppFolderButton is an AppMenuButton that gets its data from a 
 * AppConfigFile::AppFolder struct.
 */
#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "../IconThread.h"
#include "AppMenuButton.h"

class AppFolderButton : public AppMenuButton {
public:
    /**
     * Create a new button representing an AppFolder
     * @param config the config file defining this folder
     * @param appFolder defines the application data
     * @param index button position in its column
     * @param column button's column in the AppMenu
     * @param a reference to the thread that loads button icons
     */
    AppFolderButton(AppConfigFile& config,
            AppConfigFile::AppFolder appFolder, 
            int index, int column,IconThread& iconThread);

    /**
     * Check if this button is for an application folder
     * @return true
     */
    bool isFolder() const;

    /**
     * @return the display name of the associated folder
     */
    String getAppName() const;

    /**
     * @return the empty string, as AppFolderButtons don't need a
     * particular command
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
private:
    AppConfigFile& config;
    AppConfigFile::AppFolder appFolder;
};

