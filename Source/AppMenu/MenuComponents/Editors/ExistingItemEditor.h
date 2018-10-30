/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "PopupEditor.h"
#include "MenuItem.h"

/**
 * @file  ExistingItemEditor.h
 *
 * @brief  A component that allows the user to edit an AppMenuItem.
 */
class AppMenu::ExistingItemEditor : public PopupEditor
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
    ExistingItemEditor(const MenuItem editedItem,
            std::function<void()> onConfirm = [](){});

    virtual ~ExistingItemEditor() { }

protected:
    /**
     * @brief  Saves all editor field values back to the edited menu item.
     */
    virtual void commitEdits() override;

private:
    /*  The menu item being edited */
    MenuItem editedItem;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
