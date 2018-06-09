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
    setLayout({
        .xMarginFraction = 0.1,
        .yMarginFraction = 0.1,
        .rows = {
            {
                .rowWeight = 20,
                .yPaddingWeight = 2,
                .rowItems = {
                    {
                        .component = &categoryList,
                        .componentWeight = 10,
                        .xPaddingWeight = 2
                    }
                }
            }
        }
    });
}

