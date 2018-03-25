#include "Utils.h"
#include "FolderMenuItem.h"

FolderMenuItem::FolderMenuItem(
        AppConfigFile& appConfig,
        MainConfigFile& mainConfig,
        const AppConfigFile::AppFolder& appFolder,
        DesktopEntries& desktopEntries) :
AppMenuItem(mainConfig),
Localized("FolderMenuItem"),
config(appConfig),
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
Array<AppMenuItem::Ptr> FolderMenuItem::getFolderItems() const
{
    std::set<DesktopEntry> folderEntries =
            desktopEntries.getCategoryListEntries(appFolder.categories);
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
String FolderMenuItem::getAppName() const
{
    return appFolder.name;
}

/**
 * @return all application categories linked to this folder.
 */
StringArray FolderMenuItem::getCategories() const
{
    return appFolder.categories;
}

/**
 * @return the name or path used to load the icon file. 
 */
String FolderMenuItem::getIconName() const
{
    return appFolder.icon;
}

/**
 * Return true if this menu item has an index that can be moved by a given 
 * amount.
 */
bool FolderMenuItem::canChangeIndex(int offset) const
{
    int newIndex = config.getFolderIndex(appFolder) + offset;
    return newIndex >= 0 && newIndex < config.getFolders().size();
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
String FolderMenuItem::getConfirmDeleteTitle() const
{
    return localeText(delete_NAME) + appFolder.name 
            + localeText(folder);
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
String FolderMenuItem::getConfirmDeleteMessage() const
{
    return localeText(will_remove_folder);
}

/**
 * @return the title to display over an editor for this menu item. 
 */
String FolderMenuItem::getEditorTitle() const
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
    config.removeAppFolder(config.getFolderIndex(appFolder));
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
        int index = config.getFolderIndex(appFolder);
        if (index < 0)
        {
            return false;
        }
        config.removeAppFolder(index, false);
        config.addAppFolder(appFolder, index + offset);
        DBG("FolderMenuItem::" << __func__ << ": Moved " << appFolder.name
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
(String name, String icon, StringArray categories)
{
    int index = config.getFolderIndex(appFolder);
    appFolder.name = name;
    appFolder.icon = icon;
    appFolder.categories = categories;
    config.removeAppFolder(index, false);
    config.addAppFolder(appFolder, index);
}
