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
    /**
     * Create a new button representing an AppItem
     * @param appItem defines the application data
     * @param index button position in its column
     * @param column button's column in the AppMenu
     * @param a reference to the thread that loads button icons
     */
    ConfigAppButton(AppConfigFile::AppItem appItem, 
            int index, int column,IconThread& iconThread);

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
    PopupEditorComponent* getEditor() {return nullptr;};
private:
    AppConfigFile::AppItem appItem;
};

