#define APPMENU_IMPLEMENTATION_ONLY
#include "NewConfigItemEditor.h"

/* Localized title text key: */
static const constexpr char* newAppTitleKey = "newAppTitleKey";
static const constexpr char* newFolderTitleKey = "newFolderTitleKey";

AppMenu::NewConfigItemEditor::NewConfigItemEditor(
        const MenuItem parentFolder, 
        const bool isFolder,
        const int insertIndex,
        const std::function<void() > onConfirm) :
    AppMenu::PopupEditor(
            localeText((isFolder ? newFolderTitleKey : newAppTitleKey)), 
            onConfirm, isFolder, !isFolder),
    parentFolder(parentFolder), 
    insertIndex(insertIndex)
{
#    if JUCE_DEBUG
    setName("NewConfigItemEditor");
#    endif
}

void AppMenu::NewConfigItemEditor::commitEdits()
{
    ConfigFile appConfig;
    MenuItem rootFolder = appConfig.getRootFolderItem();
    appConfig.addMenuItem(getTitleField(),
            getIconNameField(),
            getCommandField(),
            getTerminalCheckboxState(),
            getCategories(),
            parentFolder, insertIndex);
}
