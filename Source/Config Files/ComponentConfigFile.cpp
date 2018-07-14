#include "ComponentConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

ScopedPointer<ResourceManager::SharedResource>
        ComponentConfigFile::sharedResource = nullptr;

ReadWriteLock ComponentConfigFile::configLock;

ComponentConfigFile::ComponentConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{
    return new ConfigJson();
}) { }

/*
 * Gets configured component settings from shared .json file data.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::getComponentSettings(String componentKey)
{
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getComponentSettings(componentKey);
}

/*
 * Add a listener to track component setting changes.
 */
void ComponentConfigFile::addListener(ConfigFile::Listener* listener,
        StringArray trackedKeys)
{
    const ScopedWriteLock writeLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->addListener(listener, trackedKeys);
}

//######################### UI Component Data ##############################
//Defines all component types managed in the config file
const String ComponentConfigFile::scrollingAppMenuKey = "scrolling app menu";
const String ComponentConfigFile::pagedAppMenuKey = "paged app menu";
const String ComponentConfigFile::menuFrameKey = "menu frame";
const String ComponentConfigFile::batteryIconKey = "battery";
const String ComponentConfigFile::batteryPercentKey = "battery percent text";
const String ComponentConfigFile::clockLabelKey = "time";
const String ComponentConfigFile::wifiIconKey = "wifi";
const String ComponentConfigFile::powerButtonKey = "power button";
const String ComponentConfigFile::settingsButtonKey = "settings button";
const String ComponentConfigFile::popupMenuKey = "popup menu";
const String ComponentConfigFile::pageLeftKey = "left arrow button";
const String ComponentConfigFile::pageRightKey = "right arrow button";
const String ComponentConfigFile::pageUpKey = "up arrow button";
const String ComponentConfigFile::pageDownKey = "down arrow button";
const String ComponentConfigFile::spinnerKey = "loading spinner";
const String ComponentConfigFile::smallTextKey = "small text";
const String ComponentConfigFile::mediumTextKey = "medium text";
const String ComponentConfigFile::largeTextKey = "large text";

/*
 * Return the most appropriate font size for drawing text
 */
int ComponentConfigFile::getFontHeight(Rectangle <int> textBounds, String text)
{
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    int heightLarge = config->getComponentSettings(largeTextKey).getBounds()
            .getHeight();
    int heightMedium = config->getComponentSettings(mediumTextKey).getBounds()
            .getHeight();
    int heightSmall = config->getComponentSettings(smallTextKey).getBounds()
            .getHeight();
    int numLines = 1;
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            numLines++;
        }
    }

    int height = textBounds.getHeight() / numLines;
    Font defaultFont(height);
    int width = defaultFont.getStringWidth(text);
    if (width > textBounds.getWidth())
    {
        height = textBounds.getWidth() * height / width;
    }
    if (height > heightLarge)
    {
        return heightLarge;
    }
    else if (height > heightMedium)
    {
        return heightMedium;
    }
    else if (height > heightSmall)
    {
        return heightSmall;
    }
    return height;
}

/*
 * @return the list of all component keys.
 */
StringArray ComponentConfigFile::getComponentKeys()
{
    return {
            scrollingAppMenuKey,
            pagedAppMenuKey,
            menuFrameKey,
            batteryIconKey,
            batteryPercentKey,
            clockLabelKey,
            wifiIconKey,
            powerButtonKey,
            settingsButtonKey,
            popupMenuKey,
            pageLeftKey,
            pageRightKey,
            pageUpKey,
            pageDownKey,
            spinnerKey,
            smallTextKey,
            mediumTextKey,
            largeTextKey};
}

ComponentConfigFile::ConfigJson::ConfigJson() : ConfigFile(filenameConst)
{
    var jsonConfig = AssetFiles::loadJSONAsset(String(configPath)
            + filenameConst, true);
    var defaultConfig = var();
    readDataFromJson(jsonConfig, defaultConfig);
    writeChanges();
}

/*
 * Gets the configured settings for a particular component.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::ConfigJson::getComponentSettings(String componentKey)
{
    return components[componentKey];
}

/*
 * @return the list of key Strings for each integer value tracked in 
 * components.json
 */
std::vector<ConfigFile::DataKey> ComponentConfigFile::ConfigJson
::getDataKeys() const
{
    std::vector<DataKey> keys = {
    };
    return keys;
}

/*
 * Read in this object's data from a json config object
 */
void ComponentConfigFile::ConfigJson::readDataFromJson
(var& config, var& defaultConfig)
{
    ConfigFile::readDataFromJson(config, defaultConfig);
    StringArray keys = getComponentKeys();
    for (const String& key : keys)
    {
        var componentData = getProperty(config, defaultConfig, key);
        components[key] = ComponentSettings(componentData);
    }
}

/*
 * Copy all config data to a json object
 */
void ComponentConfigFile::ConfigJson::copyDataToJson
(DynamicObject::Ptr jsonObj)
{
    ConfigFile::copyDataToJson(jsonObj);
    StringArray keys = getComponentKeys();
    for (const String& key : keys)
    {
        jsonObj->setProperty(key, components[key].getDynamicObject());
    }
}

ComponentConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0) { }

/*
 * Initializes from json data.
 */
ComponentConfigFile::ComponentSettings::ComponentSettings(var jsonObj)
{
    x = jsonObj.getProperty("x", -1);
    y = jsonObj.getProperty("y", -1);
    width = jsonObj.getProperty("width", -1);
    height = jsonObj.getProperty("height", -1);

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

/*
 * Packages the object into a DynamicObject that can be written to a
 * json file.
 */
DynamicObject * ComponentConfigFile::ComponentSettings::getDynamicObject()
{
    DynamicObject * componentObject = new DynamicObject();
    if (x != -1)
    {
        componentObject->setProperty("x", x);
    }
    if (y != -1)
    {
        componentObject->setProperty("y", y);
    }
    if (width != -1)
    {
        componentObject->setProperty("width", width);
    }
    if (height != -1)
    {
        componentObject->setProperty("height", height);
    }
    if (colours.size() > 0)
    {
        Array<var> coloursListed;
        for (int i = 0; i < colours.size(); i++)
        {
            coloursListed.add(var(colours[i].toString()));
        }
        componentObject->setProperty("colours", coloursListed);
    }

    if (assetFiles.size() > 0)
    {
        Array<var> assetFilesListed;
        for (int i = 0; i < assetFiles.size(); i++)
        {
            assetFilesListed.add(var(assetFiles[i]));
        }
        componentObject->setProperty("asset files", assetFilesListed);
    }
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

/*
 * @return the bounds of the component relative to the window,
 * measured in pixels.
 */
Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    Rectangle<int> window = getWindowBounds();
    return Rectangle<int>(
                          x * window.getWidth(),
                          y * window.getHeight(),
                          width * window.getWidth(),
                          height * window.getHeight());
}

/*
 * @return the list of configurable colors.
 */
Array<Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

/*
 * @return the list of component asset files.
 */
StringArray ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

/*
 * Use these settings to position and size a component.
 */
void ComponentConfigFile::ComponentSettings::applyBounds(Component * component)
{
    Rectangle<int> bounds = getBounds();
    if (x == -1)
    {
        bounds.setX(component->getX());
    }
    if (y == -1)
    {
        bounds.setY(component->getY());
    }
    if (width == -1)
    {
        bounds.setWidth(component->getWidth());
    }
    if (height == -1)
    {
        bounds.setHeight(component->getHeight());
    }
    component->setBounds(bounds);
}
        
/*
 * Gets this component's x-coordinate as a fraction of the window's
 * width.
 */
float ComponentConfigFile::ComponentSettings::getXFraction()
{
    return x;
}

/*
 * Gets this component's y-coordinate as a fraction of the window's
 * height.
 */
float ComponentConfigFile::ComponentSettings::getYFraction()
{
    return y;
}

/*
 * Gets this component's width as a fraction of the window's width.
 */
float ComponentConfigFile::ComponentSettings::getWidthFraction()
{
    return width;
}

/*
 * Gets this component's height as a fraction of the window's height.
 */
float ComponentConfigFile::ComponentSettings::getHeightFraction()
{
    return height;
}


