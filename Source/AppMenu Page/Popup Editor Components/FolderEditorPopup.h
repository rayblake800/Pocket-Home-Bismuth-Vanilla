/*
  ==============================================================================

    FolderEditorPopup.h
    Created: 12 Jan 2018 9:54:23pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../../Basic Components/FileSelectTextEditor.h"
#include "../../Configuration/AppConfigFile.h"
#include "../AppMenuButton/AppFolderButton.h"
#include "PopupEditorComponent.h"

class FolderEditorPopup : public PopupEditorComponent {
public:
    FolderEditorPopup(AppFolderButton* folderButton,
            AppConfigFile::AppFolder folder);
    virtual ~FolderEditorPopup();
private:
    void confirm();
    AppMenuButton::Ptr folderButton;
    AppConfigFile::AppFolder folder;
    
    TextEditor nameEdit;
    
    FileSelectTextEditor iconPathEdit;
    DrawableImage iconPreview;
    TextButton deleteButton;
};
