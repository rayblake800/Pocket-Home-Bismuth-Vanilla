#define APPMENU_IMPLEMENTATION_ONLY
#include "CategoryEditor.h"

AppMenu::CategoryEditor::CategoryEditor(
        juce::StringArray categories,
        std::function<void(juce::StringArray) > onConfirm) :
PopupEditorComponent( [this, onConfirm](PopupEditorComponent* thisPopup)
{
    onConfirm(categoryList.getListItems());
}),
categoryList(categories)
{

#    if JUCE_DEBUG
    setName("categoryEditor");
#    endif
    //TODO: localize this!
    setEditorTitle("Edit Categories");
    LayoutManager::Layout layout({
        LayoutManager::Row(40,
        {
            LayoutManager::RowItem(&categoryList, 10)
        })
    }
    );
    layout.setXMarginFraction(0.03);
    layout.setYMarginFraction(0.03);
    setLayout(layout);
}

