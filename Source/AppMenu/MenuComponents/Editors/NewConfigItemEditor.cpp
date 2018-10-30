#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/MenuComponents/Editors/NewConfigItemEditor.h"

/* Localized object class key: */
static const juce::Identifier localizedObjectKey =
    "AppMenu::NewConfigItemEditor";

/* Localized title text key: */
static const juce::Identifier newAppTitleKey = "newAppTitleKey";
static const juce::Identifier newFolderTitleKey = "newFolderTitleKey";

AppMenu::NewConfigItemEditor::NewConfigItemEditor(
        const MenuItem parentFolder, 
        const bool isFolder,
        const int insertIndex,
        const std::function<void() > onConfirm) :
    localeData(),
    AppMenu::PopupEditor(onConfirm, isFolder, !isFolder),
    parentFolder(parentFolder), 
    insertIndex(insertIndex)
{
#    if JUCE_DEBUG
    setName("NewConfigItemEditor");
#    endif
    setEditorTitle(localeData.getTitle(isFolder));
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

AppMenu::NewConfigItemEditor::LocaleData::LocaleData() : 
    Locale::TextUser(localizedObjectKey) { }

/*
 * Gets the localized title displayed on the editor.
 */
juce::String AppMenu::NewConfigItemEditor::LocaleData::getTitle
(const bool isFolder) const
{
    return localeText(isFolder ? newFolderTitleKey : newAppTitleKey);
}
