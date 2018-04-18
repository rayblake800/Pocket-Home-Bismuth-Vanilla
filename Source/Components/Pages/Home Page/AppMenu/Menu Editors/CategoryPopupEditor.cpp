#include "CategoryPopupEditor.h"

CategoryPopupEditor::CategoryPopupEditor(
        StringArray categories,
        std::function<void(StringArray) > onConfirm) :
PopupEditorComponent("Edit Categories",
[this, onConfirm](PopupEditorComponent* thisPopup)
{
    onConfirm(categoryList.getListItems());
}),
categoryList(categories)
{

#    if JUCE_DEBUG
    setName("categoryEditor");
#    endif
    RelativeLayoutManager::Layout layout = {
        {4,
            {
                {&categoryList, 1}
            }},
    };
    setLayout(layout);
}

