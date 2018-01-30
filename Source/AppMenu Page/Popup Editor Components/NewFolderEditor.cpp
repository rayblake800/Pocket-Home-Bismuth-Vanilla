#include "NewFolderEditor.h"

NewFolderEditor::NewFolderEditor(AppConfigFile& config,
        IconThread& iconThread, std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder", iconThread,
[this, &config, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile::AppFolder newFolder;
    newFolder.name = editor->getNameField();
    newFolder.icon = editor->getIconField();
    newFolder.categories = editor->getCategories();
    config.addAppFolder(newFolder, config.getFolders().size());
    onConfirm();
},
[]()
{
}

, false, true, false)
{
}