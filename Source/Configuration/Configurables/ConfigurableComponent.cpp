#include "../ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent
(String componentKey) :
Configurable(new ComponentConfigFile(),{componentKey}),
componentSettings(ComponentConfigFile().getComponentSettings(componentKey)) { }

ConfigurableComponent::~ConfigurableComponent() { }

/**
 * Load and apply this component's relative bounds from config.
 */
void ConfigurableComponent::applyConfigBounds()
{
    Component * component = dynamic_cast<Component*> (this);

    if (component == nullptr)
    {
        DBG("Error: non-component class trying to inherit ConfigurableComponent");
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

/**
 * This method passes in asset file names and asset color values
 * when the component is created, and whenever those values change
 * in configuration.
 */
void ConfigurableComponent::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours) { }

/**
 * Load and apply all component data from the ComponentConfigFile
 * @param key selects the correct component data from config.
 */
void ConfigurableComponent::loadConfigProperties(String key)
{
    ComponentConfigFile config;
    ComponentConfigFile::ComponentSettings oldSettings = componentSettings;
    componentSettings = config.getComponentSettings(key);
    if (componentSettings.getBounds() != oldSettings.getBounds())
    {
        applyConfigBounds();
    }
    applyConfigAssets(componentSettings.getAssetFiles(),
            componentSettings.getColours());
}
