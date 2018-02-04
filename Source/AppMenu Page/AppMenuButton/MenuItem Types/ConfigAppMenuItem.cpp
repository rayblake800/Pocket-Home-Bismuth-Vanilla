#include "../../../Utils.h"
#include "../../../PocketHomeApplication.h"
#include "ConfigAppMenuItem.h"

ConfigAppMenuItem::ConfigAppMenuItem
(AppConfigFile::AppItem appItem, AppConfigFile& config) :
appItem(appItem),
config(config)
{
}

/**
 * Check if this menu item is an application folder
 * @return false
 */
bool ConfigAppMenuItem::isFolder() const
{
    return false;
}

/**
 * @return the display name of the associated app
 */
String ConfigAppMenuItem::getAppName() const
{
    return appItem.name;
}

/**
 * @return the application's launch command
 */
String ConfigAppMenuItem::getCommand() const
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
 * @return true iff this menu item is an application that launches in
 * the terminal.
 */
bool ConfigAppMenuItem::isTerminalApp() const
{
    return appItem.launchInTerminal;
}

/**
 * @return all application categories linked to this menu item.
 * This will be an empty list, as ConfigAppMenuItems are not associated with
 * application categories.
 */
Array<String> ConfigAppMenuItem::getCategories() const
{
    return Array<String>();
}

/**
 * @return the name or path used to load the icon file. 
 */
String ConfigAppMenuItem::getIconName() const
{
    return appItem.icon;
}

/**
 * Return true if this button's data source has an index that can be
 * moved by a given amount.
 * @param offset some value to add to the button index
 * @return true if this button's data source has an index value i that can
 * be changed to i+offset 
 */
bool ConfigAppMenuItem::canChangeIndex(int offset) const
{
    int newIndex = config.getFavoriteIndex(appItem) + offset;
    return newIndex >= 0 && newIndex < config.getFavorites().size();
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
String ConfigAppMenuItem::getConfirmDeleteTitle() const
{
    return String("Remove \"") + getAppName() + String("\" from favorites?");
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
String ConfigAppMenuItem::getConfirmDeleteMessage() const
{
    return "This will permanently remove this link from the list of favorite applications.";
}

/**
 * @return the title to display over an editor for this menu item. 
 */
String ConfigAppMenuItem::getEditorTitle() const
{
    return "Edit Application";
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > ConfigAppMenuItem::getEditorCallback()
{
    return [this](AppMenuPopupEditor * editor)
    {
        editApp(editor->getNameField(), editor->getIconField(),
                editor->getCommandField(), editor->launchInTerm());
    };
}

/**
 * Edit this button's data source in the config file.
 */
void ConfigAppMenuItem::editApp
(String name, String icon, String command, bool terminal)
{
    int index = config.getFavoriteIndex(appItem);
    appItem.name = name;
    appItem.icon = icon;
    appItem.shell = command;
    appItem.launchInTerminal = terminal;
    config.removeFavoriteApp(index, false);
    config.addFavoriteApp(appItem, index);
}

/**
 * Remove this menu item's application from the config file.
 */
void ConfigAppMenuItem::removeMenuItemSource()
{
    config.removeFavoriteApp(config.getFavoriteIndex(appItem));
}

/**
 * If possible, change the index of this menu item by some
 * offset amount.
 */
bool ConfigAppMenuItem::moveDataIndex(int offset)
{
    if (canChangeIndex(offset))
    {
        int index = config.getFavoriteIndex(appItem);
        if (index == -1)
        {
            return false;
        }
        config.removeFavoriteApp(index, false);
        config.addFavoriteApp(appItem, index + offset);
        DBG(String("Moved ") + appItem.name + String(" from ") + String(index) +
                String(" to ") + String(config.getFavoriteIndex(appItem)));
        return true;
    }
    return false;
}