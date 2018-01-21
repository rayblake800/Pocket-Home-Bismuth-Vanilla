/*
  ==============================================================================

    CategoryEditorPopup.cpp
    Created: 20 Jan 2018 1:48:01pm
    Author:  anthony

  ==============================================================================
 */

#include "CategoryPopupEditor.h"

//==============================================================================

CategoryPopupEditor::CategoryPopupEditor(Array<String> categories,
        std::function<void(Array<String>) > onConfirm) :
PopupEditorComponent("Edit Categories",
        [this,onConfirm](PopupEditorComponent* thisPopup){
            onConfirm(categoryList.getListItems());
        }),
categoryList(categories)
{
    setName("categoryEditor");
    loadAllConfigProperties();
    categoryList.setColours(bgColour, bgColour, selectionColour, textColour);

    cancelButton.replaceColour(Colours::black, textColour);
    cancelButton.addListener(this);

    confirmButton.replaceColour(Colours::black, textColour);
    confirmButton.addListener(this);

    layoutManager.addRow(1);
    layoutManager.addRow(4);
    layoutManager.addRow(1);

    std::vector<GridLayoutManager::ComponentLayoutParams> layout = {
        {&titleLabel, 0, 1},
        {&categoryList, 1, 1},
        {&cancelButton, 2, 1},
        {&confirmButton, 2, 1}
    };

    layoutManager.addComponents(layout, this);
    resized();
}

CategoryPopupEditor::~CategoryPopupEditor()
{
}

