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
    RelativeLayoutManager::Layout layout(
    {
        {
            .weight = 20,
            .rowItems = {
                {
                    RelativeLayoutManager::ComponentLayout(&categoryList, 10)
                }
            }
        }
    });
    layout.setXMarginFraction(0.1);
    layout.setYMarginFraction(0.1);
}

