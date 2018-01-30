/**
 * @file ConfigAppButton.h
 * 
 * ConfigAppButton is an AppMenuButton that gets its data from a 
 * AppConfigFile::AppItem structure. It represents an application link
 * created from a configuration file.  The object creating/managing this 
 * button is expected to launch that application using the stored command
 * value when this button is selected and clicked.
 * @see AppMenuButton, AppMenuComponent, AppConfigFile
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
    ConfigAppButton(AppConfigFile& config, AppConfigFile::AppItem appItem,
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
     * This will be an empty list, as ConfigAppButtons are not associated with
     * application categories.
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
     * Edit this button's data source in the config file.
     * @param name application display name
     * @param icon application icon
     * @param command application launch command
     * @param terminal sets whether this application launches in the terminal
     */
    void editApp(String name, String icon, String command, bool terminal);

    /**
     * Remove this button's application from config, and remove the button
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
    //Application config file manager, used to read and write favorite app data
    AppConfigFile& config;
    //Application information structure
    AppConfigFile::AppItem appItem;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConfigAppButton)
};

