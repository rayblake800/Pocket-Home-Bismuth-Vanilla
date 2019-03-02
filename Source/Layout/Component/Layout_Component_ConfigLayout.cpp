#include "Layout_Component_ConfigLayout.h"
#include "Utils.h"

namespace ComponentLayout = Layout::Component;

ComponentLayout::ConfigLayout::ConfigLayout() :
x(0), y(0), width(0), height(0) { }

/*
 * Initializes the layout from JSON data.
 */
ComponentLayout::ConfigLayout::ConfigLayout(juce::DynamicObject* jsonObj)
{
    x = jsonObj->hasProperty("x") ?
	   double(jsonObj->getProperty("x")) : -1.0;
    y = jsonObj->hasProperty("y") ?
	   double(jsonObj->getProperty("y")) : -1.0;
    width = jsonObj->hasProperty("width") ?
	   double(jsonObj->getProperty("width")) : -1.0;
    height = jsonObj->hasProperty("height") ?
	   double(jsonObj->getProperty("height")) : -1.0;

}

/*
 * Packages the layout into a juce::DynamicObject.
 */
juce::DynamicObject* ComponentLayout::ConfigLayout::getDynamicObject() const
{
    juce::DynamicObject* componentObject = new juce::DynamicObject();
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
    return componentObject;
}

/*
 * Gets the component bounds defined by this object.
 */
juce::Rectangle<int> ComponentLayout::ConfigLayout::getBounds() const
{
    using juce::Rectangle;
    Rectangle<int> window = getWindowBounds();
    return Rectangle<int>(
            x * window.getWidth(),
            y * window.getHeight(),
            std::max<int>(0, width * window.getWidth()),
            std::max<int>(0, height * window.getHeight()));
}

/*
 * Applies position and size settings to a UI component.
 */
void ComponentLayout::ConfigLayout::applyBounds
(juce::Component * component) const
{
    juce::Rectangle<int> bounds = getBounds();
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
 * Gets the component's x-coordinate as a fraction of the window's width.
 */
float ComponentLayout::ConfigLayout::getXFraction() const
{
    return x;
}

/*
 * Gets the component's y-coordinate as a fraction of the window's height.
 */
float ComponentLayout::ConfigLayout::getYFraction() const
{
    return y;
}

/*
 * Gets the component's width as a fraction of the window's width.
 */
float ComponentLayout::ConfigLayout::getWidthFraction() const
{
    return width;
}

/*
 * Gets the component's height as a fraction of the window's height.
 */
float ComponentLayout::ConfigLayout::getHeightFraction() const
{
    return height;
}

/*
 * Compares this layout with another layout object.
 */
bool ComponentLayout::ConfigLayout::operator==(const ConfigLayout& rhs) const
{
    return x == rhs.x &&
           y == rhs.y &&
           width == rhs.width &&
           height == rhs.height;
}
