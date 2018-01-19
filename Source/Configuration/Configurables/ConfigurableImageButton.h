/**
 * @File ConfigurableImageButton.h
 * @author Anthony Brown
 * ConfigurableImageButton is a button drawn using a ConfigurableImageComponent
 */

#pragma once
#include "ConfigurableImageComponent.h"
#include "../../../JuceLibraryCode/JuceHeader.h"

class ConfigurableImageButton : public Button{
public:
    
    /**
     * @param componentKey one of the component keys defined by the 
     *  ComponentConfigFile
     * @param assetIndex selects which component asset file should be loaded
     *  by the image on initialization.
     * @param placement defines how the image will be scaled
     */
    ConfigurableImageButton(String componentKey, int assetIndex = 0,
            RectanglePlacement placement=RectanglePlacement::centred);
    
    virtual ~ConfigurableImageButton();
    
    /**
     *Re-calculate image bounds from config settings.
     */
    void applyConfigBounds();
    
private:
    //no special painting needed, button appearance is handled by the 
    //configurable image
    void paintButton(Graphics &g, bool isMouseOverButton, bool isButtonDown);
    ConfigurableImageComponent image;
};