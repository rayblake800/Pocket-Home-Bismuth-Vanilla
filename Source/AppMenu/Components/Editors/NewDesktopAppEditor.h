/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "AppMenu/Components/Editors/PopupEditor.h"

/** 
 * @file  NewDesktopAppEditor.h
 * 
 * @brief  A pop-up editor component that allows the user to create a new 
 *         application link as a local .desktop file.  
 * 
 * After entering data and pressing the confirm button, the new favorite 
 * application link is added to the menu, and the .desktop file is written.  
 * These files will be saved to the current user folder under 
 * .local/share/applications.  This allows the user to create menu items that
 * are visible to other applications that read desktop entry files.
 *
*/
class AppMenu::NewDesktopAppEditor : public PopupEditor {
public:
    /**
     * @brief  Creates a new editor, setting any additional actions for the
     *         editor to perform on confirmation.
     *
     * @param onConfirm   A callback function to run after the user presses the
     *                     confirm button.
     */
    NewDesktopAppEditor(std::function<void() > onConfirm = [](){});

private:
    /**
     * @brief  Creates a new desktop entry file using the editor's field 
     *         values.
     */
    virtual void commitEdits() override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
