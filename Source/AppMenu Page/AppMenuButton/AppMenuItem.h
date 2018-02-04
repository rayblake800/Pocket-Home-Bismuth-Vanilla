/* 
 * @file AppMenuItem.h
 *
 *  AppMenuItem handles the menu data for an AppMenuButton.  It provides
 * all custom data displayed by the button, including data needed for
 * handling button clicks and editing this data and its button. Methods that
 * edit this menu data are only accessible to AppMenuButton objects, as each
 * menu item should only be directly modified by its AppMenuButton.
 */

#pragma once
#include "../../Configuration/Configurables/ConfigurableComponent.h"
#include "../IconThread.h"
#include "../Popup Editor Components/AppMenuPopupEditor.h"

class AppMenuItem {
public:
    friend class AppMenuButton;

    AppMenuItem();
    virtual ~AppMenuItem();

    /**
     * @return true if this menu item is an application folder
     */
    virtual bool isFolder() const = 0;

    /**
     * @return the display name of the associated application
     */
    virtual String getAppName() const = 0;

    /**
     * @return the application shell command or directory path.
     */
    virtual String getCommand() const = 0;

    /**
     * @return true iff this menu item is an application that launches in
     * the terminal.
     */
    virtual bool isTerminalApp() const = 0;

    /**
     * @return all application categories linked to this menu item.
     */
    virtual Array<String> getCategories() const = 0;

    /**
     * @return the name or path used to load the icon file. 
     */
    virtual String getIconName() const = 0;

    /**
     * Return true if this menu item has an index that can be moved by a given 
     * amount.
     * @param offset some value to add to the menu item index
     * @return true if this menu item has an index value i that can be changed 
     * to i+offset 
     */
    virtual bool canChangeIndex(int offset) const = 0;
protected:
    
    /**
     * Get an appropriate title to use for a deletion confirmation window.
     */
    virtual String getConfirmDeleteTitle() const;

    /**
     * Gets appropriate descriptive text for a deletion confirmation window.
     */
    virtual String getConfirmDeleteMessage() const;

    /**
     * @return true iff this menu item has categories that can be edited,
     * defaults to false.
     */
    virtual bool hasEditableCategories() const;

    /**
     * @return true iff this menu item has a command that can be edited,
     * defaults to false.
     */
    virtual bool hasEditableCommand() const;
    
    /**
     * @return the title to display over an editor for this menu item. 
     */
    virtual String getEditorTitle() const;

    /**
     * Gets a PopupEditorComponent callback function that will apply 
     * changes from an AppMenuPopupEditor to this menu item.
     */
    virtual std::function<void(AppMenuPopupEditor*) > getEditorCallback() = 0;

    /**
     * Removes the source of this menu item's data
     */
    virtual void removeMenuItemSource() = 0;

    /**
     * If possible, change the index of this menu item by some
     * offset amount.
     * @param offset will be added to the menu item's current index, if
     * possible.
     * @return true iff the operation succeeded.
     */
    virtual bool moveDataIndex(int offset) = 0;

};
