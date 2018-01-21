/*
  ==============================================================================

    desktopEntryButton.cpp
    Created: 11 Jan 2018 6:57:42pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "DesktopEntryButton.h"

DesktopEntryButton::DesktopEntryButton
(DesktopEntry desktopEntry, int index, int column, IconThread& iconThread)
: AppMenuButton
(desktopEntry.getValue(DesktopEntry::name), index, column, iconThread),
desktopEntry(desktopEntry)
{
    loadIcon(desktopEntry.getValue(DesktopEntry::icon));
}

/**
 * @return true if this button is for an application folder
 */
bool DesktopEntryButton::isFolder() const
{
    return desktopEntry.getType() == DesktopEntry::directory;
}

/**
 * @return the display name of the associated application
 */
String DesktopEntryButton::getAppName() const
{
    return desktopEntry.getValue(DesktopEntry::name);
}

/**
 * @return application shell command or directory path.
 */
String DesktopEntryButton::getCommand() const
{
    String command = desktopEntry.getValue(DesktopEntry::exec);
    if (desktopEntry.getValue(DesktopEntry::terminal))
    {
        MainConfigFile& config = PocketHomeApplication::getInstance()
                ->getConfig();
        command = config.getConfigString
                (MainConfigFile::termLaunchCommandKey) + String(" ") + command;
    }
    return command;
}

/**
 * @return all application categories linked to this button.
 */
Array<String> DesktopEntryButton::getCategories() const
{
    return desktopEntry.getValue(DesktopEntry::categories);
}

/**
 * Gets a PopupEditorComponent configured to edit this button
 * @return a new PopupEditorComponent, ready to be added to the screen.
 */
AppMenuPopupEditor* DesktopEntryButton::getEditor()
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor("Edit Application",
            iconThread,
            true, true,
            [this](AppMenuPopupEditor * editor)
            {
                editEntry(editor->getNameField(), editor->getIconField(), 
                        editor->getCategories(),editor->getCommandField(), 
                        editor->launchInTerm());
            },
    [this]()
    {
        hideEntry();
    });
    editor->setNameField(getAppName());
    editor->setIconField(desktopEntry.getValue(DesktopEntry::icon));
    editor->setCategories(desktopEntry.getValue(DesktopEntry::categories));
    editor->setCommandField(desktopEntry.getValue(DesktopEntry::exec));
    editor->setTerminalCheckbox(desktopEntry.getValue(DesktopEntry::terminal));
    return editor;
};

/**
 * Update this button's desktopEntry. This writes to 
 * ~/.local/share/applications, so changes will only affect the current user.
 * @param name application display name
 * @param icon application icon
 * @param categories application categories
 * @param command application launch command
 * @param useTerminal sets if this launches in a terminal window
 */
void DesktopEntryButton::editEntry(String name, String icon,
        Array<String> categories,
        String command, bool useTerminal)
{
    desktopEntry.setValue(DesktopEntry::name, name);
    desktopEntry.setValue(DesktopEntry::icon, icon);
    desktopEntry.setValue(DesktopEntry::categories, categories);
    desktopEntry.setValue(DesktopEntry::exec, command);
    desktopEntry.setValue(DesktopEntry::terminal, useTerminal);
    desktopEntry.writeFile();
}

/**
 * Sets this button's desktopEntry to not display in pocket-home for the
 * current user, and reload all AppMenuButtons.
 */
void DesktopEntryButton::hideEntry()
{
    Array<String> notShowIn = desktopEntry.getValue(DesktopEntry::notShowIn);
    notShowIn.add("pocket-home");
    desktopEntry.setValue(DesktopEntry::notShowIn, notShowIn);
    desktopEntry.writeFile();
    AppMenuComponent* appMenu =
            dynamic_cast<AppMenuComponent*> (getParentComponent());
    if (appMenu != nullptr)
    {
        appMenu->loadButtons();
    }

}