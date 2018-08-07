#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent(const juce::String& componentKey) :
componentKey(componentKey)
{ 
    ComponentConfigFile config;
    componentSettings = config.getComponentSettings(componentKey);
    config.addListener(this,{componentKey});
}

/*
 * Load and apply this component's relative bounds from config.
 */
void ConfigurableComponent::applyConfigBounds()
{
    using namespace juce;
    Component * component = dynamic_cast<Component*> (this);

    if (component == nullptr)
    {
        DBG("ConfigurableComponent::" << __func__ 
                << ": this object is not a component!");
        return;
    }

    Rectangle<int> newBounds = componentSettings.getBounds();
    if (newBounds.getX() < 0)
    {
        newBounds.setX(component->getX());
    }
    if (newBounds.getY() < 0)
    {
        newBounds.setY(component->getY());
    }
    if (newBounds.getWidth() < 0)
    {
        newBounds.setWidth(component->getWidth());
    }
    if (newBounds.getHeight() < 0)
    {
        newBounds.setHeight(component->getHeight());
    }
    component->setBounds(newBounds);
}

/*
 * Gets the componentKey that defines this component's bounds and asset
 * files
 */
const juce::String& ConfigurableComponent::getComponentKey() const
{
    return componentKey;
}

/*
 * Gets this component's x-coordinate as a fraction of the window's
 * width.
 */
float ConfigurableComponent::getXFraction()
{
    return componentSettings.getXFraction();
}

/*
 * Gets this component's y-coordinate as a fraction of the window's
 * height.
 */
float ConfigurableComponent::getYFraction()
{
    return componentSettings.getYFraction();
}

/*
 * Gets this component's width as a fraction of the window's width.
 */
float ConfigurableComponent::getWidthFraction()
{
    return componentSettings.getWidthFraction();
}

/*
 * Gets this component's height as a fraction of the window's height.
 */
float ConfigurableComponent::getHeightFraction()
{
    return componentSettings.getHeightFraction();
}


/**
 * Load and apply all component data from the ComponentConfigFile.
 */
void ConfigurableComponent::configValueChanged(juce::String key)
{
    ComponentConfigFile config;
    if(key != componentKey)
    {
        extraConfigValueChanged(key);
    }
    else
    {
        ComponentConfigFile::ComponentSettings oldSettings 
                = componentSettings;
        componentSettings = config.getComponentSettings(key);
        if (componentSettings.getBounds() != oldSettings.getBounds())
        {
            applyConfigBounds();
        }
        applyConfigAssets(componentSettings.getAssetFiles(),
                componentSettings.getColours());
    }
}
