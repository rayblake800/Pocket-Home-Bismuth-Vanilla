/*
  ==============================================================================

    FavoriteEditorPopup.cpp
    Created: 12 Jan 2018 9:54:07pm
    Author:  anthony

  ==============================================================================
 */

#include "FavoriteEditorPopup.h"

FavoriteEditorPopup::FavoriteEditorPopup
(ConfigAppButton* faveButton, AppConfigFile& config,
        AppConfigFile::AppItem appItem, IconThread& iconThread) :
PopupEditorComponent("Edit custom application", iconThread, false, true, true),
faveButton(faveButton),
config(config),
appItem(appItem)
{
    setName(appItem.name);
    setIcon(appItem.icon);
    setCommandField(appItem.shell);
    setTerminalCheckbox(appItem.launchInTerminal);
}

FavoriteEditorPopup::~FavoriteEditorPopup()
{
}

void FavoriteEditorPopup::deleteBtnPressed()
{
    faveButton->deleteApp();
}

void FavoriteEditorPopup::confirm()
{
    faveButton->editApp(getNameField(),getIconField(),
            getCommandField(),launchInTerm());
    closePopup();
}