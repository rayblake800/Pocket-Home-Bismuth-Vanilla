/*
  ==============================================================================

    NewFolderEditor.h
    Created: 26 Jan 2018 5:53:53pm
    Author:  anthony

  ==============================================================================
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