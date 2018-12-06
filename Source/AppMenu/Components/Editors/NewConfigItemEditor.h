#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/** 
 * @file  AppMenu/Components/Editors/NewConfigItemEditor.h
 * 
 * @brief  Creates a pop-up editor component that allows the user to add a new 
 *         item to the AppMenu, saved in the AppMenu JSON configuration file.
 */ 

#include "AppMenu/Components/Editors/PopupEditor.h"
#include "AppMenu/Data/MenuItem.h"

namespace AppMenu{ class NewConfigItemEditor; }

/**
 *  On construction, the editor is set to create either an application shortcut,
 * or a menu folder. After entering data and pressing the confirm button, the 
 * new menu item is added to the menu, and saved by AppMenu::ConfigFile.
 */
class AppMenu::NewConfigItemEditor : public PopupEditor 
{
public:
    /**
     * @brief  Creates the new editor component.
     *
     * @param parentFolder  The folder menu item the new component will be
     *                      inserted into.
     *
     * @param isFolder      If true, the editor creates a folder item, if false,
     *                      it creates an application shortcut item.
     *
     * @param insertIndex   An optional index value where the new item will be
     *                      inserted, By default it will be added to the start
     *                      of the folder.
     *
     * @param onConfirm     An optional callback routine to run after creating 
     *                      the new application entry.
     */
    NewConfigItemEditor(MenuItem parentFolder,
            const bool isFolder,
            const int insertIndex = 0,
            const std::function<void() > onConfirm = [](){});

protected:
    /**
     * @brief  Creates the new application menu item and inserts it into the
     *         parent folder item provided on editor construction.
     */
    virtual void commitEdits();

private:
    /* Folder where the new menu item will be inserted. */
    MenuItem parentFolder;

    /* Index in the folder where the new menu item will be inserted. */
    int insertIndex;
};
