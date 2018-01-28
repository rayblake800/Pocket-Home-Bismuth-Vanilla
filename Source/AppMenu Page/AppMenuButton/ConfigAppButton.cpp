/*
  ==============================================================================

    ConfigAppButton.cpp 
    Created: 11 Jan 2018 6:58:24pm
    Author:  Anthony Brown

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
    confirmDeleteTitle = String("Remove \"")+
            getAppName() + String("\" from favorites?");
    confirmDeleteMessage = 
            "This will permanently remove this link from the list of favorite applications.";
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
 * This will be an empty list, as ConfigAppButtons are not associated with
 * application categories.
 */
Array<String> ConfigAppButton::getCategories() const
{
    return Array<String>();
}

/**
 * @return the name or path used to load the icon file. 
 */
String ConfigAppButton::getIconName() const
{
    return appItem.icon;
}

/**
 * Gets a PopupEditorComponent configured to edit this button
 */
AppMenuPopupEditor* ConfigAppButton::getEditor()
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor("Edit Folder",
            iconThread,
            [this](AppMenuPopupEditor * editor)
            {
                editApp(editor->getNameField(), editor->getIconField(),
                        editor->getCommandField(), editor->launchInTerm());
            },
    [this]()
    {
        removeButtonSource();
    }, false);
    editor->setNameField(appItem.name);
    editor->setIconField(appItem.icon);
    editor->setCommandField(appItem.shell);
    editor->setTerminalCheckbox(appItem.launchInTerminal);
    return editor;
}

/**
 * Edit this button's data source in the config file.
 */
void ConfigAppButton::editApp
(String name, String icon, String command, bool terminal)
{
    int index=config.getFavoriteIndex(appItem);
    if (icon != appItem.icon)
    {
        loadIcon(icon);
    }
    appItem.name = name;
    appItem.icon = icon;
    appItem.shell = command;
    appItem.launchInTerminal = terminal;
    config.removeFavoriteApp(index,false);
    config.addFavoriteApp(appItem,index);
}

/**
 * Remove this button's application from config, and remove the button
 * from its parent component.
 */
void ConfigAppButton::removeButtonSource()
{

    config.removeFavoriteApp(config.getFavoriteIndex(appItem));
    reloadAllButtons();
}

/**
 * Return true if this button's data source has an index that can be
 * moved by a given amount.
 * @param offset some value to add to the button index
 * @return true if this button's data source has an index value i that can
 * be changed to i+offset 
 */
bool ConfigAppButton::canChangeIndex(int offset)
{
    int newIndex = config.getFavoriteIndex(appItem) + offset;
    return newIndex >= 0 && newIndex < config.getFavorites().size();
}

/**
 * If possible, change the index of this button's data source by some
 * offset amount.
 * @param offset will be added to the button's current index, if possible.
 */
void ConfigAppButton::moveDataIndex(int offset)
{
    if (canChangeIndex(offset))
    {
        int index = config.getFavoriteIndex(appItem);
        config.removeFavoriteApp(index,false);
        config.addFavoriteApp(appItem,index+offset);
        DBG(String("Moved ")+appItem.name+String(" from ")+String(index)+
                String(" to ")+String(config.getFavoriteIndex(appItem)));
    }
}