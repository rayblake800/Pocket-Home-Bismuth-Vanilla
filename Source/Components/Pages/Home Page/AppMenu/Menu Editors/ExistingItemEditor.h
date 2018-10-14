#pragma once
#include "AppMenuPopupEditor.h"
#include "AppMenuItem.h"

/**
 * @file  ExistingItemEditor.h
 *
 * @brief  A component that allows the user to edit an AppMenuItem.
 */
class ExistingItemEditor : public AppMenuPopupEditor, 
    private AppMenuItem::Editor
{
public:
    /**
     * @brief  Creates an editor for editing a specific menu item.
     *
     * @param editedItem  The menu item being edited.
     *
     * @param onConfirm   An optional callback function to run after
     *                    the user confirms changes.
     */
    ExistingItemEditor(const AppMenuItem& editedItem,
            std::function<void()> onConfirm = [](){});

    virtual ~ExistingItemEditor() { }

protected:
    /**
     * @brief  Saves all editor field values back to the edited menu item.
     */
    virtual void commitEdits() override;

private:
    /*  The menu item being edited */
    AppMenuItem editedItem;
};
