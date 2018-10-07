#include "NewConfigAppEditor.h"

NewConfigAppEditor::NewConfigAppEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New favorite application link",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    DBG("NewConfigAppEditor: editor not yet re-implemented!");
    onConfirm();
}
, false, true)
{

#    if JUCE_DEBUG
    setName("NewConfigAppEditor");
#    endif
}
