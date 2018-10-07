#include "NewFolderEditor.h"
#include "ConfigItemData.h"

NewFolderEditor::NewFolderEditor(std::function<void() > onConfirm) :
AppMenuPopupEditor("New folder",
[this, onConfirm](AppMenuPopupEditor* editor)
{
    DBG("NewFolderEditor.h: Editor not yet re-implemented!");
    onConfirm();
},
true, false)
{

#    if JUCE_DEBUG
    setName("NewFolderEditor");
#    endif
}
