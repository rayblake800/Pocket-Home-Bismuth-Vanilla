/*
  ==============================================================================

    FolderEditorPopup.cpp
    Created: 12 Jan 2018 9:54:23pm
    Author:  anthony

  ==============================================================================
 */

#include "FolderEditorPopup.h"

FolderEditorPopup::FolderEditorPopup(AppFolderButton* folderButton,
        AppConfigFile& config, AppConfigFile::AppFolder folder,
        IconThread& iconThread) :
PopupEditorComponent("Edit folder", iconThread, true, false, true),
folderButton(folderButton),
config(config),
folder(folder)
{
    setName(folder.name);
    setIcon(folder.icon);
    setCategories(folder.categories);
    resized();
}

FolderEditorPopup::~FolderEditorPopup()
{
}

void FolderEditorPopup::deleteBtnPressed()
{
    folderButton->deleteFolder();
}

void FolderEditorPopup::confirm()
{
    folderButton->editFolder(getNameField(), getIconField(), getCategories());
    closePopup();
}
