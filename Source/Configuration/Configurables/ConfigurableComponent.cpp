#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ConfigurableComponent.h"

ConfigurableComponent::ConfigurableComponent
(const String& componentKey, ComponentConfigFile& config) :
componentKey(componentKey),
componentSettings(config.getComponentSettings(componentKey)) 
{ 
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
void ConfigurableComponent::configValueChanged
(ConfigFile* config,String key)
{
    ComponentConfigFile* componentConf = 
            dynamic_cast<ComponentConfigFile*>(config);
    if(key != componentKey || componentConf == nullptr)
    {
        extraConfigValueChanged(config,key);
    }
    else
    {
        ComponentConfigFile::ComponentSettings oldSettings 
                = componentSettings;
        componentSettings = componentConf->getComponentSettings(key);
        if (componentSettings.getBounds() != oldSettings.getBounds())
        {
            applyConfigBounds();
        }
        applyConfigAssets(componentSettings.getAssetFiles(),
                componentSettings.getColours());
    }
}
