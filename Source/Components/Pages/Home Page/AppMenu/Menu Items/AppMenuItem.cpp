#include "MainConfigFile.h"
#include "AppMenuItem.h"

AppMenuItem::AppMenuItem()
{
    MainConfigFile config;
    config.addListener(this,{MainConfigFile::termLaunchCommandKey});
    loadAllConfigProperties();
}

/**
 * @return true if this menu item is an application folder.
 */
bool AppMenuItem::isFolder() const
{
    return false;
}

/**
 * @return all menu items in this folder, or an empty array if this isn't
 *          a folder.
 */
juce::Array<AppMenuItem::Ptr> AppMenuItem::getFolderItems() const
{
    return {};
}

/**
 * @return the display name of the associated application.
 */
juce::String AppMenuItem::getAppName() const
{
    return juce::String();
}

/**
 * @return the application shell command or directory path.
 */
juce::String AppMenuItem::getCommand() const
{
    return juce::String();
}

/**
 * @return true iff this menu item is an application that launches in
 *          the terminal. 
 */
bool AppMenuItem::isTerminalApp() const
{
    return false;
}

/**
 * @return true iff changing this menu item makes changes to .desktop or
 *          .directory files.
 */
bool AppMenuItem::changesDesktopEntries() const
{
    return false;
}

/**
 * @return all application categories linked to this menu item.
 */
juce::StringArray AppMenuItem::getCategories() const
{
    return {};
}

/**
 * @return the name or path used to load the icon file. 
 */
juce::String AppMenuItem::getIconName() const
{
    return juce::String();
}

/**
 * @return true iff this menu item has an index that can be moved by a given 
 *          amount.
 */
bool AppMenuItem::canChangeIndex(int offset) const
{
    return false;
}

/**
 * @return true iff this menu item and another share the same
 *          properties.
 */
bool AppMenuItem::operator==(const AppMenuItem& toCompare) const
{
    return getAppName() == toCompare.getAppName()
            && getCategories() == toCompare.getCategories()
            && getCommand() == toCompare.getCommand()
            && isFolder() == toCompare.isFolder()
            && isTerminalApp() == toCompare.isTerminalApp()
            && getIconName() == toCompare.getIconName()
            && changesDesktopEntries() == toCompare.changesDesktopEntries();
}

/**
 * Assigns this FactoryInterface to a folder menu item.  This does nothing if 
 * the menu item is not a folder.
 */
AppMenuItem::Ptr AppMenuItem::FactoryInterface::setFactory
(AppMenuItem::Ptr menuItem)
{
    if (menuItem->isFolder())
    {
        menuItem->factoryInterface = this;
    }
    return menuItem;
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
juce::String AppMenuItem::getConfirmDeleteTitle() const
{
    return txt.localeText(delete_APP) + getAppName()
            + txt.localeText(question_mark);
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String AppMenuItem::getConfirmDeleteMessage() const
{
    return txt.localeText(really_delete);
}

/**
 * @return true iff this menu item has categories that can be edited,
 *          defaults to false.
 */
bool AppMenuItem::hasEditableCategories() const
{
    return false;
}

/**
 * @return true iff this menu item has a command that can be edited,
 *          defaults to false.
 */
bool AppMenuItem::hasEditableCommand() const
{
    return false;
}

/**
 * @return the title to display over an editor for this menu item. 
 */
juce::String AppMenuItem::getEditorTitle() const
{
    return txt.localeText(edit_menu_item);
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > AppMenuItem::getEditorCallback()
{
    return [](AppMenuPopupEditor*)
    {
    };
}

/**
 * Removes the source of this menu item's data
 * 
 * @return true iff the source was removed.
 */
bool AppMenuItem::removeMenuItemSource()
{
    return false;
}

/**
 * If possible, change the index of this menu item by some
 * offset amount.
 */
bool AppMenuItem::moveDataIndex(int offset)
{
    return false;
}

/**
 * Gets the string to add before a launch command to make it launch in the
 * terminal.
 */
juce::String AppMenuItem::getTermLaunchPrefix() const
{
    return termLaunchPrefix;
}

/**
 * Get an AppMenuItem for an application link provided by the 
 * AppConfigFile.
 */
AppMenuItem::Ptr AppMenuItem::create
(const AppConfigFile::AppItem& appItem) const
{
    if (factoryInterface == nullptr)
    {
        return nullptr;
    }
    return factoryInterface->create(appItem);
}

/**
 * Get an AppMenuItem for an application link that was read from a 
 * desktop entry file.
 */
AppMenuItem::Ptr AppMenuItem::create(const DesktopEntry& desktopEntry) const
{
    if (factoryInterface == nullptr)
    {
        return nullptr;
    }
    return factoryInterface->create(desktopEntry);
}

/**
 * Get an AppMenuItem for an application folder provided by the 
 * AppConfigFile. 
 */
AppMenuItem::Ptr AppMenuItem::create
(const AppConfigFile::AppFolder& appFolder) const
{
    if (factoryInterface == nullptr)
    {
        return nullptr;
    }
    return factoryInterface->create(appFolder);
}

/**
 * Updates the termLaunchPrefix if it's changed in configuration.
 */
void AppMenuItem::configValueChanged(juce::String propertyKey)
{
    using namespace juce;
    if (propertyKey == MainConfigFile::termLaunchCommandKey)
    {
        MainConfigFile mainConfig;
        termLaunchPrefix = mainConfig.getConfigValue<String>(propertyKey);
    }
}