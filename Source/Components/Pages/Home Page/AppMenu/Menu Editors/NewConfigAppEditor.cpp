#include "NewConfigAppEditor.h"

NewConfigAppEditor::NewConfigAppEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New favorite application link",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile appConfig;
    AppConfigFile::AppItem newApp;
    newApp.name = editor->getNameField();
    newApp.icon = editor->getIconField();
    newApp.shell = editor->getCommandField();
    newApp.launchInTerminal = editor->launchInTerm();
    appConfig.addFavoriteApp(newApp, appConfig.getFavorites().size());
    onConfirm();
}
, false, true)
{

#    if JUCE_DEBUG
    setName("NewConfigAppEditor");
#    endif
}