/*
  ==============================================================================

    MainConfigFile.cpp
    Created: 5 Jan 2018 2:35:02pm
    Author:  anthony

  ==============================================================================
 */

#include "MainConfigFile.h"
#include "../Utils.h"
//Assign keymaps
const std::map<String, ComponentType> MainConfigFile::componentKeys =
        MainConfigFile::setComponentKeys();

MainConfigFile::MainConfigFile() : ConfigFile(filenameConst)
{
    const ScopedLock readLock(lock);
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    var jsonConfig = JSON::parse(configFile);
    var defaultConfig = var::null;
    readDataFromJson(jsonConfig, defaultConfig);

}

MainConfigFile::~MainConfigFile()
{
}

//############################ String Data #####################################
const String MainConfigFile::backgroundKey = "background";
const String MainConfigFile::shutdownCommandKey = "shutdown command";
const String MainConfigFile::restartCommandKey = "restart command";
const String MainConfigFile::termLaunchCommandKey = "terminal launch command";

Array<String> MainConfigFile::getStringKeys() const
{
    return {backgroundKey,
        shutdownCommandKey,
        restartCommandKey,
        termLaunchCommandKey};
}
//############################ Boolean Data ####################################
const String MainConfigFile::showCursorKey = "cursor";
const String MainConfigFile::showClockKey = "show clock";
const String MainConfigFile::use24HrModeKey = "use 24h mode";

Array<String> MainConfigFile::getBoolKeys() const
{
    return {showCursorKey,
        showClockKey,
        use24HrModeKey};
}

//########################### Application Data #################################

MainConfigFile::AppItem::AppItem()
{
}

MainConfigFile::AppItem::AppItem(var jsonObj)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");
    shell = jsonObj.getProperty("shell", "");
}

DynamicObject* MainConfigFile::AppItem::getDynamicObject()
{
    DynamicObject * appObject = new DynamicObject();
    appObject->setProperty("name", name);
    appObject->setProperty("icon", icon);
    appObject->setProperty("shell", shell);
    return appObject;
}

bool MainConfigFile::AppItem::operator==(const AppItem& rhs) const
{
    return icon == rhs.icon &&
            name == rhs.name &&
            shell == rhs.shell;
}

std::vector<MainConfigFile::AppItem> MainConfigFile::getFavorites()
{
    const ScopedLock readLock(lock);
    return favoriteApps;
}

void MainConfigFile::setFavorites(std::vector<AppItem> newFavorites)
{
    const ScopedLock writeLock(lock);
    favoriteApps = newFavorites;
    {
        const ScopedUnlock writeUnlock(lock);
        writeChanges();
    }
}

//######################### Folder/Category Data ###############################

MainConfigFile::AppFolder::AppFolder()
{
}

MainConfigFile::AppFolder::AppFolder(var jsonObj)
{
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");

    categories.clear();
    var catList = jsonObj["categories"];
    if (catList.isArray())
    {
        for (var category : *catList.getArray())
        {
            categories.push_back(category);
        }
    }

    pinnedApps.clear();
    var appList = jsonObj["pinned apps"];
    if (appList.isArray())
    {
        for (var app : *appList.getArray())
        {
            pinnedApps.push_back(AppItem(app));
        }
    }
}

DynamicObject* MainConfigFile::AppFolder::getDynamicObject()
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

bool MainConfigFile::AppFolder::operator==(const AppFolder& rhs) const
{
    return name == rhs.name &&
            icon == rhs.icon &&
            categories == rhs.categories &&
            pinnedApps == rhs.pinnedApps;
}

std::vector<MainConfigFile::AppFolder> MainConfigFile::getFolders()
{
    const ScopedLock readLock(lock);
    return categoryFolders;
}

//########################### UI Component Data ################################

MainConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0)
{
}

MainConfigFile::ComponentSettings::ComponentSettings(var jsonObj)
{
    x = jsonObj.getProperty("x", 0);
    y = jsonObj.getProperty("y", 0);
    width = jsonObj.getProperty("width", 0);
    height = jsonObj.getProperty("height", 0);

    var colourList = jsonObj["colours"];
    if (colourList.isArray())
    {
        for (var colour : *colourList.getArray())
        {
            if (colour.isString())
            {
                colours.push_back(Colour::fromString(colour.toString()));
            }
        }
    }
    var assetList = jsonObj["asset files"];
    if (assetList.isArray())
    {
        for (var asset : *assetList.getArray())
        {
            assetFiles.push_back(asset);
        }
    }
}

DynamicObject * MainConfigFile::ComponentSettings::getDynamicObject()
{
    DynamicObject * componentObject = new DynamicObject();
    componentObject->setProperty("x", x);
    componentObject->setProperty("y", y);
    componentObject->setProperty("width", width);
    componentObject->setProperty("height", height);
    Array<var> coloursListed;
    for (int i = 0; i < colours.size(); i++)
    {
        coloursListed.add(var(colours[i].toString()));
    }
    componentObject->setProperty("colours", coloursListed);

    Array<var> assetFilesListed;
    for (int i = 0; i < assetFiles.size(); i++)
    {
        assetFilesListed.add(var(assetFiles[i]));
    }
    componentObject->setProperty("asset files", assetFilesListed);
    return componentObject;
}

bool MainConfigFile::ComponentSettings::operator==
(const ComponentSettings& rhs) const
{
    return x == rhs.x &&
            y == rhs.y &&
            width == rhs.width &&
            height == rhs.height &&
            colours == rhs.colours &&
            assetFiles == rhs.assetFiles;
}

Rectangle<int> MainConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int>window = getWindowSize();
    return Rectangle<int>(x * window.getWidth(), y * window.getHeight(),
            width * window.getWidth(), height * window.getHeight());
}

std::vector<Colour> MainConfigFile::ComponentSettings::getColours()
{
    return colours;
}

std::vector<String> MainConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

void MainConfigFile::ComponentSettings::applyBounds(Component * component)
{
    component->setBounds(getBounds());
}

void MainConfigFile::ComponentSettings::applySize(Component * component)
{
    Rectangle<int>screen = getWindowSize();
    component->setSize(width * screen.getWidth(), height * screen.getHeight());
}

MainConfigFile::ComponentSettings MainConfigFile::getComponentSettings
(ComponentType componentType)
{
    const ScopedLock readLock(lock);
    return components[componentType];
}

//################################# File IO ####################################

/**
 * Read in this object's data from a json config object
 */
void MainConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    //load favorites
    var favoriteList;
    if (propertyExists(config, "pages"))
    {
        DBG("backing up and extracting data from marshmallow config file");
        //found a Marshmallow Pocket-Home config file, update it.
        File legacyBackup = File(getHomePath()
                + "/.pocket-home/marshmallowConfig.json");
        legacyBackup.create();
        legacyBackup.replaceWithText(JSON::toString(config));
        var pages = config.getProperty("pages", Array<var>());
        for (const var& page : *pages.getArray())
        {
            if (page["name"] == "Apps" && page["items"].isArray())
            {
                favoriteList = page["items"];
                changesPending = true;
                break;
            }
        }
    } else
    {//load favorites normally
        favoriteList = getProperty(config, defaultConfig, FAVORITES_KEY);
    }
    if (favoriteList.isArray())
    {
        for (const var& app : *favoriteList.getArray())
        {
            favoriteApps.push_back(AppItem(app));
        }
    }
    //load categories
    var categoryList = getProperty(config, defaultConfig, FOLDERS_KEY);
    if (categoryList.isArray())
    {
        for (const var& folder : *categoryList.getArray())
        {
            categoryFolders.push_back(AppFolder(folder));
        }
    }

    //load component settings
    for (std::map<String, ComponentType>::const_iterator it =
            componentKeys.begin();
            it != componentKeys.end(); it++)
    {
        var componentData = getProperty(config, defaultConfig, it->first);
        components[it->second] = ComponentSettings(componentData);
    }
}

/**
 * Copy all config data to a json object
 */
void MainConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
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

    //set components
    for (std::map<String, ComponentType>::const_iterator it =
            componentKeys.begin();
            it != componentKeys.end(); it++)
    {
        String key = it->first;
        jsonObj->setProperty(key, components[it->second].getDynamicObject());
    }
}