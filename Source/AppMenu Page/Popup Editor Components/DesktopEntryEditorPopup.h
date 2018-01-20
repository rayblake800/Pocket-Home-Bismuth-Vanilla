/*
  ==============================================================================

    DesktopEntryEditorPopup.h
    Created: 12 Jan 2018 9:54:39pm
    Author:  anthony

  ==============================================================================
*/

#pragma once
#include "../DesktopEntry.h"
#include "../AppMenuButton/DesktopEntryButton.h"
#include "PopupEditorComponent.h"

class DesktopEntryEditorPopup : public PopupEditorComponent {
public:
    DesktopEntryEditorPopup(DesktopEntryButton* appButton,
            DesktopEntry desktopEntry, IconThread& iconThread);
    virtual ~DesktopEntryEditorPopup();
private:
    void deleteBtnPressed();
    void confirm();
    DesktopEntryButton::Ptr appButton;
    DesktopEntry desktopEntry;
};