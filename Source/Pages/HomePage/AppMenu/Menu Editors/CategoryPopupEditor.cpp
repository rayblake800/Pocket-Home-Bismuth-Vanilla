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
    setName("categoryEditor");
    loadAllConfigProperties();

    cancelButton.addListener(this);
    confirmButton.addListener(this);

    GridLayoutManager::Layout layout = {
        {1,
            {
                {&titleLabel, 1}
            }},
        {4,
            {
                {&categoryList, 1}
            }},
        {1,
            {
                {&cancelButton, 1},
                {&confirmButton, 1}
            }}
    };

    layoutManager.setLayout(layout, this);
    resized();
}

CategoryPopupEditor::~CategoryPopupEditor() { }

