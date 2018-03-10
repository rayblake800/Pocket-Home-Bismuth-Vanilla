/**
 *  @file ConfigurableComponent.h
 * 
 * ConfigurableComponent applies properties set in a ComponentConfigFile
 * to a component, updating those properties as necessary.
 */
#pragma once
#include "Configurable.h"
#include "../ComponentConfigFile.h"

class ConfigurableComponent : public Configurable {
public:
    ConfigurableComponent(String componentKey);
    virtual ~ConfigurableComponent();
    
    /**
     * Load and apply this component's relative bounds from config.
     */
    void applyConfigBounds();
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

    /**
     * Handles updates to any tracked config key values other than the 
     * one defining the component. Inheriting classes should override 
     * this instead of loadConfigProperties.
     * @param config the updated configuration object.
     * @param key the updated data value's key
     */
    virtual void loadExtraConfigProperties(ConfigFile* config,String key) {}

private:    
    /**
     * Load and apply all component data from the ComponentConfigFile
     * @param config should be the componentConfigFile
     * @param key selects the correct component data from config.
     */
    void loadConfigProperties(ConfigFile* config,String key) final override;

    ComponentConfigFile::ComponentSettings componentSettings;
    const String componentKey;
};
