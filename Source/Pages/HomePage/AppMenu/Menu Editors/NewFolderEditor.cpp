#include "NewFolderEditor.h"

NewFolderEditor::NewFolderEditor(IconThread& iconThread,
        std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder", iconThread,
[this, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile config;
    AppConfigFile::AppFolder newFolder;
    newFolder.name = editor->getNameField();
    newFolder.icon = editor->getIconField();
    newFolder.categories = editor->getCategories();
    config.addAppFolder(newFolder, config.getFolders().size());
    onConfirm();
},
true, false)
{
}