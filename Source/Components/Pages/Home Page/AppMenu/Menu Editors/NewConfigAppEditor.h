/* 
 * @file   NewConfigAppEditor.h
 * 
 * NewConfigAppEditor creates a pop-up editor component that allows the user
 * to add a new application link to the list of favorite applications managed
 * by AppConfigFile.  After entering data and pressing the confirm button,
 * the new favorite application link is added to the menu and saved by
 * AppConfigFile.
 *
*/
#pragma once
#include "AppConfigFile.h"
#include "AppMenuPopupEditor.h"

class NewConfigAppEditor : public AppMenuPopupEditor {
public:
    /**
     * @onConfirm               An optional callback routine to run after 
     *                           creating the new application entry.
     */
    NewConfigAppEditor(std::function<void() > onConfirm = [](){});
};