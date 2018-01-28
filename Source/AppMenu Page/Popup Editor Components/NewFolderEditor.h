/* 
 * @file   NewFolderEditor.h
 * 
 * NewFolderEditor creates a pop-up editor component that allows the user
 * to add a new folder to the list of application folders managed
 * by AppConfigFile.  After entering data and pressing the confirm button,
 * the new folder is added to the menu and saved by AppConfigFile.
 *
*/

#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "AppMenuPopupEditor.h"
class NewFolderEditor : public AppMenuPopupEditor {
public:
    /**
     * @param config manages the json file where the new folder will be added
     * @param iconThread will be used to load icon previews.
     * @onConfirm is an optional callback routine to run after creating the
     * new application entry
     */
    NewFolderEditor(AppConfigFile& config, IconThread& iconThread,
            std::function<void() > onConfirm = []() {
            });
};