#include "NewDesktopAppEditor.h"

NewDesktopAppEditor::NewDesktopAppEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New desktop application",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    using namespace juce;
    String entryName = editor->getNameField();
    entryName.removeCharacters("./\\");
    if (entryName.isNotEmpty())
    {
        DesktopEntry(entryName, editor->getIconField(),
                editor->getCommandField(), editor->getCategories(),
                editor->launchInTerm());
                onConfirm();
    }
},
true, true)
{
#    if JUCE_DEBUG
    setName("NewDesktopAppEditor");
#    endif
}