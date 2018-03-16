#include "CategoryPopupEditor.h"

CategoryPopupEditor::CategoryPopupEditor(Array<String> categories,
        std::function<void(Array<String>) > onConfirm) :
PopupEditorComponent("Edit Categories",
[this, onConfirm](PopupEditorComponent* thisPopup)
{
    onConfirm(categoryList.getListItems());
}),
categoryList(categories)
{
    
#if JUCE_DEBUG
    setName("categoryEditor");
#endif
    RelativeLayoutManager::Layout layout = {
        {4,
            {
                {&categoryList, 1}
            }},
    };
    setLayout(layout);
}

CategoryPopupEditor::~CategoryPopupEditor() { }

