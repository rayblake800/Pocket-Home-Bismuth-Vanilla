#pragma once
#include "ComponentConfigFile.h"

/**
 * @file ConfigurableComponent.h
 * 
 * @brief  Applies properties set in a ComponentConfigFile to a component, 
 *         updating those properties as necessary.  
 * 
 * This may set the component size and position relative to the window, change 
 * component asset files, and/or change image color values.
 * 
 * @see ComponentConfigFile.h
 */

class ConfigurableComponent
{
public:
    /**
     * @param componentKey  Sets the componentKey that defines this component's
     *                      bounds and asset files.
     */
    ConfigurableComponent(const juce::Identifier& componentKey);

    virtual ~ConfigurableComponent() { }

    /**
     * @brief  Loads and applies this component's relative bounds, asset files,
     *         and custom colors from the configuration file.
     *
     * All sizes and coordinates are relative to the application window size.
     * Any coordinates or dimensions that are not defined in the configuration
     * file will remain unchanged.
     *
     * Subclasses must override applyConfigAssets to define how image assets
     * and colors are applied. Otherwise, they are ignored by default. 
     */
    virtual void applyConfigSettings();
    
    /**
     * @brief  Gets the key that defines this component's properties.
     * 
     * @return  The stored component key. 
     */
    const juce::Identifier& getComponentKey() const;
        
    /**
     * @brief  Gets this component's x-coordinate as a fraction of the window's
     *         width.
     * 
     * @return  The x coordinate fraction, or -1 if the x-coordinate fraction
     *          is not defined.
     */
    float getXFraction();

    /**
     * @brief  Gets this component's y-coordinate as a fraction of the window's
     *         height.
     * 
     * @return  The y coordinate fraction, or -1 if the y-coordinate fraction
     *          is not defined.
     */
    float getYFraction();

    /**
     * @brief  Gets this component's width as a fraction of the window's width.
     * 
     * @return  The width fraction, or -1 if the width fraction is not 
     *          defined.
     */
    float getWidthFraction();      

    /**
     * @brief  Gets this component's height as a fraction of the window's 
     *         height.
     * 
     * @return  The height fraction, or -1 if the height fraction is not 
     *          defined.
     */
    float getHeightFraction();

protected:
    /**
     * @brief  Passes in asset file names and asset color values when the 
     *         component is created, and whenever those values change
     *         in configuration.
     * 
     * By default, this will do nothing, inheriting classes are responsible
     * for implementing any useful behavior.
     * 
     * @param assetNames  A list of file names to load from the program asset 
     *                    folder.
     * 
     * @param colours     An optional list of Colour objects to use for 
     *                    recoloring image assets.  Standard component colors 
     *                    should be loaded through the component's LookAndFeel,
     *                    not through here.
     * 
     * @see PokeLookAndFeel.h
     */
    virtual void applyConfigAssets(juce::StringArray assetNames,
            juce::Array<juce::Colour> colours) { }

    /**
     * @brief  Handles updates to any tracked config key values other than the 
     *         one defining the component. Inheriting classes should override 
     *         this instead of loadConfigProperties.
     * 
     * @param key  The updated data value's key
     */
    virtual void extraConfigValueChanged(const juce::Identifier& key) { }

private:
    /**
     * Package Listener functions as a separate object.
     */
    class Listener : public ComponentConfigFile::Listener
    {
    public:
        Listener(ConfigurableComponent& component);

        virtual ~Listener();

        /**
         * Load and apply all component data from the ComponentConfigFile.
         * 
         * @param key  Selects the correct component data from configuration.
         */
        virtual void configValueChanged(const juce::Identifier& key) 
        final override;
    private:
        ConfigurableComponent& component;
    };
    friend Listener;
    Listener configListener;

    /* Component properties loaded from configuration file. */
    ComponentSettings componentSettings;
    
    /* The component configuration key */
    const juce::Identifier& componentKey;
};
