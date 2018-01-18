/*
  ==============================================================================

    FolderEditorPopup.cpp
    Created: 12 Jan 2018 9:54:23pm
    Author:  anthony

  ==============================================================================
 */

#include "FolderEditorPopup.h"

FolderEditorPopup::FolderEditorPopup(AppFolderButton* folderButton,
        AppConfigFile& config, AppConfigFile::AppFolder folder) :
PopupEditorComponent("Edit folder"),
folderButton(folderButton),
config(config),
folder(folder),
nameLabel("nameLabel", "Folder name:"),
nameEditor(folder.name),
iconLabel("iconLabel", "Icon path:"),
iconPathEditor(folder.icon),
iconPreview(folderButton->getIcon()),
categoryLabel("categoryLabel", "Folder categories:"),
categoryList(folder.categories, Colours::white, Colours::blueviolet, Colours::black),
deleteButton("Delete folder")
{
    std::vector<GridLayoutManager::ComponentLayoutParams> layoutList{
        {&nameLabel, 1, 1},
        {&nameEditor, 1, 1},
        {&iconPreview, 2, 1},
        {&iconLabel, 2, 2},
        {&iconPathEditor, 2, 3},
        {&categoryLabel, 3, 1},
        {&categoryList, 4, 1},
        {&deleteButton, 5, 1}};

    layoutManager.addComponents(layoutList, this);
    layoutManager.setRowWeight(4, 5);
    addClosingButtons();
    layoutManager.layoutComponents(getLocalBounds(), 1, 1);
    
}

FolderEditorPopup::~FolderEditorPopup()
{
}

void FolderEditorPopup::confirm()
{
    closePopup();
}
