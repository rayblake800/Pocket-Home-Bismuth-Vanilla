/*
  ==============================================================================

    NewDesktopAppEditor.cpp
    Created: 23 Jan 2018 10:44:43pm
    Author:  anthony

  ==============================================================================
 */

#include "NewDesktopAppEditor.h"
NewDesktopAppEditor::NewDesktopAppEditor(IconThread& iconThread,
        std::function<void() > onConfirm) :
AppMenuPopupEditor("New desktop application", iconThread,
[this, onConfirm](AppMenuPopupEditor* editor)
{
    String entryName = editor->getNameField();
    entryName.removeCharacters("./\\");
    if (entryName.isNotEmpty())
    {
        DesktopEntry(entryName,editor->getIconField(),
                editor->getCommandField(),editor->getCategories(),
                editor->launchInTerm());
        onConfirm();
    }
},
[]()
{
}

, true, true, false)
{
}