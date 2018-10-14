#include "NewConfigItemEditor.h"

/* Localized title text key: */
static const constexpr char* newAppTitleKey = "newAppTitleKey";
static const constexpr char* newFolderTitleKey = "newFolderTitleKey";

NewConfigItemEditor::NewConfigItemEditor(
        const AppMenuItem& parentFolder, 
        const bool isFolder,
        const int insertIndex,
        const std::function<void() > onConfirm) :
    AppMenuPopupEditor(
            localeText((isFolder ? newFolderTitleKey : newAppTitleKey)), 
            onConfirm, isFolder, !isFolder),
    parentFolder(parentFolder), 
    insertIndex(insertIndex)
{
#    if JUCE_DEBUG
    setName("NewConfigItemEditor");
#    endif
}

void NewConfigItemEditor::commitEdits()
{
    AppConfigFile appConfig;
    AppMenuItem rootFolder = appConfig.getRootFolderItem();
    appConfig.addMenuItem(getTitleField(),
            getIconNameField(),
            getCommandField(),
            getTerminalCheckboxState(),
            getCategories(),
            parentFolder, insertIndex);
}
