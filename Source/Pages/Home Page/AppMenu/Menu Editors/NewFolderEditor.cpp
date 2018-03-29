#include "NewFolderEditor.h"

NewFolderEditor::NewFolderEditor(AppConfigFile& appConfig,
        ComponentConfigFile& componentConfig,
        std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder",
componentConfig,
[this, &appConfig, onConfirm](AppMenuPopupEditor* editor)
{

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