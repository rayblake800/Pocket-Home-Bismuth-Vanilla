#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent
(const juce::Identifier& componentKey) :
componentKey(componentKey), configListener(*this) { }

/*
 * Loads and applies this component's relative bounds from the configuration 
 * file.
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
 * Gets the key that defines this component's properties.
 */
const juce::Identifier& ConfigurableComponent::getComponentKey() const
{
    return componentKey;
}

/*
 * Gets this component's x-coordinate as a fraction of the window's* width.
 */
float ConfigurableComponent::getXFraction()
{
    return componentSettings.getXFraction();
}

/*
 * Gets this component's y-coordinate as a fraction of the window's height.
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

ConfigurableComponent::Listener::Listener(ConfigurableComponent& component) :
component(component)
{
   addTrackedKey(component.componentKey); 
   loadAllConfigProperties();
}

/**
 * Load and apply all component data from the ComponentConfigFile.
 */
void ConfigurableComponent::Listener::configValueChanged
(const juce::Identifier& key)
{
    ComponentConfigFile config;
    if(key != component.componentKey)
    {
        component.extraConfigValueChanged(key);
    }
    else
    {
        ComponentSettings oldSettings = component.componentSettings;
        component.componentSettings = config.getComponentSettings(key);
        if (component.componentSettings.getBounds() != oldSettings.getBounds())
        {
            component.applyConfigBounds();
        }
        component.applyConfigAssets(component.componentSettings.getAssetFiles(),
                component.componentSettings.getColours());
    }
}
