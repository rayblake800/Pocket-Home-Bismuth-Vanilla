#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Components/Editors/CategoryEditor.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "AppMenu::CategoryEditor";

/* Localized text value key: */
static const juce::Identifier editCategoryTitleKey = "editCategoryTitle";
/*
 * Creates a new category editor, setting the initial categories and the 
 * confirmation callback function.
 */
AppMenu::CategoryEditor::CategoryEditor(juce::StringArray categories,
        std::function<void(juce::StringArray) > onConfirm) :
Locale::TextUser(localeClassKey),
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
    setEditorTitle(localeText(editCategoryTitleKey));
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

