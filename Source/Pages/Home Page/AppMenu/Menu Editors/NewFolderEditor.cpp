#include "NewFolderEditor.h"

NewFolderEditor::NewFolderEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile appConfig;
    AppConfigFile::AppFolder newFolder;
    newFolder.name = editor->getNameField();
    newFolder.icon = editor->getIconField();
    newFolder.categories = editor->getCategories();
    appConfig.addAppFolder(newFolder, appConfig.getFolders().size());
    onConfirm();
},
true, false)
{

#    if JUCE_DEBUG
    setName("NewFolderEditor");
#    endif
}