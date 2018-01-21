/*
  ==============================================================================

    CategoryPopupEditor.h
    Created: 20 Jan 2018 1:48:01pm
    Author:  anthony

 * Note: lots of repeated code between this and PopupEditorComponent. 
 * Copy/Paste is quicker for now, but in the future have them inherit from a
 * common parent class.
  ==============================================================================
 */

#pragma once
#include "../../Basic Components/ListEditor.h"
#include "PopupEditorComponent.h"

//==============================================================================

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
