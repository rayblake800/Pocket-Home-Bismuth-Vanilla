/*
  ==============================================================================

    ConfigFile.cpp
    Created: 26 Dec 2017 10:31:17am
    Author:  anthony
  ==============================================================================
 */
/**
 * Implementation reminder: Every method that gets data,sets data, or does file 
 * IO should use the lock. Don't forget to unlock before calling one locking
 * method within another!  
 */


#include "../Utils.h"
#include "ConfigFile.h"

//Assign keymaps
const std::map<String, ComponentType> ConfigFile::componentKeys =
        ConfigFile::setComponentKeys();
const std::map<ConfigString, String> ConfigFile::stringKeys =
        ConfigFile::setStringKeys();
const std::map<ConfigBool, String> ConfigFile::boolKeys =
        ConfigFile::setBoolKeys();

ConfigFile::~ConfigFile() {
}

ConfigFile* ConfigFile::getInstance() {
    static ConfigFile instance;
    return &instance;
}

//############################ String Data #####################################

String ConfigFile::getConfigString(ConfigString configString) {
    const ScopedLock readLock(lock);
    try {
        String key = stringKeys.at(configString);
        return stringValues[key];
    } catch (std::out_of_range e) {
        DBG("getConfigString:missing key value!");
        return "";
    }
}

void ConfigFile::setConfigString(ConfigString configString,
        String newValue) {
    const ScopedLock writeLock(lock);
    try {
        String key = stringKeys.at(configString);
        if (stringValues[key] != newValue) {
            stringValues[key] = newValue;
            const ScopedUnlock writeUnlock(lock);
            writeChanges();
        }
    } catch (std::out_of_range e) {
        DBG("setConfigString:missing key value!");
    }
}

//############################ Boolean Data ####################################

bool ConfigFile::getConfigBool(ConfigBool configBool) {
    const ScopedLock readLock(lock);
    try {
        String key = boolKeys.at(configBool);
        return boolValues[key];
    } catch (std::out_of_range e) {
        DBG("setConfigBool:missing key value!");
        return false;
    }

}

void ConfigFile::setConfigBool(ConfigBool configBool,
        bool newValue) {
    const ScopedLock writeLock(lock);
    try {
        String key = boolKeys.at(configBool);
        if (boolValues[key] != newValue) {
            boolValues[key] = newValue;
            const ScopedUnlock writeUnlock(lock);
            writeChanges();
        }
    } catch (std::out_of_range e) {
        DBG("setConfigBool:missing key value!");
    }
}

//######################### Application Data ###############################

ConfigFile::AppItem::AppItem() {
}

ConfigFile::AppItem::AppItem(var jsonObj) {
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");
    shell = jsonObj.getProperty("shell", "");
}

DynamicObject* ConfigFile::AppItem::getDynamicObject() {
    DynamicObject * appObject = new DynamicObject();
    appObject->setProperty("name", name);
    appObject->setProperty("icon", icon);
    appObject->setProperty("shell", shell);
    return appObject;
}

bool ConfigFile::AppItem::operator==(const AppItem& rhs) const{
    return icon == rhs.icon &&
            name == rhs.name &&
            shell == rhs.shell;
}

std::vector<ConfigFile::AppItem> ConfigFile::getFavorites() {
    const ScopedLock readLock(lock);
    return favoriteApps;
}

void ConfigFile::setFavorites(std::vector<AppItem> newFavorites) {
    const ScopedLock writeLock(lock);
    favoriteApps = newFavorites;
    {
        const ScopedUnlock writeUnlock(lock);
        writeChanges();
    }
}

//######################### Folder/Category Data ###########################

ConfigFile::AppFolder::AppFolder() {
}

ConfigFile::AppFolder::AppFolder(var jsonObj) {
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");

    categories.clear();
    var catList = jsonObj["categories"];
    if (catList.isArray()) {
        for (var category : *catList.getArray()) {
            categories.push_back(category);
        }
    }

    pinnedApps.clear();
    var appList = jsonObj["pinned apps"];
    if (appList.isArray()) {
        for (var app : *appList.getArray()) {
            pinnedApps.push_back(AppItem(app));
        }
    }
}

DynamicObject* ConfigFile::AppFolder::getDynamicObject() {
    DynamicObject * folderObject = new DynamicObject();
    folderObject->setProperty("name", name);
    folderObject->setProperty("icon", icon);

    Array<var> categoryArray;
    for (int i = 0; i < categories.size(); i++) {
        categoryArray.add(categories[i]);
    }
    folderObject->setProperty(String("categories"), categoryArray);

    Array<var> appArray;
    for (int i = 0; i < pinnedApps.size(); i++) {
        appArray.add(var(pinnedApps[i].getDynamicObject()));
    }
    folderObject->setProperty(String("pinned apps"), appArray);
    return folderObject;
}

bool ConfigFile::AppFolder::operator==(const AppFolder& rhs) const{
    return name == rhs.name &&
            icon == rhs.icon &&
            categories == rhs.categories &&
            pinnedApps == rhs.pinnedApps;
}

std::vector<ConfigFile::AppFolder> ConfigFile::getFolders() {
    const ScopedLock readLock(lock);
    return categoryFolders;
}

//########################### UI Component Data ################################

ConfigFile::ComponentSettings::ComponentSettings() : x(0), y(0), width(0), height(0) {
}

ConfigFile::ComponentSettings::ComponentSettings(var jsonObj) {
    x = jsonObj.getProperty("x", 0);
    y = jsonObj.getProperty("y", 0);
    width = jsonObj.getProperty("width", 0);
    height = jsonObj.getProperty("height", 0);

    var colourList = jsonObj["colours"];
    if (colourList.isArray()) {
        for (var colour : *colourList.getArray()) {
            if (colour.isString()) {
                colours.push_back(Colour::fromString(colour.toString()));
            }
        }
    }
    var assetList = jsonObj["asset files"];
    if (assetList.isArray()) {
        for (var asset : *assetList.getArray()) {
            assetFiles.push_back(asset);
        }
    }
}

DynamicObject * ConfigFile::ComponentSettings::getDynamicObject() {
    DynamicObject * componentObject = new DynamicObject();
    componentObject->setProperty("x", x);
    componentObject->setProperty("y", y);
    componentObject->setProperty("width", width);
    componentObject->setProperty("height", height);
    Array<var> coloursListed;
    for (int i = 0; i < colours.size(); i++) {
        coloursListed.add(var(colours[i].toString()));
    }
    componentObject->setProperty("colours", coloursListed);

    Array<var> assetFilesListed;
    for (int i = 0; i < assetFiles.size(); i++) {
        assetFilesListed.add(var(assetFiles[i]));
    }
    componentObject->setProperty("asset files", assetFilesListed);
    return componentObject;
}

bool ConfigFile::ComponentSettings::operator==(const ComponentSettings& rhs) const{
    return x==rhs.x &&
            y==rhs.y &&
            width==rhs.width &&
            height==rhs.height &&
            colours==rhs.colours &&
            assetFiles==rhs.assetFiles;
}

Rectangle<int> ConfigFile::ComponentSettings::getBounds() {
    Rectangle<int>window = getWindowSize();
    return Rectangle<int>(x * window.getWidth(), y * window.getHeight(),
            width * window.getWidth(), height * window.getHeight());
}

std::vector<Colour> ConfigFile::ComponentSettings::getColours() {
    return colours;
}

std::vector<String> ConfigFile::ComponentSettings::getAssetFiles() {
    return assetFiles;
}

void ConfigFile::ComponentSettings::applyBounds(Component * component) {
    component->setBounds(getBounds());
}

void ConfigFile::ComponentSettings::applySize(Component * component) {
    Rectangle<int>screen = getWindowSize();
    component->setSize(width * screen.getWidth(), height * screen.getHeight());
}

ConfigFile::ComponentSettings ConfigFile::getComponentSettings(ComponentType componentType) {
    const ScopedLock readLock(lock);
    return components[componentType];
}

//################################# File IO ####################################

ConfigFile::ConfigFile() {
    bool madeChanges = false;
    const ScopedLock readLock(lock);

    File configFile = File(getHomePath() + String(CONFIG_PATH));
    if (!configFile.exists()) {
        configFile.create();
    }
    var jsonConfig = JSON::parse(configFile);

    //checks if a var member exists
    std::function<bool(var, String) > exists = [](var json, String key)->bool {
        return json.getProperty(key, var::null) != var::null;
    };

    //gets a var member from config if it exists, or from the default config
    //file otherwise.
    var defaultConfig = var::null;
    std::function < var(String) > getMainProperty =
            [exists, &jsonConfig, &defaultConfig, &madeChanges](String key)->var {
                if (exists(jsonConfig, key)) {
                    return jsonConfig.getProperty(key, "");
                } else {
                    if (defaultConfig == var::null) {
                        defaultConfig = JSON::parse(assetFile("config.json"));
                    }
                    madeChanges = true;
                    return defaultConfig.getProperty(key, "");
                }
            };


    //load strings
    for (std::map<ConfigString, String>::const_iterator it =
            stringKeys.begin();
            it != stringKeys.end(); it++) {
        String key = it->second;
        //        if (!(jsonConfig->getProperty(key).isString())) {
        //            jsonConfig->setProperty(key, var::null);
        //        }
        stringValues[key] = getMainProperty(key).toString();
    }

    //load bools
    for (std::map<ConfigBool, String>::const_iterator it =
            boolKeys.begin();
            it != boolKeys.end(); it++) {
        String key = it->second;
        //        if (!jsonConfig->getProperty(key).isBool()) {
        //            jsonConfig->setProperty(key, var::null);
        //        }
        boolValues[key] = getMainProperty(key);
    }

    //load favorites
    var favoriteList;
    if (exists(jsonConfig, "pages")) {
        DBG("backing up and extracting data from marshmallow config file");
        //found a Marshmallow Pocket-Home config file, update it.
        File legacyBackup = File(getHomePath() + "/.pocket-home/marshmallowConfig.json");
        legacyBackup.create();
        legacyBackup.replaceWithText(JSON::toString(jsonConfig));
        var pages = jsonConfig.getProperty("pages", Array<var>());
        for (const var& page : *pages.getArray()) {
            if (page["name"] == "Apps" && page["items"].isArray()) {
                favoriteList = page["items"];
                madeChanges = true;
                break;
            }
        }
    } else {//load favorites normally
        favoriteList = getMainProperty(FAVORITES_KEY);
    }
    if (favoriteList.isArray()) {
        for (const var& app : *favoriteList.getArray()) {
            favoriteApps.push_back(AppItem(app));
        }
    }

    //load categories
    var categoryList = getMainProperty(FOLDERS_KEY);
    if (categoryList.isArray()) {
        for (const var& folder : *categoryList.getArray()) {
            categoryFolders.push_back(AppFolder(folder));
        }
    }

    //load component settings
    for (std::map<String, ComponentType>::const_iterator it = componentKeys.begin();
            it != componentKeys.end(); it++) {
        var componentData = getMainProperty(it->first);
        components[it->second] = ComponentSettings(componentData);
    }
    if (madeChanges) {
        //writeChanges will want the lock, so it must be unlocked now
        const ScopedUnlock unlock(lock);
        writeChanges();
    }
}

void ConfigFile::writeChanges() {
    const ScopedLock writeLock(lock);
    DynamicObject * jsonBuilder = new DynamicObject();

    //set strings
    for (std::map<ConfigString, String>::const_iterator it =
            stringKeys.begin();
            it != stringKeys.end(); it++) {
        String key = it->second;
        jsonBuilder->setProperty(key, stringValues[key]);
    }

    //set bools
    for (std::map<ConfigBool, String>::const_iterator it =
            boolKeys.begin();
            it != boolKeys.end(); it++) {
        String key = it->second;
        jsonBuilder->setProperty(key, boolValues[key]);
    }

    //set favorites
    Array<var> favoriteArray;
    for (int i = 0; i < favoriteApps.size(); i++) {
        favoriteArray.add(var(favoriteApps[i].getDynamicObject()));
    }

    //set folders
    Array<var> categoryArray;
    for (int i = 0; i < categoryFolders.size(); i++) {
        categoryArray.add(var(categoryFolders[i].getDynamicObject()));
    }
    jsonBuilder->setProperty(FAVORITES_KEY, favoriteArray);
    jsonBuilder->setProperty(FOLDERS_KEY, categoryArray);

    //set components
    for (std::map<String, ComponentType>::const_iterator it = componentKeys.begin();
            it != componentKeys.end(); it++) {
        String key = it->first;
        jsonBuilder->setProperty(key, components[it->second].getDynamicObject());
    }

    //convert to JSON string, write to config.json
    String jsonText = JSON::toString(jsonBuilder);
    File configFile = File(getHomePath() + String(CONFIG_PATH));
    if (!configFile.exists()) {
        configFile.create();
    }
    if (!configFile.replaceWithText(jsonText)) {
        String message = "Error writing the configuration into the file\nCheck the permissions";
        AlertWindow::showMessageBox(
                AlertWindow::AlertIconType::WarningIcon,
                "Error",
                message);
    }
}



