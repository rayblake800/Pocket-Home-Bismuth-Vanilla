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
nameEditor("nameEditor"),
iconLabel("iconLabel", "Icon path:"),
iconPathEditor("iconPathEditor"),
iconPreview(folderButton->getIcon()),
categoryLabel("categoryLabel", "Folder categories:"),
categoryList(folder.categories, bgColour, selectionColour, textColour),
deleteButton("Delete folder")
{
    nameLabel.setColour(Label::textColourId, textColour);
    nameEditor.setColour(TextEditor::textColourId, textColour);
    iconLabel.setColour(Label::textColourId, textColour);
    iconPathEditor.setColour(TextEditor::textColourId, textColour);
    categoryLabel.setColour(Label::textColourId, textColour);

    nameEditor.setText(folder.name, false);
    iconPathEditor.setText(folder.icon);
    std::vector<GridLayoutManager::ComponentLayoutParams> layoutList{
        {&nameLabel, 1, 1},
        {&nameEditor, 1, 1},
        {&iconPreview, 2, 1},
        {&iconLabel, 2, 2},
        {&iconPathEditor, 2, 3},
        {&categoryLabel, 3, 1},
        {&categoryList, 4, 1},
        {nullptr, 5, 1},
        {&deleteButton, 5, 2},
        {nullptr, 5, 1}};

    layoutManager.addComponents(layoutList, this);
    layoutManager.setRowWeight(4, 5);
    deleteButton.addListener(this);
    addClosingButtons();
    layoutManager.layoutComponents(getLocalBounds(), 2, 2);

}

FolderEditorPopup::~FolderEditorPopup()
{
}

void FolderEditorPopup::buttonClicked(Button* buttonClicked)
{
    if (buttonClicked == &deleteButton)
    {
        folderButton->deleteFolder();
        closePopup();
    } else
    {
        this->PopupEditorComponent::buttonClicked(buttonClicked);
    }
}

void FolderEditorPopup::confirm()
{
    folderButton->editFolder(nameEditor.getText(),iconPathEditor.getText(),
            categoryList.getListItems());
    closePopup();
}
