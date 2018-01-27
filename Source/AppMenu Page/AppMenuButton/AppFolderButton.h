/**
 * @file AppFolderButton.h
 * @author Anthony Brown
 * 
 * AppFolderButton is an AppMenuButton that gets its data from a 
 * AppConfigFile::AppFolder structure. These buttons represent a set of 
 * application categories.  The object creating/containing the AppFolderButton
 * is expected to display all applications in those categories whenever this
 * button is selected and clicked.
 * @see AppMenuComponent, AppMenuButton, AppConfigFile
 * 
 */
#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "../IconThread.h"
#include "AppMenuButton.h"

class AppFolderButton : public AppMenuButton {
public:
    typedef ReferenceCountedObjectPtr<AppFolderButton> Ptr;

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
            int index, int column, IconThread& iconThread);

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
     * @return the name or path used to load the icon file. 
     */
    String getIconName() const;

    /**
     * Gets a PopupEditorComponent configured to edit this button
     * @return a new PopupEditorComponent, ready to be added to the screen.
     */
    AppMenuPopupEditor* getEditor();

    /**
     * Assign new properties to this folder button, changing configuration files
     * @param name folder display name
     * @param icon folder icon image name/path
     * @param categories list of folder application categories
     */
    void editFolder(String name, String icon, Array<String> categories);

    /**
     * Remove this folder from config,and remove the button
     * from its parent component.
     */
    void removeButtonSource();
    
    /**
     * Return true if this button's data source has an index that can be
     * moved by a given amount.
     * @param offset some value to add to the button index
     * @return true if this button's data source has an index value i that can
     * be changed to i+offset 
     */
     bool canChangeIndex(int offset);
    
    /**
     * If possible, change the index of this button's data source by some
     * offset amount.
     * @param offset will be added to the button's current index, if possible.
     */
    void moveDataIndex(int offset);
private:
    AppConfigFile& config;
    AppConfigFile::AppFolder appFolder;
};

