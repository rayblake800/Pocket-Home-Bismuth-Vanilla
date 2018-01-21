/*
  ==============================================================================

    ConfigAppButton.cpp
    Created: 11 Jan 2018 6:58:24pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "ConfigAppButton.h"

ConfigAppButton::ConfigAppButton(AppConfigFile& config,
        AppConfigFile::AppItem appItem,
        int index, int column, IconThread& iconThread) :
AppMenuButton(appItem.name, index, column, iconThread),
config(config),
appItem(appItem)
{
    loadIcon(appItem.icon);
}

/**
 * Check if this button is for an application folder
 * @return false
 */
bool ConfigAppButton::isFolder() const
{
    return false;
}

/**
 * @return the display name of the associated app
 */
String ConfigAppButton::getAppName() const
{
    return appItem.name;
}

/**
 * @return the application's launch command
 */
String ConfigAppButton::getCommand() const
{
    String command = appItem.shell;
    if (appItem.launchInTerminal)
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
Array<String> ConfigAppButton::getCategories() const
{
    return Array<String>();
}

/**
 * Gets a PopupEditorComponent configured to edit this button
 */
AppMenuPopupEditor* ConfigAppButton::getEditor()
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor("Edit Folder",
            iconThread,
            false, true,
            [this](AppMenuPopupEditor * editor)
            {
                editApp(editor->getNameField(), editor->getIconField(),
                        editor->getCommandField(), editor->launchInTerm());
            },
    [this]()
    {
        deleteApp();
    });
    editor->setNameField(appItem.name);
    editor->setIconField(appItem.icon);
    editor->setCommandField(appItem.shell);
    editor->setTerminalCheckbox(appItem.launchInTerminal);
    return editor;
}

/**
 * Edit this button's application in the settings
 */
void ConfigAppButton::editApp
(String name, String icon, String command, bool terminal)
{
    if (icon != appItem.icon)
    {
        loadIcon(icon);
    }
    appItem.name = name;
    appItem.icon = icon;
    appItem.shell = command;
    appItem.launchInTerminal = terminal;
    if (appItem.folderIndex > 0)
    {
        config.removePinnedApp(appItem.folderIndex, appItem.index);
        config.addPinnedApp(appItem, appItem.folderIndex, appItem.index);
    } else
    {
        config.removeFavoriteApp(appItem.index);
        config.addFavoriteApp(appItem, appItem.index);
    }
}

/**
 * Remove this button's application from config, and remove the button
 * from its parent component.
 */
void ConfigAppButton::deleteApp()
{
    if (appItem.folderIndex > 0)
    {
        config.removePinnedApp(appItem.folderIndex, appItem.index);
    } else
    {
        config.removeFavoriteApp(appItem.index);
    }
    reloadAllButtons();
}