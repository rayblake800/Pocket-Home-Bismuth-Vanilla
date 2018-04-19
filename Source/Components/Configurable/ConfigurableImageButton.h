#pragma once
#include "JuceHeader.h"
#include "ConfigurableImageComponent.h"
#include "DrawableImageButton.h"

/**
 * @file ConfigurableImageButton.h
 * 
 * @brief ConfigurableImageButton is a DrawableImageButton managed by the
 * ComponentConfigFile.
 */

class ConfigurableImageButton : public Button,
public ConfigurableComponent
{
public:

    /**
     * @param componentKey  One of the component keys defined by the 
     *                       ComponentConfigFile.
     * 
     * @param assetIndex    Selects which component asset file should be loaded
     *                       by the image on initialization.
     * 
     * @param placement     Defines how the image will be scaled.
     */
    ConfigurableImageButton(
            String componentKey,
            int assetIndex = 0,
            RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~ConfigurableImageButton() { }

    /**
     * Load a new image from a different asset file
     * 
     * @param index   The index of an asset file defined for this component in
     *                 the ComponentConfigFile.  If index is out of bounds or it
     *                 equals the current loaded index, nothing will happen. 
     */
    void setImageAssetIndex(int index);
    
private:
    /**
     * Keeps the image set to component bounds.
     */
    void resized() override;

    //Handles asset management and drawing the component.
    ConfigurableImageComponent buttonImage;
    
    //buttonImage handles all config assets
    void applyConfigAssets(StringArray assetNames,
            Array<Colour> colours) override { }

    //buttonImage handles all component draw operations.
    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
    override { }


};
