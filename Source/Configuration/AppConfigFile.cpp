/*
  ==============================================================================

    AppConfigFile.cpp
    Created: 5 Jan 2018 9:53:32pm
    Author:  anthony

  ==============================================================================
 */

#include "AppConfigFile.h"
#include "../Utils.h"

AppConfigFile::AppConfigFile() : ConfigFile(filenameConst)
{
    const ScopedLock readLock(lock);
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    var jsonConfig = JSON::parse(configFile);
    var defaultConfig = var::null;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

AppConfigFile::~AppConfigFile()
{
}

Array<String> AppConfigFile::getStringKeys() const
{
    return Array<String>();
}

Array<String> AppConfigFile::getBoolKeys() const
{
    return Array<String>();
}

//########################### Application Data #################################

AppConfigFile::AppItem::AppItem()
{
}

AppConfigFile::AppItem::AppItem(var jsonObj)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");
    shell = jsonObj.getProperty("shell", "");
    folderIndex = -1;
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
    const ScopedLock readLock(lock);
    return favoriteApps;
}

/**
 * Add a new app to the list of pinned favorite apps in the config file
 */
void AppConfigFile::addFavoriteApp(AppItem newApp, int index)
{
    const ScopedLock changeLock(lock);
    favoriteApps.insert(index, newApp);
    changesPending = true;
    writeChanges();
}

/**
 * Remove an app from the list of favorite applications
 */
void AppConfigFile::removeFavoriteApp(int index)
{
    const ScopedLock changeLock(lock);
    if (index >= 0 && index < favoriteApps.size())
    {
        favoriteApps.remove(index);
        changesPending = true;
        writeChanges();
    }
}

//######################### Folder/Category Data ###############################

AppConfigFile::AppFolder::AppFolder()
{
}

AppConfigFile::AppFolder::AppFolder(var jsonObj,int index):
index(index)
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

    pinnedApps.clear();
    var appList = jsonObj["pinned apps"];
    if (appList.isArray())
    {
        for (var app : *appList.getArray())
        {
            AppItem pinnedApp = AppItem(app);
            pinnedApp.index = pinnedApps.size();
            pinnedApp.folderIndex = index;
            pinnedApps.add(pinnedApp);
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

    Array<var> appArray;
    for (int i = 0; i < pinnedApps.size(); i++)
    {
        appArray.add(var(pinnedApps[i].getDynamicObject()));
    }
    folderObject->setProperty(String("pinned apps"), appArray);
    return folderObject;
}

bool AppConfigFile::AppFolder::operator==(const AppFolder& rhs) const
{
    return name == rhs.name &&
            icon == rhs.icon &&
            categories == rhs.categories &&
            pinnedApps == rhs.pinnedApps;
}

/**
 * @return A list of folders to display in the AppMenu 
 */
Array<AppConfigFile::AppFolder> AppConfigFile::getFolders()
{
    const ScopedLock readLock(lock);
    return categoryFolders;
}

/**
 * Add a new folder to the list of AppFolders in the config file
 */
void AppConfigFile::addAppFolder(AppFolder newFolder, int index)
{
    const ScopedLock changeLock(lock);
    for (AppFolder& folder : categoryFolders)
    {
        if (folder.index >= index)
        {
            folder.index++;
        }
    }
    categoryFolders.insert(index, newFolder);
    changesPending = true;
    writeChanges();
}

/**
 * Remove a folder from the list of AppFolders
 */
void AppConfigFile::removeAppFolder(int index)
{
    int size = categoryFolders.size();
    const ScopedLock changeLock(lock);
    for (AppFolder& folder : categoryFolders)
    {
        if (folder.index > index)
        {
            folder.index--;
        }
    }
    categoryFolders.remove(index);
    changesPending = true;
    writeChanges();
}

/**
 * Add a new app to the top of an AppFolder
 */
void AppConfigFile::addPinnedApp
(AppItem newApp, int folderIndex, int appIndex)
{
    const ScopedLock changeLock(lock);
    if (folderIndex >= 0 && folderIndex < categoryFolders.size())
    {
        categoryFolders[folderIndex].pinnedApps.insert(appIndex, newApp);
        changesPending = true;
        writeChanges();
    }
}

/**
 * Remove a pinned app from an AppFolder
 */
void AppConfigFile::removePinnedApp(int folderIndex, int appIndex)
{
    const ScopedLock changeLock(lock);
    DBG(String("Removing app ") + String(appIndex) + String(" from folder ") +
            String(folderIndex) + String(" of ") + String(categoryFolders.size()));
    if (folderIndex >= 0 && folderIndex < categoryFolders.size())
    {
        AppFolder editedFolder = categoryFolders[folderIndex];
        editedFolder.pinnedApps.remove(appIndex);
        jassert(editedFolder.pinnedApps.size() == 0);
        removeAppFolder(folderIndex);
        addAppFolder(editedFolder, folderIndex);
    }
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
            fave.index = favoriteApps.size();
            favoriteApps.add(fave);
        }
    }
    //load categories
    var categoryList = getProperty(config, defaultConfig, FOLDERS_KEY);
    if (categoryList.isArray())
    {
        for (const var& folder : *categoryList.getArray())
        {
            AppFolder menuFolder = AppFolder(folder,categoryFolders.size());
            categoryFolders.add(menuFolder);
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