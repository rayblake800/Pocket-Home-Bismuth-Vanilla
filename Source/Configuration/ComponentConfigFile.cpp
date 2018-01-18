/*
  ==============================================================================

    ComponentConfigFile.cpp
    Created: 5 Jan 2018 7:08:00pm
    Author:  anthony

  ==============================================================================
 */

#include "../Utils.h"
#include "ComponentConfigFile.h"

ComponentConfigFile::ComponentConfigFile() : ConfigFile(filenameConst)
{
    const ScopedLock readLock(lock);
    File configFile = File(getHomePath() + String(CONFIG_PATH) + filename);
    var jsonConfig = JSON::parse(configFile);
    var defaultConfig = var::null;
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

ComponentConfigFile::~ComponentConfigFile()
{
}

Array<String> ComponentConfigFile::getStringKeys() const
{
    return Array<String>();
}
//######################### Boolean Data ###################################
//boolean value keys
const String ComponentConfigFile::showClockKey = "show clock";
const String ComponentConfigFile::use24HrModeKey = "use 24h mode";

Array<String> ComponentConfigFile::getBoolKeys() const
{
    return {showClockKey, use24HrModeKey};
}

//######################### UI Component Data ##############################
//Defines all component types managed in the config file
const String ComponentConfigFile::appMenuButtonKey = "app menu buttons";
const String ComponentConfigFile::appMenuKey = "app menu";
const String ComponentConfigFile::menuFrameKey = "menu frame";
const String ComponentConfigFile::batteryIconKey = "battery";
const String ComponentConfigFile::batteryPercentKey = "battery percent text";
const String ComponentConfigFile::clockLabelKey = "time";
const String ComponentConfigFile::wifiIconKey = "wifi";
const String ComponentConfigFile::powerButtonKey = "power button";
const String ComponentConfigFile::settingsButtonKey = "settings button";
const String ComponentConfigFile::popupMenuKey = "popup menu";

ComponentConfigFile::ComponentSettings ComponentConfigFile::getComponentSettings
(String componentKey)
{
    const ScopedLock readLock(lock);
    return components[componentKey];
}

/**
 * Read in this object's data from a json config object
 */
void ComponentConfigFile::readDataFromJson(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    Array<String> keys = getComponentKeys();
    for (const String& key : keys)
    {
        var componentData = getProperty(config, defaultConfig, key);
        components[key] = ComponentSettings(componentData);
    }
}

/**
 * Copy all config data to a json object
 */
void ComponentConfigFile::copyDataToJson(DynamicObject::Ptr jsonObj)
{
    ConfigFile::copyDataToJson(jsonObj);
    Array<String> keys = getComponentKeys();
    for (const String& key : keys)
    {
        jsonObj->setProperty(key, components[key].getDynamicObject());
    }
}

/**
 * @return the list of all component keys.
 */
Array<String> ComponentConfigFile::getComponentKeys()
{
    return {appMenuButtonKey,
        appMenuKey,
        menuFrameKey,
        batteryIconKey,
        batteryPercentKey,
        clockLabelKey,
        wifiIconKey,
        powerButtonKey,
        settingsButtonKey,
        popupMenuKey};
}

ComponentConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0)
{
}

//copy constructor

ComponentConfigFile::ComponentSettings::ComponentSettings
(const ComponentConfigFile::ComponentSettings& copy) :
x(copy.x), y(copy.y),
width(copy.width), height(copy.height),
assetFiles(copy.assetFiles), colours(copy.colours)
{
}

ComponentConfigFile::ComponentSettings::ComponentSettings(var jsonObj)
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
                colours.add(Colour::fromString(colour.toString()));
            }
        }
    }
    var assetList = jsonObj["asset files"];
    if (assetList.isArray())
    {
        for (var asset : *assetList.getArray())
        {
            assetFiles.add(asset);
        }
    }
}

DynamicObject * ComponentConfigFile::ComponentSettings::getDynamicObject()
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

bool ComponentConfigFile::ComponentSettings::operator==
(const ComponentSettings& rhs) const
{
    return x == rhs.x &&
            y == rhs.y &&
            width == rhs.width &&
            height == rhs.height &&
            colours == rhs.colours &&
            assetFiles == rhs.assetFiles;
}

Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int>window = getWindowSize();
    return Rectangle<int>(x * window.getWidth(), y * window.getHeight(),
            width * window.getWidth(), height * window.getHeight());
}

Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

Array<String> ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

void ComponentConfigFile::ComponentSettings::applyBounds(Component * component)
{
    component->setBounds(getBounds());
}

void ComponentConfigFile::ComponentSettings::applySize(Component * component)
{
    Rectangle<int>screen = getWindowSize();
    component->setSize(width * screen.getWidth(), height * screen.getHeight());
}

