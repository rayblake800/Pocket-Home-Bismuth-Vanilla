/*
  ==============================================================================

    NewConfigAppEditor.cpp
    Created: 23 Jan 2018 9:54:29pm
    Author:  anthony

  ==============================================================================
 */

#include "NewConfigAppEditor.h"

NewConfigAppEditor::NewConfigAppEditor(AppConfigFile& config,
        IconThread& iconThread, std::function<void() > onConfirm,
        int folderIndex) :
AppMenuPopupEditor("New pinned application", iconThread,
[this, &config, onConfirm, folderIndex](AppMenuPopupEditor* editor)
{
    AppConfigFile::AppItem newApp;
    newApp.name = editor->getNameField();
    newApp.icon = editor->getIconField();
    newApp.shell = editor->getCommandField();
    newApp.launchInTerminal = editor->launchInTerm();

    if (folderIndex >= 0 && folderIndex < config.getFolders().size())
    {
        AppConfigFile::AppFolder sourceFolder
                = config.getFolders()[folderIndex];
        newApp.folderIndex = folderIndex;
        newApp.index = sourceFolder.pinnedApps.size();
        config.addPinnedApp(newApp, newApp.folderIndex,
                newApp.index);
    } else
    {
        newApp.index = config.getFavorites().size();
        config.addFavoriteApp(newApp, newApp.index);
    }
    onConfirm();
},
[]()
{
}

, false, true, false)
{
}