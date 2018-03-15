/**
 * @File ConfigurableLabel.h
 * 
 * ConfigurableLabel is a scaling label that sets its properties from the
 * component configuration file.
 */

#pragma once
#include "ScalingLabel.h"
#include "ConfigurableComponent.h"

class ConfigurableLabel : public ScalingLabel, public ConfigurableComponent {
public:
    /**
     * @param componentKey the label's key in the ComponentConfigFile
     * @param componentName component's internal name value 
     * @param labelText initial label display text
     * @param fontPadding font height will be kept at this many pixels
     * less than the component height.
     */
    ConfigurableLabel(String componentKey,
            const String &componentName = String(),
            const String &labelText = String(),
            const int& fontPadding = 0);

    virtual ~ConfigurableLabel();

protected:
};
