#pragma once
#include "ListEditor.h"
#include "PopupEditorComponent.h"

/**
 * @file CategoryPopupEditor.h
 * 
 * @brief CategoryPopupEditor provides a user interface for editing a list of 
 * category strings.
 */

class CategoryPopupEditor : public PopupEditorComponent
{
public:
    /**
     * @param categories  An initial list of editable category strings to show.
     * 
     * @param onConfirm   This runs when the user presses the confirm button. 
     *                     The final list of edited categories will be passed 
     *                     to it as its only parameter.
     */
    CategoryPopupEditor
    (StringArray categories, std::function<void(StringArray) > onConfirm);

    virtual ~CategoryPopupEditor() { }

private:
    //Holds and allows updates to the list of categories 
    ListEditor categoryList;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryPopupEditor)
};
