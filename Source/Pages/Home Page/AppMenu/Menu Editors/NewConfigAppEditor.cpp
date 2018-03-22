#include "NewConfigAppEditor.h"

NewConfigAppEditor::NewConfigAppEditor(AppConfigFile& appConfig,
        ComponentConfigFile& componentConfig,
        IconThread& iconThread,
        std::function<void() > onConfirm) :
AppMenuPopupEditor("New favorite application link",
        componentConfig,
        iconThread,
[this, &appConfig, onConfirm](AppMenuPopupEditor* editor)
{

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