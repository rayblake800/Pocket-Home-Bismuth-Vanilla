/**
 * @file ConfigurableImageButton.h
 * 
 * ConfigurableImageButton is a DrawableImageButton managed by the
 * ComponentConfigFile.
 */
#pragma once
#include "JuceHeader.h"
#include "ConfigurableImageComponent.h"
#include "DrawableImageButton.h"

class ConfigurableImageButton : public Button,
public ConfigurableComponent
{
public:

    /**
     * @param componentKey one of the component keys defined by the 
     *  ComponentConfigFile
     * @param assetIndex selects which component asset file should be loaded
     *  by the image on initialization.
     * @param placement defines how the image will be scaled
     */
    ConfigurableImageButton(String componentKey, int assetIndex = 0,
            RectanglePlacement placement = RectanglePlacement::centred);

    virtual ~ConfigurableImageButton();

    /**
     * Load a new image from a different asset file
     * @param index the index of an asset file defined for this component in
     * the ComponentConfigFile.  If index is out of bounds or it equals the
     * current loaded index, nothing will happen. 
     */
    void setImageAssetIndex(int index);
private:
    
    /**
     * Keeps the image set to component bounds.
     */
    void resized() override;

    //Handles asset management and drawing the component.
    ConfigurableImageComponent buttonImage;
    
    void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours) { }

    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
    override { }


};
