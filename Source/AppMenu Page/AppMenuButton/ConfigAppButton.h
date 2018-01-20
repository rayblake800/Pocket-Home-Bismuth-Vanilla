/**
 * @file ConfigAppButton.h
 * @author Anthony Brown
 * 
 * ConfigAppButton is an AppMenuButton that gets its data from a 
 * AppConfigFile::AppItem struct.
 */
#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "../IconThread.h"
#include "AppMenuButton.h"

class ConfigAppButton : public AppMenuButton {
public:
    typedef ReferenceCountedObjectPtr<ConfigAppButton> Ptr;

    /**
     * Create a new button representing an AppItem
     * @param config grants access to the config file defining this item.
     * @param appItem defines the application data
     * @param index button position in its column
     * @param column button's column in the AppMenu
     * @param a reference to the thread that loads button icons
     */
    ConfigAppButton(AppConfigFile& config,AppConfigFile::AppItem appItem,
            int index, int column, IconThread& iconThread);

    /**
     * Check if this button is for an application folder
     * @return false
     */
    bool isFolder() const;

    /**
     * @return the display name of the associated folder
     */
    String getAppName() const;

    /**
     * @return the application's launch command
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
    
    /**
     * Edit this button's application in the settings
     * @param name application display name
     * @param icon application icon
     * @param command application launch command
     * @param terminal sets whether this application launches in the terminal
     */
    void editApp(String name,String icon,String command,bool terminal);
    
    /**
     * Remove this button's application from config, and remove the button
     * from its parent component.
     */
    void deleteApp();
    
private:
    AppConfigFile& config;
    AppConfigFile::AppItem appItem;
};

