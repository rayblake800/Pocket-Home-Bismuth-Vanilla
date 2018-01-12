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
}

DynamicObject* AppConfigFile::AppItem::getDynamicObject()
{
    DynamicObject * appObject = new DynamicObject();
    appObject->setProperty("name", name);
    appObject->setProperty("icon", icon);
    appObject->setProperty("shell", shell);
    return appObject;
}

bool AppConfigFile::AppItem::operator==(const AppItem& rhs) const
{
    return icon == rhs.icon &&
            name == rhs.name &&
            shell == rhs.shell;
}

Array<AppConfigFile::AppItem> AppConfigFile::getFavorites()
{
    const ScopedLock readLock(lock);
    return favoriteApps;
}

void AppConfigFile::setFavorites(Array<AppItem> newFavorites)
{
    const ScopedLock writeLock(lock);
    favoriteApps = newFavorites;
    {
        const ScopedUnlock writeUnlock(lock);
        writeChanges();
    }
}

//######################### Folder/Category Data ###############################

AppConfigFile::AppFolder::AppFolder()
{
}

AppConfigFile::AppFolder::AppFolder(var jsonObj)
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
            AppItem pinnedApp=AppItem(app);
            pinnedApp.index=pinnedApps.size();
            pinnedApp.folder=name;
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

Array<AppConfigFile::AppFolder> AppConfigFile::getFolders()
{
    const ScopedLock readLock(lock);
    return categoryFolders;
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
            fave.index=favoriteApps.size();
            favoriteApps.add(fave);
        }
    }
    //load categories
    var categoryList = getProperty(config, defaultConfig, FOLDERS_KEY);
    if (categoryList.isArray())
    {
        for (const var& folder : *categoryList.getArray())
        {
            AppFolder menuFolder = AppFolder(folder);
            menuFolder.index=categoryFolders.size();
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