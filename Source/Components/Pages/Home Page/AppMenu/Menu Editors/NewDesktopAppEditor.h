#pragma once
#include "DesktopEntry.h"
#include "AppMenuPopupEditor.h"

/** 
 * @file   NewDesktopAppEditor.h
 * 
 * @brief NewConfigAppEditor creates a pop-up editor component that allows the 
 * user to create a new application link as a local .desktop file.  
 * 
 * After entering data and pressing the confirm button, the new favorite 
 * application link is added to the menu, and the .desktop file is written.  
 * These files will be saved to the current user folder under 
 * .local/share/applications
 *
*/

class NewDesktopAppEditor : public AppMenuPopupEditor {
public:
    /**
     * @param onConfirm   A callback function to run after the user presses the
     *                     confirm button.
     */
    NewDesktopAppEditor(std::function<void() > onConfirm = [](){});
};