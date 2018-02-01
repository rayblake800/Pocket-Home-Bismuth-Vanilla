/**
 * @File ConfigurableImageButton.h
 * @author Anthony Brown
 * ConfigurableImageButton is a DrawableImageButton managed by the
 * ComponentConfigFile.
 */

#pragma once
#include "ConfigurableImageComponent.h"
#include "../../Basic Components/DrawableImageButton.h"
#include "../../../JuceLibraryCode/JuceHeader.h"

class ConfigurableImageButton : public DrawableImageButton,
public ConfigurableComponent {
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
     *Adjust bounds based on config settings
     */
    void applyConfigBounds() override;
    
private:
    /**
     * No action needed, asset management is handled by the internal
     * ConfigurableImageComponent
     */
    void applyConfigAssets(Array<String> assetNames,
            Array<Colour> colours) {
    }
};