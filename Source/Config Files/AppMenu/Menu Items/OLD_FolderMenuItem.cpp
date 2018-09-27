#include "Utils.h"
#include "FolderMenuItem.h"

FolderMenuItem::FolderMenuItem(
        const AppFolder& appFolder,
        DesktopEntryLoader& desktopEntries) :
Localized("FolderMenuItem"),
appFolder(appFolder),
desktopEntries(desktopEntries) { }

/**
 * Check if this button is for an application folder
 * @return true
 */
bool FolderMenuItem::isFolder() const
{
    return true;
}

/**
 * @return all menu items in this folder
 */
juce::Array<AppMenuItem::Ptr> FolderMenuItem::getFolderItems() const
{
    using namespace juce;
    std::set<DesktopEntry> folderEntries =
            desktopEntries.getCategoryListEntries(appFolder.getCategories());
    Array<AppMenuItem::Ptr> folderItems;
    for (const DesktopEntry& entry : folderEntries)
    {
        folderItems.add(create(entry));
    }
    return folderItems;
}

/**
 * @return the display name of the associated folder
 */
juce::String FolderMenuItem::getAppName() const
{
    return appFolder.getName();
}

/**
 * @return all application categories linked to this folder.
 */
juce::StringArray FolderMenuItem::getCategories() const
{
    return appFolder.getCategories();
}

/**
 * @return the name or path used to load the icon file. 
 */
juce::String FolderMenuItem::getIconName() const
{
    return appFolder.getIcon();
}

/**
 * Return true if this menu item has an index that can be moved by a given 
 * amount.
 */
bool FolderMenuItem::canChangeIndex(int offset) const
{
    AppConfigFile config;
    int newIndex = config.getFolderIndex(appFolder) + offset;
    return newIndex >= 0 && newIndex < config.getFolders().size();
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
juce::String FolderMenuItem::getConfirmDeleteTitle() const
{
    return localeText(delete_NAME) + appFolder.getName() 
            + localeText(folder);
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String FolderMenuItem::getConfirmDeleteMessage() const
{
    return localeText(will_remove_folder);
}

/**
 * @return the title to display over an editor for this menu item. 
 */
juce::String FolderMenuItem::getEditorTitle() const
{
    return localeText(edit_folder);
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > FolderMenuItem::getEditorCallback()
{
    return [this](AppMenuPopupEditor * editor)
    {
        editFolder(editor->getNameField(), editor->getIconField(),
                editor->getCategories());
    };
}

/**
 * Removes the source of this menu item's data, deleting the folder from
 * apps.json
 */
bool FolderMenuItem::removeMenuItemSource()
{
    AppConfigFile config;
    config.removeFolder(config.getFolderIndex(appFolder));
    return true;
}

/**
 * If possible, change the index of this menu item by some
 * offset amount.
 */
bool FolderMenuItem::moveDataIndex(int offset)
{
    if (canChangeIndex(offset))
    {
        AppConfigFile config;
        int index = config.getFolderIndex(appFolder);
        if (index < 0)
        {
            return false;
        }
        config.removeFolder(index, false);
        config.addFolder(appFolder, index + offset);
        DBG("FolderMenuItem::" << __func__ << ": Moved " << appFolder.getName()
                << " from " << index << " to "
                << config.getFolderIndex(appFolder));
        return true;
    }
    return false;
}

/**
 * Assign new properties to this folder button, changing configuration files
 * @param name folder display name
 * @param icon folder icon image name/path
 * @param categories list of folder application categories
 */
void FolderMenuItem::editFolder
(juce::String name, juce::String icon, juce::StringArray categories)
{
    AppConfigFile config;
    int index = config.getFolderIndex(appFolder);
    appFolder.setName(name);
    appFolder.setIcon(icon);
    appFolder.setCategories(categories);
    config.removeFolder(index, false);
    config.addFolder(appFolder, index);
}
