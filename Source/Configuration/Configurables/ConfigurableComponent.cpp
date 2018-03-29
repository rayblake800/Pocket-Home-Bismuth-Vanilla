#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent(const String& componentKey) :
componentKey(componentKey)
{ 
    ComponentConfigFile config;
    componentSettings = config.getComponentSettings(componentKey);
    config.addListener(this,{componentKey});
}

/**
 * Load and apply this component's relative bounds from config.
 */
void ConfigurableComponent::applyConfigBounds()
{
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


/**
 * Load and apply all component data from the ComponentConfigFile.
 */
void ConfigurableComponent::configValueChanged(String key)
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
