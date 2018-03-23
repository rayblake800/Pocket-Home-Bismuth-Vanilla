/**
 * @file ConfigurableComponent.h
 * 
 * ConfigurableComponent applies properties set in a ComponentConfigFile
 * to a component, updating those properties as necessary.  This may set the
 * component size and position relative to the window, component asset files,
 * and/or component color values.
 * 
 * @see ConfigFile.h
 */
#pragma once
#include "ComponentConfigFile.h"

class ConfigurableComponent : public ConfigFile::Listener
{
public:

    /**
     * @param componentKey  Sets the componentKey that defines this component's
     *                       bounds and asset files.
     * 
     * @param config        A reference to the ComponentConfigFile used to load
     *                       component properties.
     */
    ConfigurableComponent(const String& componentKey,
            ComponentConfigFile& config);

    virtual ~ConfigurableComponent() { }

    /**
     * Load and apply this component's relative bounds from config.
     */
    virtual void applyConfigBounds();

protected:

    /**
     * This method passes in asset file names and asset color values
     * when the component is created, and whenever those values change
     * in configuration.
     * 
     * By default, this will do nothing, inheriting classes are responsible
     * for implementing any useful behavior.
     * 
     * @param assetNames  A list of file names to load from the program asset 
     *                     folder.
     * 
     * @param colours     An optional list of Colour objects to use for 
     *                     recoloring image assets.  Standard component colors 
     *                     should be loaded through the component's LookAndFeel,
     *                     not through here.
     * 
     * @see PokeLookAndFeel.h
     */
    virtual void applyConfigAssets(StringArray assetNames,
            Array<Colour> colours) { }

    /**
     * Handles updates to any tracked config key values other than the 
     * one defining the component. Inheriting classes should override 
     * this instead of loadConfigProperties.
     * 
     * @param config  The updated ConfigFile object.
     * 
     * @param key     The updated data value's key.
     */
    virtual void extraConfigValueChanged(ConfigFile* config,
            String key) { }

private:
    /**
     * Load and apply all component data from the ComponentConfigFile
     * 
     * @param config  This should be the ComponentConfigFile.
     * 
     * @param key     This selects the correct component data from config.
     */
    void configValueChanged(ConfigFile* config, String key) final override;

    //Component properties loaded from config
    ComponentConfigFile::ComponentSettings componentSettings;
    //this component's config key
    const String componentKey;
};
