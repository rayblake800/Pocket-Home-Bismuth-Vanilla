#pragma once
#include "DesktopEntry.h"
#include "PopupEditor.h"
#include "AppMenu.h"

/** 
 * @file  NewDesktopAppEditor.h
 * 
 * @brief  A pop-up editor component that allows the user to create a new 
 *         application link as a local .desktop file.  
 * 
 * After entering data and pressing the confirm button, the new favorite 
 * application link is added to the menu, and the .desktop file is written.  
 * These files will be saved to the current user folder under 
 * .local/share/applications
 *
*/

class AppMenu::NewDesktopAppEditor : public PopupEditor {
public:
    /**
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
