#define APPMENU_IMPLEMENTATION_ONLY
#include "ExistingItemEditor.h"

/*
 * Creates an editor for editing a specific menu item.
 */
AppMenu::ExistingItemEditor::ExistingItemEditor
(const MenuItem editedItem, std::function<void()> onConfirm) :
    editedItem(editedItem),
    AppMenu::PopupEditor(editedItem.getEditorTitle(), onConfirm,
            editedItem.isEditable(MenuItem::DataField::categories),
            editedItem.isEditable(MenuItem::DataField::command)) 
{
    setTitleField(editedItem.getTitle());
    setIconField(editedItem.getIconName());
    if(editedItem.isEditable(MenuItem::DataField::categories))
    {
        setCategoryList(editedItem.getCategories());
    }
    if(editedItem.isEditable(MenuItem::DataField::command))
    {
        setCommandField(editedItem.getCommand());
        setTerminalCheckboxState(editedItem.getLaunchedInTerm());
    }
}

/*
 * Saves all editor field values back to the edited menu item.
 */
void AppMenu::ExistingItemEditor::commitEdits()
{
    editedItem.setTitle(getTitleField());
    editedItem.setIconName(getIconNameField());
    if(editedItem.isEditable(MenuItem::DataField::categories))
    {
        editedItem.setCategories(getCategories());
    }
    if(editedItem.isEditable(MenuItem::DataField::command))
    {
        editedItem.setCommand(getCommandField());
        editedItem.setLaunchedInTerm(getTerminalCheckboxState());
    }
    editedItem.saveChanges();
}
