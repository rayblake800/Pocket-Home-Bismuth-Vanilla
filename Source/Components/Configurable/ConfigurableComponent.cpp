#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent
(const juce::Identifier& componentKey) :
componentKey(componentKey), configListener(*this) 
{
    ComponentConfigFile config;
    componentSettings = config.getComponentSettings(componentKey);
}

/*
 * Applies the relative bounds, asset files, and custom colors defined in 
 * configuration file to this component.
 */
void ConfigurableComponent::applyConfigSettings()
{
    applyConfigBounds();
    applyConfigAssets(componentSettings.getAssetFiles(), 
            componentSettings.getColours());
}

/*
 * Uses configuration data to set the component's bounds.
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
float ConfigurableComponent::getXFraction() const
{
    return componentSettings.getXFraction();
}

/*
 * Gets this component's y-coordinate as a fraction of the window's height.
 */
float ConfigurableComponent::getYFraction() const
{
    return componentSettings.getYFraction();
}

/*
 * Gets this component's width as a fraction of the window's width.
 */
float ConfigurableComponent::getWidthFraction() const
{
    return componentSettings.getWidthFraction();
}

/*
 * Gets this component's height as a fraction of the window's height.
 */
float ConfigurableComponent::getHeightFraction() const
{
    return componentSettings.getHeightFraction();
}

ConfigurableComponent::Listener::Listener(ConfigurableComponent& component) :
component(component)
{
   addTrackedKey(component.componentKey); 
   loadAllConfigProperties();
}

/*
 * Applies changes to component settings to the component.
 */
void ConfigurableComponent::Listener::configValueChanged
(const juce::Identifier& key)
{
    ComponentConfigFile config;
    jassert(key == component.componentKey);
    ComponentSettings oldSettings = component.componentSettings;
    component.componentSettings = config.getComponentSettings(key);
    if (component.componentSettings.getBounds() != oldSettings.getBounds())
    {
        component.applyConfigBounds();
    }
    component.applyConfigAssets(component.componentSettings.getAssetFiles(),
            component.componentSettings.getColours());
}
