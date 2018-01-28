/**
 * @file CategoryPopupEditor.h
 * 
 * CategoryPopupEditor provides a user interface for editing a list of category
 * strings.
 */

#pragma once
#include "../../Basic Components/ListEditor.h"
#include "PopupEditorComponent.h"


/*
 */
class CategoryPopupEditor : public PopupEditorComponent{
public:
    CategoryPopupEditor(Array<String>categories,
            std::function<void(Array<String>) > onConfirm);
    virtual ~CategoryPopupEditor();

private:
    ListEditor categoryList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryPopupEditor)
};
