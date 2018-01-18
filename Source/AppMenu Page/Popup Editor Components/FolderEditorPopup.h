/*
  ==============================================================================

    FolderEditorPopup.h
    Created: 12 Jan 2018 9:54:23pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../../Basic Components/FileSelectTextEditor.h"
#include "../../Basic Components/ListEditor.h"
#include "../../Basic Components/DrawableImageComponent.h"
#include "../../Configuration/AppConfigFile.h"
#include "../AppMenuButton/AppFolderButton.h"
#include "PopupEditorComponent.h"

class FolderEditorPopup : public PopupEditorComponent {
public:
    FolderEditorPopup(AppFolderButton* folderButton,
            AppConfigFile& config, AppConfigFile::AppFolder folder);
    virtual ~FolderEditorPopup();
private:
    void confirm();
    AppMenuButton::Ptr folderButton;
    AppConfigFile& config;
    AppConfigFile::AppFolder folder;
    
    ScalingLabel nameLabel;
    TextEditor nameEditor;
    
    ScalingLabel iconLabel;
    DrawableImageComponent iconPreview;
    FileSelectTextEditor iconPathEditor;
    
    ScalingLabel categoryLabel;
    ListEditor categoryList;
    
    TextButton deleteButton;
};
