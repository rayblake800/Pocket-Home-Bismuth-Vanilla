#pragma once
#include "ScalingLabel.h"
#include "ConfigurableComponent.h"

/**
 * @file ConfigurableLabel.h
 * 
 * @brief ConfigurableLabel is a scaling label that sets its properties from the
 * component configuration file.
 */

class ConfigurableLabel : public ScalingLabel, public ConfigurableComponent {
public:
    /**
     * @param componentKey  The label's key in the ComponentConfigFile.
     * 
     * @param componentName The component's internal name value.
     *  
     * @param labelText     The initial label display text.
     *
     * @param fontPadding  The font height will not exceed 
     *                       (getHeight() - fontPadding).
     */
    ConfigurableLabel(String componentKey,
            const String &componentName = String(),
            const String &labelText = String(),
            const int& fontPadding = 0);

    virtual ~ConfigurableLabel() { }

private:
    /**
     * Applies text color settings to a configurable label.
     * 
     * @param assetNames   This should be empty, and will be ignored.
     * 
     * @param colours      If this list has a color at index 0, it will be 
     *                      applied to the label text.  If it has a color at
     *                      index 1, that will be applied to the label
     *                      background.
     */
    void applyConfigAssets(StringArray assetNames,
            Array<Colour> colours) override;
};
