#include "../../../../Utils.h"
#include "../AppMenuItemFactory.h"
#include "FolderMenuItem.h"

FolderMenuItem::FolderMenuItem(const AppConfigFile::AppFolder& appFolder,
        DesktopEntries& desktopEntries) :
appFolder(appFolder),
desktopEntries(desktopEntries) { }

FolderMenuItem::~FolderMenuItem() { }

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
        folderItems.add(AppMenuItemFactory::create(entry));
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
Array<String> FolderMenuItem::getCategories() const
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
    AppConfigFile config;
    int newIndex = config.getFolderIndex(appFolder) + offset;
    return newIndex >= 0 && newIndex < config.getFolders().size();
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
String FolderMenuItem::getConfirmDeleteTitle() const
{
    return String("Delete \"") + appFolder.name + String("\" folder?");
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
String FolderMenuItem::getConfirmDeleteMessage() const
{
    return "This will permanently remove this folder from the menu.";
}

/**
 * @return the title to display over an editor for this menu item. 
 */
String FolderMenuItem::getEditorTitle() const
{
    return "Edit folder";
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
        AppConfigFile config;
        int index = config.getFolderIndex(appFolder);
        if (index < 0)
        {
            return false;
        }
        config.removeAppFolder(index, false);
        config.addAppFolder(appFolder, index + offset);
        DBG(String("Moved ") + appFolder.name + String(" from ") + String(index) +
                String(" to ") + String(config.getFolderIndex(appFolder)));
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
(String name, String icon, Array<String> categories)
{
    AppConfigFile config;
    int index = config.getFolderIndex(appFolder);
    appFolder.name = name;
    appFolder.icon = icon;
    appFolder.categories = categories;
    config.removeAppFolder(index, false);
    config.addAppFolder(appFolder, index);
}
