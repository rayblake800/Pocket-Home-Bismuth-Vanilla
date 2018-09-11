#include "NewConfigAppEditor.h"

NewConfigAppEditor::NewConfigAppEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New favorite application link",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    AppConfigFile appConfig;
    AppShortcut newApp(
            editor->getNameField(),
            editor->getIconField(),
            editor->getCommandField(),
            editor->launchInTerm());
    appConfig.addShortcut(newApp, appConfig.getShortcuts().size());
    onConfirm();
}
, false, true)
{

#    if JUCE_DEBUG
    setName("NewConfigAppEditor");
#    endif
}
