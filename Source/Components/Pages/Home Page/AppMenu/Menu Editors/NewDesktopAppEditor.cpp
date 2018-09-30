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
        DesktopEntry newEntry(entryName, entryName,
                DesktopEntry::Type::application);
        newEntry.setIcon(editor->getIconField());
        newEntry.setExec(editor->getCommandField());
        newEntry.setCategories(editor->getCategories());
        newEntry.setLaunchedInTerm(editor->launchInTerm());
        newEntry.writeFile();
        onConfirm();
    }
},
true, true)
{
#    if JUCE_DEBUG
    setName("NewDesktopAppEditor");
#    endif
}
