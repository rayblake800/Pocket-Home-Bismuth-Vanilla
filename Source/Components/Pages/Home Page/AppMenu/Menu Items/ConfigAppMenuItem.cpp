#include "Utils.h"
#include "ConfigAppMenuItem.h"

ConfigAppMenuItem::ConfigAppMenuItem(const AppShortcut& appItem) :
Localized("ConfigAppMenuItem"),
appItem(appItem) { }

/**
 * @return the display name of the associated app
 */
juce::String ConfigAppMenuItem::getAppName() const
{
    return appItem.getDisplayName();
}

/**
 * @return the application's launch command
 */
juce::String ConfigAppMenuItem::getCommand() const
{
    using namespace juce;
    String command = appItem.getLaunchCommand();
    if (appItem.isTerminalApplication())
    {
        command = getTermLaunchPrefix() + String(" ") + command;
    }
    return command;
}

/**
 * @return true iff this menu item is an application that launches in
 * the terminal.
 */
bool ConfigAppMenuItem::isTerminalApp() const
{
    return appItem.isTerminalApplication();
}

/**
 * @return the name or path used to load the icon file. 
 */
juce::String ConfigAppMenuItem::getIconName() const
{
    return appItem.getIconName();
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
    AppConfigFile appConfig;
    int newIndex = appConfig.getShortcutIndex(appItem) + offset;
    return newIndex >= 0 && newIndex < appConfig.getShortcuts().size();
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
juce::String ConfigAppMenuItem::getConfirmDeleteTitle() const
{
    return localeText(remove_APP) 
            + getAppName() + localeText(from_favorites);
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String ConfigAppMenuItem::getConfirmDeleteMessage() const
{
    return localeText(will_remove_link);
}

/**
 * @return the title to display over an editor for this menu item. 
 */
juce::String ConfigAppMenuItem::getEditorTitle() const
{
    return localeText(edit_app);
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
(juce::String name, juce::String icon, juce::String command, bool terminal)
{
    AppConfigFile appConfig;
    int index = appConfig.getShortcutIndex(appItem);
    appItem = AppShortcut(name, icon, command, terminal);
    appConfig.removeShortcut(index, false);
    appConfig.addShortcut(appItem, index);
}

/**
 * Remove this menu item's application from the config file.
 */
bool ConfigAppMenuItem::removeMenuItemSource()
{
    AppConfigFile appConfig;
    appConfig.removeShortcut(appConfig.getShortcutIndex(appItem));
    return true;
}

/**
 * If possible, change the index of this menu item by some
 * offset amount.
 */
bool ConfigAppMenuItem::moveDataIndex(int offset)
{
    using namespace juce;
    if (canChangeIndex(offset))
    {
        AppConfigFile appConfig;
        int index = appConfig.getShortcutIndex(appItem);
        if (index == -1)
        {
            return false;
        }
        appConfig.removeShortcut(index, false);
        appConfig.addShortcut(appItem, index + offset);
        DBG("ConfigAppMenuItem::" << __func__ << ": Moved " 
                << appItem.getDisplayName()  << " from " << index << " to "
                << appConfig.getShortcutIndex(appItem));
        return true;
    }
    return false;
}
