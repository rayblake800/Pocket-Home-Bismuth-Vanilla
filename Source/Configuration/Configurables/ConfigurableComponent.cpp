/*
  ==============================================================================

    ConfigurableComponent.cpp
    Created: 17 Jan 2018 3:36:42pm
    Author:  anthony

  ==============================================================================
 */

#include "../../PocketHomeApplication.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent
(String componentKey) :
Configurable(static_cast<ConfigFile*>
(&PocketHomeApplication::getInstance()->getComponentConfig()),{componentKey})
{
}

ConfigurableComponent::~ConfigurableComponent()
{
}

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

void ConfigurableComponent::loadConfigProperties
(ConfigFile * config, String key)
{
    ComponentConfigFile * compConfig
            = dynamic_cast<ComponentConfigFile*> (config);
    if (compConfig != nullptr)
    {
        ComponentConfigFile::ComponentSettings oldSettings = componentSettings;
        componentSettings = compConfig->getComponentSettings(key);
        if (componentSettings.getBounds() != oldSettings.getBounds())
        {
            applyConfigBounds();
        }
        applyConfigAssets(componentSettings.getAssetFiles(),
                componentSettings.getColours());
    }
}