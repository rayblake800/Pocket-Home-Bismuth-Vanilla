#include "../../Utils.h"
#include "AppFolderButton.h"
#include "../AppMenuComponents/AppMenuComponent.h"

AppFolderButton::AppFolderButton(AppConfigFile& config,
        AppConfigFile::AppFolder appFolder,
        int index, int column, IconThread& iconThread) :
AppMenuButton(appFolder.name, index, column, iconThread),
config(config),
appFolder(appFolder)
{
    confirmDeleteTitle = String("Delete \"") + appFolder.name
            + String("\" folder?");
    confirmDeleteMessage = 
            "This will permanently remove this folder from the menu.";
    loadIcon(appFolder.icon);
}

/**
 * Check if this button is for an application folder
 * @return true
 */
bool AppFolderButton::isFolder() const
{
    return true;
}

/**
 * @return the display name of the associated folder
 */
String AppFolderButton::getAppName() const
{
    return appFolder.name;
}

/**
 * @return the empty string, as AppFolderButtons don't need a
 * particular command
 */
String AppFolderButton::getCommand() const
{
    return "";
}

/**
 * @return all application categories linked to this button.
 */
Array<String> AppFolderButton::getCategories() const
{
    return appFolder.categories;
}

/**
 * @return the name or path used to load the icon file. 
 */
String AppFolderButton::getIconName() const
{
    return appFolder.icon;
}

/**
 * Gets a PopupEditorComponent configured to edit this button
 * @return a new PopupEditorComponent, ready to be added to the screen.
 */
AppMenuPopupEditor* AppFolderButton::getEditor()
{
    AppMenuPopupEditor* editor = new AppMenuPopupEditor("Edit Application",
            iconThread,
            [this](AppMenuPopupEditor * editor)
            {
                editFolder(editor->getNameField(), editor->getIconField(),
                        editor->getCategories());
            },
    [this]()
    {
        removeButtonSource();
    }, true, false);
    editor->setNameField(appFolder.name);
    editor->setIconField(appFolder.icon);
    editor->setCategories(appFolder.categories);
    return editor;
}

/**
 * Assign new properties to this folder button, changing configuration files
 * @param name folder display name
 * @param icon folder icon image name/path
 * @param categories list of folder application categories
 */
void AppFolderButton::editFolder
(String name, String icon, Array<String> categories)
{
    int index = config.getFolderIndex(appFolder);
    if (icon != appFolder.icon)
    {
        loadIcon(icon);
    }
    appFolder.name = name;
    appFolder.icon = icon;
    appFolder.categories = categories;
    config.removeAppFolder(index,false);
    config.addAppFolder(appFolder,index);
}

/**
 * Remove this folder from config, and reload buttons.
 */
void AppFolderButton::removeButtonSource()
{
    config.removeAppFolder(config.getFolderIndex(appFolder));
    reloadAllButtons();
}

/**
 * Return true if this button's data source has an index that can be
 * moved by a given amount.
 */
bool AppFolderButton::canChangeIndex(int offset)
{
    int newIndex = config.getFolderIndex(appFolder) + offset;
    return newIndex >= 0 && newIndex < config.getFolders().size();
}

/**
 * If possible, change the index of this button's data source by some
 * offset amount.
 */
void AppFolderButton::moveDataIndex(int offset)
{
    if (canChangeIndex(offset))
    {
        int index = config.getFolderIndex(appFolder);
        config.removeAppFolder(index,false);
        config.addAppFolder(appFolder,index+offset);
        DBG(String("Moved ")+appFolder.name+String(" from ")+String(index)+
                String(" to ")+String(config.getFolderIndex(appFolder)));
    }
}