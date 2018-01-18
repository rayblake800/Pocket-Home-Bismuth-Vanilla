/**
 *  @file ConfigurableComponent.h
 *  @author Anthony Brown
 * 
 *  ConfigurableComponent applies properties set in a ComponentConfigFile
 * to a component, updating those properties as necessary.
 */

#pragma once
#include "Configurable.h"
#include "../ComponentConfigFile.h"

class ConfigurableComponent : protected Configurable {
public:
    ConfigurableComponent(String componentKey);
    virtual ~ConfigurableComponent();
    virtual void applyConfigBounds();
protected:
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours)=0;
private:
    void loadConfigProperties(ConfigFile * config,String key);
    Component * component;
    ComponentConfigFile::ComponentSettings componentSettings;
};
