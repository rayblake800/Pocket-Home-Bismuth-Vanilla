#include "ComponentConfigFile.h"
#include "AssetFiles.h"
#include "Utils.h"

juce::ScopedPointer<ResourceManager::SharedResource>
        ComponentConfigFile::sharedResource = nullptr;

juce::ReadWriteLock ComponentConfigFile::configLock;

ComponentConfigFile::ComponentConfigFile() :
ResourceManager(sharedResource, configLock,
[this]()->ResourceManager::SharedResource*
{
    return new ConfigJson();
}) 
{
    static bool first = true;
    if(!getWindowBounds().isEmpty() && first)
    {
        DBG("LargeText = " << getFontHeight(largeText));
        DBG("MediumText = " << getFontHeight(mediumText));
        DBG("SmallText = " << getFontHeight(smallText));
        first = false;
    }

}

/*
 * Gets configured component settings from shared .json file data.
 */
ComponentConfigFile::ComponentSettings
ComponentConfigFile::getComponentSettings(juce::String componentKey)
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    return config->getComponentSettings(componentKey);
}

/*
 * Add a listener to track component setting changes.
 */
void ComponentConfigFile::addListener(ConfigFile::Listener* listener,
        juce::StringArray trackedKeys)
{
    using namespace juce;
    const ScopedWriteLock writeLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    config->addListener(listener, trackedKeys);
}

/*
 * Return the most appropriate font size for drawing text
 */
int ComponentConfigFile::getFontHeight
(juce::Rectangle <int> textBounds, juce::String text)
{
    using namespace juce;
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());

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
    int testHeight = getFontHeight(largeText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(mediumText);
    if (height > testHeight)
    {
        return testHeight;
    }
    testHeight = getFontHeight(smallText);
    if (height > testHeight)
    {
        return testHeight;
    }
    return height;
}

/**
 * Gets the height in pixels of one of the three configured text sizes.
 */
int ComponentConfigFile::getFontHeight(TextSize sizeType)
{
    using namespace juce;
    String key;
    switch(sizeType)
    {
    case smallText:
        key = smallTextKey;
        break;
    case mediumText:
        key = mediumTextKey;
        break;
    case largeText:
        key = largeTextKey;
    }
    const ScopedReadLock readLock(configLock);
    ConfigJson* config = static_cast<ConfigJson*> (sharedResource.get());
    double size = config->getConfigValue<double>(key);
    if(size > 1)
    {
        return (int) size;
    }
    return (int) (size * getWindowBounds().getHeight());
}

/*
 * Gets the list of all configurable component keys.
 */
const juce::StringArray& ComponentConfigFile::getComponentKeys()
{
    static const juce::StringArray keys =
    {
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
        settingsListBtnKey,
        spinnerKey,
    };
    return keys;
}

ComponentConfigFile::ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0) { }

/*
 * Initializes from json data.
 */
ComponentConfigFile::ComponentSettings::ComponentSettings
(juce::DynamicObject* jsonObj)
{
    using namespace juce;
    x = jsonObj->hasProperty("x") ?
	   double(jsonObj->getProperty("x")) : -1.0;
    y = jsonObj->hasProperty("y") ?
	   double(jsonObj->getProperty("y")) : -1.0;
    width = jsonObj->hasProperty("width") ?
	   double(jsonObj->getProperty("width")) : -1.0;
    height = jsonObj->hasProperty("height") ?
	   double(jsonObj->getProperty("height")) : -1.0;

    var colourList = jsonObj->getProperty("colours");
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
    
    var assetList = jsonObj->getProperty("asset files");
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
juce::DynamicObject * ComponentConfigFile::ComponentSettings::getDynamicObject()
{
    using namespace juce;
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
juce::Rectangle<int> ComponentConfigFile::ComponentSettings::getBounds()
{
    using namespace juce;
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
juce::Array<juce::Colour> ComponentConfigFile::ComponentSettings::getColours()
{
    return colours;
}

/*
 * @return the list of component asset files.
 */
juce::StringArray ComponentConfigFile::ComponentSettings::getAssetFiles()
{
    return assetFiles;
}

/*
 * Use these settings to position and size a component.
 */
void ComponentConfigFile::ComponentSettings::applyBounds
(juce::Component * component)
{
    using namespace juce;
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


