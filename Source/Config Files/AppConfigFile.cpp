#include "AssetFiles.h"
#include "AppConfigFile.h"
#include "Utils.h"


juce::ScopedPointer<ResourceManager::SharedResource>
        AppConfigFile::sharedResource = nullptr;

juce::ReadWriteLock AppConfigFile::configLock;

AppConfigFile::AppConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{

    return new AppJson();
}) { }


//############ Application Shortcut Methods: ############

/**
 * @return a list of AppItems to be pinned to the main column 
 * of the AppMenu
 */
juce::Array<AppConfigFile::AppItem> AppConfigFile::getFavorites()
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    return config->getFavorites();
}

/**
 * Add a new app to the list of pinned favorite apps in the config file.
 */
void AppConfigFile::addFavoriteApp
(AppConfigFile::AppItem newApp, int index, bool writeChangesNow)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    config->addFavoriteApp(newApp,index,writeChangesNow);
}

/**
 * Remove an app from the list of favorite applications
 */
void AppConfigFile::removeFavoriteApp(int index, bool writeChangesNow)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    config->removeFavoriteApp(index,writeChangesNow);
}

/**
 * Find the index of an AppItem in favorites.
 */
int AppConfigFile::getFavoriteIndex(AppConfigFile::AppItem toFind)
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    return config->getFavoriteIndex(toFind);
}


//############ Application Folder Methods: ############
/**
 * @return A list of folders to display in the AppMenu.
 */
juce::Array<AppConfigFile::AppFolder> AppConfigFile::getFolders()
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    return config->getFolders();
}

/**
 * Add a new folder to the list of AppFolders in the config file.
 */
void AppConfigFile::addAppFolder
(AppConfigFile::AppFolder newFolder, int index, bool writeChangesNow)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    config->addAppFolder(newFolder,index,writeChangesNow);
}

/**
 * Remove a folder from the list of AppFolders.
 */
void AppConfigFile::removeAppFolder(int index, bool writeChangesNow)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    config->removeAppFolder(index,writeChangesNow);
}

/**
 * Find the index of an AppFolder in the list of folders.
 */
int AppConfigFile::getFolderIndex(AppConfigFile::AppFolder toFind)
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    AppJson* config = static_cast<AppJson*> (sharedResource.get());
    return config->getFolderIndex(toFind);
}

/**
 * Load an AppItem from json file data.
 */
AppConfigFile::AppItem::AppItem(juce::var jsonObj)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");
    shell = jsonObj.getProperty("shell", "");
    launchInTerminal = jsonObj.getProperty("launch in terminal", false);
}

/**
 * @return AppItem data stored as a DynamicObject* that can be written
 *          to json.
 */
juce::DynamicObject* AppConfigFile::AppItem::getDynamicObject()
{
    using namespace juce;
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


//######################### Folder/Category Data ###############################

/**
 * Load folder information from json data.
 */
AppConfigFile::AppFolder::AppFolder(juce::var jsonObj)
{
    using namespace juce;
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

/**
 * @return folder data as a DynamicObject* ready to be written to a 
 *          json file.
 */
juce::DynamicObject* AppConfigFile::AppFolder::getDynamicObject()
{
    using namespace juce;
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


//############################### JSON Data Access #############################
/**
 * @return a list of AppItems to be pinned to the main column 
 * of the AppMenu
 */
juce::Array<AppConfigFile::AppItem> AppConfigFile::AppJson::getFavorites()
{
    return favoriteApps;
}

/**
 * Add a new app to the list of pinned favorite apps in the config file
 */
void AppConfigFile::AppJson::addFavoriteApp
(AppItem newApp, int index, bool writeChangesNow)
{
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
void AppConfigFile::AppJson::removeFavoriteApp(int index, bool writeChangesNow)
{
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
int AppConfigFile::AppJson::getFavoriteIndex(AppItem toFind)
{
    return favoriteApps.indexOf(toFind);
}

/**
 * @return A list of folders to display in the AppMenu.
 */
juce::Array<AppConfigFile::AppFolder> AppConfigFile::AppJson::getFolders()
{
    return categoryFolders;
}

/**
 * Add a new folder to the list of AppFolders in the config file.
 */
void AppConfigFile::AppJson::addAppFolder
(AppFolder newFolder, int index, bool writeChangesNow)
{
    categoryFolders.insert(index, newFolder);
    markPendingChanges();
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Remove a folder from the list of AppFolders.
 */
void AppConfigFile::AppJson::removeAppFolder(int index, bool writeChangesNow)
{
    int size = categoryFolders.size();
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
int AppConfigFile::AppJson::getFolderIndex(AppFolder toFind)
{
    return categoryFolders.indexOf(toFind);
}

AppConfigFile::AppJson::AppJson() : ConfigFile(filenameConst)
{
    using namespace juce;
    var jsonConfig = AssetFiles::loadJSONAsset
            (String(configPath) + filenameConst, true);
    var defaultConfig = var();
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}


/**
 * Read in this object's data from a json config object
 */
void AppConfigFile::AppJson::readDataFromJson
(juce::var& config, juce::var& defaultConfig)
{
    using namespace juce;
    ConfigFile::readDataFromJson(config, defaultConfig);
    //load favorites
    var favoriteList = getProperty(config, defaultConfig, favoritesKey);
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
    var categoryList = getProperty(config, defaultConfig, foldersKey);
    if (categoryList.isArray())
    {
        for (const var& folder : *categoryList.getArray())
        {
            AppFolder menuFolder = AppFolder(folder);
            if (!categoryFolders.contains(menuFolder))
            {
                categoryFolders.add(menuFolder);
            }
        }
    }
}

/**
 * Copy all config data to a json object.
 */
void AppConfigFile::AppJson::copyDataToJson(juce::DynamicObject::Ptr jsonObj)
{
    using namespace juce;
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
    jsonObj->setProperty(favoritesKey, favoriteArray);
    jsonObj->setProperty(foldersKey, categoryArray);
}
