#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Data/JSON/MenuFile.h"
#include "AppMenu/Components/Editors/NewConfigItemEditor.h"

/* NewConfigItemEditor shares a localized object class key inherited from
   AppMenu::PopupEditor. */

/* Localized title text key: */
static const juce::Identifier newAppTitleKey    = "newAppTitleKey";
static const juce::Identifier newFolderTitleKey = "newFolderTitleKey";

/* 
 * Creates the new editor component. 
 */
AppMenu::NewConfigItemEditor::NewConfigItemEditor(
        const MenuItem parentFolder, 
        const bool isFolder,
        const int insertIndex,
        const std::function<void() > onConfirm) :
    AppMenu::PopupEditor(onConfirm, isFolder, !isFolder),
    parentFolder(parentFolder), 
    insertIndex(insertIndex)
{
#    if JUCE_DEBUG
    setName("NewConfigItemEditor");
#    endif
    setEditorTitle(localeText(isFolder ? newFolderTitleKey : newAppTitleKey));
}

/*
 * Creates the new application menu item and inserts it into the parent folder 
 * provided on editor construction.
 */
void AppMenu::NewConfigItemEditor::commitEdits()
{
    MenuFile appConfig;
    MenuItem rootFolder = appConfig.getRootFolderItem();
    appConfig.addMenuItem(getTitleField(),
            getIconNameField(),
            getCommandField(),
            getTerminalCheckboxState(),
            getCategories(),
            parentFolder, insertIndex);
}

