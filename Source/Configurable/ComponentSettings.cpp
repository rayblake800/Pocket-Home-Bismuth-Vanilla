#include "ComponentSettings.h"
#include "Utils.h"


ComponentSettings::ComponentSettings() :
x(0), y(0), width(0), height(0) { }

/*
 * Initializes the settings from JSON data.
 */
ComponentSettings::ComponentSettings(juce::DynamicObject* jsonObj)
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
 * Packages the component settings into a juce::DynamicObject.
 */
juce::DynamicObject * ComponentSettings::getDynamicObject() const
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

/*
 * Gets the component bounds defined by this object.
 */
juce::Rectangle<int> ComponentSettings::getBounds() const
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
 * Gets the list of configurable color values.
 */
const juce::Array<juce::Colour>& ComponentSettings::getColours() const
{
    return colours;
}

/*
 * Gets the list of configurable asset files.
 */
const juce::StringArray& ComponentSettings::getAssetFiles() const
{
    return assetFiles;
}


/*
 * Applies position and size settings to a UI component.
 */
void ComponentSettings::applyBounds(juce::Component * component) const
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
 * Gets this component's x-coordinate as a fraction of the window's width.
 */
float ComponentSettings::getXFraction() const
{
    return x;
}

/*
 * Gets this component's y-coordinate as a fraction of the window's height.
 */
float ComponentSettings::getYFraction() const
{
    return y;
}

/*
 * Gets this component's width as a fraction of the window's width.
 */
float ComponentSettings::getWidthFraction() const
{
    return width;
}

/*
 * Gets this component's height as a fraction of the window's height.
 */
float ComponentSettings::getHeightFraction() const
{
    return height;
}

/*
 * Compares these settings with another settings object.
 */
bool ComponentSettings::operator==(const ComponentSettings& rhs) const
{
    return x == rhs.x &&
            y == rhs.y &&
            width == rhs.width &&
            height == rhs.height &&
            colours == rhs.colours &&
            assetFiles == rhs.assetFiles;
}
