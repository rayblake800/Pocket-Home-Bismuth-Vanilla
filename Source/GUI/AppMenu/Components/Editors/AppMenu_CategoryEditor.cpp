#define APPMENU_IMPLEMENTATION
#include "AppMenu_CategoryEditor.h"

// Localized object class key:
static const juce::Identifier localeClassKey = "AppMenu::CategoryEditor";

// Localized text value key:
static const juce::Identifier editCategoryTitleKey = "editCategoryTitle";

// Creates a new category editor, setting the initial categories and the
// confirmation callback function.
AppMenu::CategoryEditor::CategoryEditor(juce::StringArray categories,
        std::function<void(juce::StringArray) > onConfirm) :
Locale::TextUser(localeClassKey),
Widgets::PopupEditor( [this, onConfirm] (Widgets::PopupEditor* thisPopup)
{
    onConfirm(categoryList.getListItems());
}),
categoryList(categories)
{
#if JUCE_DEBUG
    setName("AppMenu::CategoryEditor");
#endif
    setEditorTitle(localeText(editCategoryTitleKey));

    using namespace Layout::Group;
    RelativeLayout layout({
        Row(40,
        {
            RowItem(&categoryList, 10)
        })
    }
    );
    layout.setXMarginFraction(0.03);
    layout.setYMarginFraction(0.03);
    setLayout(layout);
}

