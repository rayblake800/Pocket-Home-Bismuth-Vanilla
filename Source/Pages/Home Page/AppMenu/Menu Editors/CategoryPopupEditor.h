/**
 * @file CategoryPopupEditor.h
 * 
 * CategoryPopupEditor provides a user interface for editing a list of category
 * strings.
 */

#pragma once
#include "ListEditor.h"
#include "PopupEditorComponent.h"


class CategoryPopupEditor : public PopupEditorComponent{
public:
    /**
     * @param categories initial list of editable category strings
     * @param onConfirm runs when the user presses the confirm button. The
     * final list of edited categories will be passed to it as its only 
     * parameter.
     */
    CategoryPopupEditor(Array<String>categories,
            std::function<void(Array<String>) > onConfirm);
    virtual ~CategoryPopupEditor();

private:
    //Holds and allows updates to the list of categories 
    ListEditor categoryList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryPopupEditor)
};
