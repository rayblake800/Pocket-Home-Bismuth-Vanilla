#include "CategoryPopupEditor.h"

CategoryPopupEditor::CategoryPopupEditor(ComponentConfigFile& config,
        StringArray categories,
        std::function<void(StringArray) > onConfirm) :
PopupEditorComponent("Edit Categories",
config,
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

CategoryPopupEditor::~CategoryPopupEditor() { }

