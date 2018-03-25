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
#include "AppConfigFile.h"
#include "AppMenuPopupEditor.h"

class NewFolderEditor : public AppMenuPopupEditor
{
public:
    /**
     * @param appConfig          Needed to save the new folder to the apps.json
     *                            config file.
     * 
     * @param componentConfig    Component settings used to setup the editor
     *                            component.
     * 
     * @param iconThread         This will be used to load icon previews.
     * 
     * @onConfirm                An optional callback routine to run after 
     *                            creating the new application entry.
     */
    NewFolderEditor(
            AppConfigFile& appConfig,
            ComponentConfigFile& componentConfig,
            IconThread& iconThread,
            std::function<void() > onConfirm = [](){});
};