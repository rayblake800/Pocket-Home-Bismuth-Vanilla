#pragma once
#include "ComponentConfigFile.h"

/**
 * @file ConfigurableComponent.h
 * 
 * @brief ConfigurableComponent applies properties set in a ComponentConfigFile
 * to a component, updating those properties as necessary.  
 * 
 * This may set the component size and position relative to the window, change 
 * component asset files, and/or change image color values.
 * 
 * @see ComponentConfigFile.h
 */

class ConfigurableComponent : public ConfigFile::Listener
{
public:
    /**
     * @param componentKey  Sets the componentKey that defines this component's
     *                       bounds and asset files.
     */
    ConfigurableComponent(const juce::String& componentKey);

    virtual ~ConfigurableComponent() { }

    /**
     * Load and apply this component's relative bounds from config.  All
     * sizes and coordinates are relative to the application window size.
     * Any coordinates or dimensions that are not defined in config will remain
     * unchanged.
     */
    virtual void applyConfigBounds();
    
    /**
     * Gets the componentKey that defines this component's bounds and asset
     * files
     * 
     * @return  the stored component key. 
     */
    const juce::String& getComponentKey() const;
        
    /**
     * Gets this component's x-coordinate as a fraction of the window's
     * width.
     * 
     * @return the x coordinate fraction, or -1 if the x-coordinate fraction
     *         is not defined.
     */
    float getXFraction();

    /**
     * Gets this component's y-coordinate as a fraction of the window's
     * height.
     * 
     * @return the y coordinate fraction, or -1 if the y-coordinate fraction
     *         is not defined.
     */
    float getYFraction();

    /**
     * Gets this component's width as a fraction of the window's width.
     * 
     * @return the width fraction, or -1 if the width fraction is not 
     *         defined.
     */
    float getWidthFraction();      

    /**
     * Gets this component's height as a fraction of the window's height.
     * 
     * @return the height fraction, or -1 if the height fraction is not 
     *         defined.
     */
    float getHeightFraction();

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
    virtual void applyConfigAssets(juce::StringArray assetNames,
            juce::Array<juce::Colour> colours) { }

    /**
     * Handles updates to any tracked config key values other than the 
     * one defining the component. Inheriting classes should override 
     * this instead of loadConfigProperties.
     * 
     * @param key     The updated data value's key.
     */
    virtual void extraConfigValueChanged(juce::String key) { }

private:
    /**
     * Load and apply all component data from the ComponentConfigFile
     * 
     * @param key     This selects the correct component data from config.
     */
    void configValueChanged(juce::String key) final override;

    //Component properties loaded from config
    ComponentConfigFile::ComponentSettings componentSettings;
    //this component's config key
    const juce::String componentKey;
};
