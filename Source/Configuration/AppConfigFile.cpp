#include "AppConfigFile.h"
#include "../Utils.h"

Array<AppConfigFile::AppItem> AppConfigFile::favoriteApps;
Array<AppConfigFile::AppFolder> AppConfigFile::categoryFolders;
CriticalSection AppConfigFile::appConfigLock;

AppConfigFile::AppConfigFile() : ConfigFile(filenameConst)
{
    if (!fileOpened())
    {
        const ScopedLock readLock(appConfigLock);
        var jsonConfig = openFile();
        var defaultConfig = var();
        readDataFromJson(jsonConfig, defaultConfig);
        writeChanges();
    }
}

AppConfigFile::~AppConfigFile() { }


//########################### Application Data #################################

AppConfigFile::AppItem::AppItem() { }

AppConfigFile::AppItem::AppItem(var jsonObj)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");
    shell = jsonObj.getProperty("shell", "");
    launchInTerminal = jsonObj.getProperty("launch in terminal", false);
}

DynamicObject* AppConfigFile::AppItem::getDynamicObject()
{
    DynamicObject * appObject = new DynamicObject();
    appObject->setProperty("name", name);
    appObject->setProperty("icon", icon);
    appObject->setProperty("shell", shell);
    appObject->setProperty("launch in terminal", launchInTerminal);
    return appObject;
}

bool AppConfigFile::AppItem::operator==(const AppItem& rhs) const
{
    return icon == rhs.icon &&
            name == rhs.name &&
            shell == rhs.shell;
}

/**
 * @return a list of AppItems to be pinned to the main column 
 * of the AppMenu
 */
Array<AppConfigFile::AppItem> AppConfigFile::getFavorites()
{
    const ScopedLock readLock(appConfigLock);
    return favoriteApps;
}

/**
 * Add a new app to the list of pinned favorite apps in the config file
 */
void AppConfigFile::addFavoriteApp
(AppItem newApp, int index, bool writeChangesNow)
{
    const ScopedLock changeLock(appConfigLock);
    favoriteApps.insert(index, newApp);
    markPendingChanges();
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Remove an app from the list of favorite applications
 */
void AppConfigFile::removeFavoriteApp(int index, bool writeChangesNow)
{
    const ScopedLock changeLock(appConfigLock);
    if (index >= 0 && index < favoriteApps.size())
    {
        favoriteApps.remove(index);
        markPendingChanges();
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/**
 * Find the index of an AppItem in favorites.
 */
int AppConfigFile::getFavoriteIndex(AppItem toFind)
{
    return favoriteApps.indexOf(toFind);
}

//######################### Folder/Category Data ###############################

AppConfigFile::AppFolder::AppFolder() { }

AppConfigFile::AppFolder::AppFolder(var jsonObj, int index)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");

    categories.clear();
    var catList = jsonObj["categories"];
    if (catList.isArray())
    {
        for (var category : *catList.getArray())
        {
            categories.add(category);
        }
    }
}

DynamicObject* AppConfigFile::AppFolder::getDynamicObject()
{
    DynamicObject * folderObject = new DynamicObject();
    folderObject->setProperty("name", name);
    folderObject->setProperty("icon", icon);

    Array<var> categoryArray;
    for (int i = 0; i < categories.size(); i++)
    {
        categoryArray.add(categories[i]);
    }
    folderObject->setProperty(String("categories"), categoryArray);
    return folderObject;
}

bool AppConfigFile::AppFolder::operator==(const AppFolder& rhs) const
{
    return name == rhs.name &&
            icon == rhs.icon &&
            categories == rhs.categories;
}

/**
 * @return A list of folders to display in the AppMenu 
 */
Array<AppConfigFile::AppFolder> AppConfigFile::getFolders()
{
    const ScopedLock readLock(appConfigLock);
    return categoryFolders;
}

/**
 * Add a new folder to the list of AppFolders in the config file
 */
void AppConfigFile::addAppFolder
(AppFolder newFolder, int index, bool writeChangesNow)
{
    const ScopedLock changeLock(appConfigLock);
    categoryFolders.insert(index, newFolder);
    markPendingChanges();
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Remove a folder from the list of AppFolders
 */
void AppConfigFile::removeAppFolder(int index, bool writeChangesNow)
{
    int size = categoryFolders.size();
    const ScopedLock changeLock(appConfigLock);
    categoryFolders.remove(index);
    markPendingChanges();
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Find the index of an AppFolder in the list of folders.
 */
int AppConfigFile::getFolderIndex(AppFolder toFind)
{
    return categoryFolders.indexOf(toFind);
}

//################################# File IO ####################################

/**
 * Read in this object's data from a json config object
 */
void AppConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    //load favorites
    var favoriteList = getProperty(config, defaultConfig, FAVORITES_KEY);
    if (favoriteList.isArray())
    {
        for (const var& app : *favoriteList.getArray())
        {
            AppItem fave = AppItem(app);
            if (!favoriteApps.contains(fave))
            {
                favoriteApps.add(fave);
            }
        }
    }
    //load categories
    var categoryList = getProperty(config, defaultConfig, FOLDERS_KEY);
    if (categoryList.isArray())
    {
        for (const var& folder : *categoryList.getArray())
        {
            AppFolder menuFolder = AppFolder(folder, categoryFolders.size());
            if (!categoryFolders.contains(menuFolder))
            {
                categoryFolders.add(menuFolder);
            }
        }
    }
}

/**
 * Copy all config data to a json object
 */
void AppConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
{
    ConfigFile::copyDataToJson(jsonObj);
    //set favorites
    Array<var> favoriteArray;
    for (int i = 0; i < favoriteApps.size(); i++)
    {
        favoriteArray.add(var(favoriteApps[i].getDynamicObject()));
    }

    //set folders
    Array<var> categoryArray;
    for (int i = 0; i < categoryFolders.size(); i++)
    {
        categoryArray.add(var(categoryFolders[i].getDynamicObject()));
    }
    jsonObj->setProperty(FAVORITES_KEY, favoriteArray);
    jsonObj->setProperty(FOLDERS_KEY, categoryArray);
}

std::vector<ConfigFile::DataKey> AppConfigFile::getDataKeys() const
{
    return {};
}