/*
  ==============================================================================

    DesktopEntryEditorPopup.cpp
    Created: 12 Jan 2018 9:54:39pm
    Author:  anthony

  ==============================================================================
 */

#include "DesktopEntryEditorPopup.h"

DesktopEntryEditorPopup::DesktopEntryEditorPopup
(DesktopEntryButton* appButton, DesktopEntry desktopEntry,
        IconThread& iconThread) :
PopupEditorComponent("Edit Application", iconThread, true, true, true),
appButton(appButton),
desktopEntry(desktopEntry)
{
    setName(appButton->getAppName());
    setIcon(desktopEntry.getValue(DesktopEntry::icon));
    setCategories(desktopEntry.getValue(DesktopEntry::categories));
    setCommandField(desktopEntry.getValue(DesktopEntry::exec));
    setTerminalCheckbox(desktopEntry.getValue(DesktopEntry::terminal));
}

DesktopEntryEditorPopup::~DesktopEntryEditorPopup()
{
}

void DesktopEntryEditorPopup::deleteBtnPressed()
{
    appButton->hideEntry();
}

void DesktopEntryEditorPopup::confirm()
{
    appButton->editEntry(getNameField(),getIconField(),getCategories(),
            getCommandField(),launchInTerm());
    closePopup();
}