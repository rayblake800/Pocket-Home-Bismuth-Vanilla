#include "ExistingItemEditor.h"

/*
 * Creates an editor for editing a specific menu item.
 */
AppMenu::ExistingItemEditor::ExistingItemEditor
(const MenuItem& editedItem, std::function<void()> onConfirm) :
    editedItem(editedItem),
    AppMenuPopupEditor(editedItem.getEditorTitle(), onConfirm,
            isEditable(editedItem, DataField::categories),
            isEditable(editedItem, DataField::command)) 
{
    setTitleField(editedItem.getTitle());
    setIconField(editedItem.getIconName());
    if(isEditable(editedItem, DataField::categories))
    {
        setCategoryList(editedItem.getCategories());
    }
    if(isEditable(editedItem, DataField::command))
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
    setTitle(editedItem, getTitleField());
    setIconName(editedItem, getIconNameField());
    if(isEditable(editedItem, DataField::categories))
    {
        setCategories(editedItem, getCategories());
    }
    if(isEditable(editedItem, DataField::command))
    {
        setCommand(editedItem, getCommandField());
        setLaunchedInTerm(editedItem, getTerminalCheckboxState());
    }
    saveChanges(editedItem);
}
