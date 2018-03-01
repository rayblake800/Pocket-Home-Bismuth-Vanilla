/* 
 * @file   NewDesktopAppEditor.h
 * 
 * NewConfigAppEditor creates a pop-up editor component that allows the user
 * to create a new application link as a local .desktop file.  After entering 
 * data and pressing the confirm button, the new favorite application link is 
 * added to the menu, and the .desktop file is written.
 *
*/
#pragma once
#include "../DesktopEntry.h"
#include "AppMenuPopupEditor.h"

class NewDesktopAppEditor : public AppMenuPopupEditor {
public:
    /**
     * 
     * @param iconThread
     * @param onConfirm
     */
    NewDesktopAppEditor(IconThread& iconThread,
            std::function<void() > onConfirm = []() {
            });
};