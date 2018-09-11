#include "NewFolderEditor.h"

NewFolderEditor::NewFolderEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile appConfig;
    AppFolder newFolder(
            editor->getNameField(),
            editor->getCategories(),
            editor->getIconField());
    appConfig.addFolder(newFolder, appConfig.getFolders().size());
    onConfirm();
},
true, false)
{

#    if JUCE_DEBUG
    setName("NewFolderEditor");
#    endif
}
