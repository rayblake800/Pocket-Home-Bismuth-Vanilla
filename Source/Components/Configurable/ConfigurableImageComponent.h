#pragma once
#include "ConfigurableComponent.h"
#include "DrawableImageComponent.h"

/**
 * @file  ConfigurableImageComponent.h
 * 
 * @brief  A DrawableImageComponent with relative bounds, image assets, and 
 *         image colors set by the ComponentConfigFile.
 */

class ConfigurableImageComponent : public DrawableImageComponent,
public ConfigurableComponent
{
public:
    /**
     * @param componentKey  One of the component keys defined by the 
     *                      ComponentConfigFile.
     * 
     * @param assetIndex    Selects which component asset file should be 
     *                      loaded on initialization.
     * 
     * @param placement     Defines how the image will be scaled.
     */
    ConfigurableImageComponent(const juce::Identifier& componentKey,
            const int assetIndex = 0,
            const juce::RectanglePlacement placement 
                    = juce::RectanglePlacement::centred);

    ~ConfigurableImageComponent() { }

    /**
     * @brief  Loads a new image from a different asset file.
     * 
     * @param index  The index of an asset file defined for this component in
     *               the ComponentConfigFile.  If index is out of bounds or it 
     *               equals the current loaded index, nothing will happen. 
     */
    void setImageAssetIndex(const int index);

protected:
    /**
     * @brief  Reloads the image file and sets the image colors.
     * 
     * @param assetNames  The list of all asset files associated with this
     *                    component.
     * 
     * @param colours     Custom colour values used to replace the default 
     *                    image colours.
     */
    virtual void applyConfigAssets(const juce::StringArray& assetNames,
            const juce::Array<juce::Colour>& colours) override;

private:
    /* current selected image asset. */
    int assetIndex = 0;
};
