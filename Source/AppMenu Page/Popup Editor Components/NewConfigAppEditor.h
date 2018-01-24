/*
  ==============================================================================

    NewConfigAppEditor.h
    Created: 23 Jan 2018 9:54:29pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "AppMenuPopupEditor.h"

class NewConfigAppEditor : public AppMenuPopupEditor {
public:
    /**
     * @param config manages the json file where new config apps will
     * be added.
     * @param iconThread will be used to load icon previews.
     * @onConfirm is an optional callback routine to run after creating the
     * new application entry
     * @folderIndex marks which folder will contain the new app. If this is
     * less than zero, the new app will not be in any folder.
     */
    NewConfigAppEditor(AppConfigFile& config, IconThread& iconThread,
            std::function<void() > onConfirm = []() {
            }, int folderIndex = -1);
};