/**
 *  @file ConfigurableComponent.h
 * 
 *  ConfigurableComponent applies properties set in a ComponentConfigFile
 * to a component, updating those properties as necessary.
 */
#pragma once
#include "Configurable.h"
#include "../ComponentConfigFile.h"

class ConfigurableComponent : public Configurable {
public:
    ConfigurableComponent(String componentKey);
    virtual ~ConfigurableComponent();
    virtual void applyConfigBounds();
protected:
    /**
     * This method passes in asset file names and asset color values
     * when the component is created, and whenever those values change
     * in configuration.
     * 
     * By default, this will do nothing, inheriting classes are responsible
     * for implementing any useful behavior.
     * @param assetNames a list of names of files in the program asset folder.
     * @param colours an optional list of Colour objects to use for recoloring
     * image assets.  Standard component colors should be loaded through
     * the component's LookAndFeel, not through here.
     * @see PokeLookAndFeel
     */
    virtual void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours);
    
    
    virtual void loadConfigProperties(ConfigFile * config,String key);
private:
    Component * component;
    ComponentConfigFile::ComponentSettings componentSettings;
};
