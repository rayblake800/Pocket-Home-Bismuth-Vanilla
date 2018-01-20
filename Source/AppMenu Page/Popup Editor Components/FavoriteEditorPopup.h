/*
  ==============================================================================

    FavoriteEditorPopup.h
    Created: 12 Jan 2018 9:54:07pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "../../Configuration/AppConfigFile.h"
#include "../AppMenuButton/ConfigAppButton.h"
#include "PopupEditorComponent.h"

class FavoriteEditorPopup : public PopupEditorComponent {
public:
    FavoriteEditorPopup(ConfigAppButton* faveButton, AppConfigFile& config,
            AppConfigFile::AppItem appItem, IconThread& iconThread);
    virtual ~FavoriteEditorPopup();
private:
    void deleteBtnPressed();
    void confirm();
    ConfigAppButton::Ptr faveButton;
    AppConfigFile& config;
    AppConfigFile::AppItem appItem;
};