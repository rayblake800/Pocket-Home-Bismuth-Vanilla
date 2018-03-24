/**
 * @file ConfigurableImageComponent
 * 
 * ConfigurableImageComponent defines a DrawableImageComponent with properties
 * set in the ComponentConfigFile.
 */

#pragma once
#include "ConfigurableComponent.h"
#include "DrawableImageComponent.h"

class ConfigurableImageComponent : public DrawableImageComponent,
public ConfigurableComponent
{
public:
    /**
     * @param componentKey  One of the component keys defined by the 
     *                       ComponentConfigFile.
     * 
     * @param config        A reference to the ComponentConfigFile.
     * 
     * @param assetIndex    Selects which component asset file should be 
     *                       loaded on initialization.
     * 
     * @param placement     Defines how the image will be scaled.
     */
    ConfigurableImageComponent(String componentKey,
            ComponentConfigFile& config,
            int assetIndex = 0,
            RectanglePlacement placement = RectanglePlacement::centred);

    ~ConfigurableImageComponent() { }

    /**
     * Load a new image from a different asset file.
     * 
     * @param index  The index of an asset file defined for this component in
     *                the ComponentConfigFile.  If index is out of bounds or it 
     *                equals the current loaded index, nothing will happen. 
     */
    void setImageAssetIndex(int index);

protected:
    /**
     * (re)load the image file and set the colors.
     * 
     * @param assetNames  The list of all asset files associated with this
     *                     component.
     * 
     * @param colours     Custom colour values used to replace the default 
     *                     image colours.
     */
    virtual void applyConfigAssets(StringArray assetNames,
            Array<Colour> colours);

private:
    //current selected image asset.
    int assetIndex = 0;
};
