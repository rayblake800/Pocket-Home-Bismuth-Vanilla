/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "ListEditor.h"
#include "PopupEditorComponent.h"
#include "AppMenu.h"

/**
 * @file CategoryEditor.h
 * 
 * @brief  Provides a user interface for editing a list of category strings.
 */

class AppMenu::CategoryEditor : public PopupEditorComponent
{
public:
    /**
     * @brief  Creates a new category editor, setting the initial categories and
     *         the confirmation callback function.
     *
     * @param categories  An initial list of editable category strings to show.
     * 
     * @param onConfirm   A callback to run if the user presses the confirm 
     *                    button.  The final list of edited categories will be 
     *                    passed to it as its only parameter.
     */
    CategoryEditor(juce::StringArray categories, 
            std::function<void(juce::StringArray) > onConfirm);

    virtual ~CategoryEditor() { }

private:
    /* Holds and allows updates to the list of categories: */ 
    ListEditor categoryList;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryEditor)
};

/* Only include this file directly in the AppMenu implementation! */
#endif
