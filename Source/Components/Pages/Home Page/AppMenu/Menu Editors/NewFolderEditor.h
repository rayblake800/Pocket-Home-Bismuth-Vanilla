#pragma once
#include "AppConfigFile.h"
#include "AppMenuPopupEditor.h"

/** 
 * @file   NewFolderEditor.h
 * 
 * @brief NewFolderEditor creates a pop-up editor component that allows the
 *  user to add a new folder to the list of application folders managed
 * by AppConfigFile.  
 * 
 * After entering data and pressing the confirm button,
 * the new folder is added to the menu and saved by AppConfigFile.
 *
 */

class NewFolderEditor : public AppMenuPopupEditor
{
public:
    /**
     * @onConfirm                An optional callback routine to run after 
     *                            creating the new application entry.
     */
    NewFolderEditor(std::function<void() > onConfirm = [](){});
};