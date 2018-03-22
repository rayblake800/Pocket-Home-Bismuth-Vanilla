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
     * @param config        A reference to the ComponentConfigFile.
     * 
     * @param componentKey  The label's key in the ComponentConfigFile.
     * 
     * @param componentName The component's internal name value.
     *  
     * @param labelText     The initial label display text.
     *
     *  @param fontPadding  The font height will not exceed 
     *                       (getHeight() - fontPadding).
     */
    ConfigurableLabel(ComponentConfigFile& config,
            String componentKey,
            const String &componentName = String(),
            const String &labelText = String(),
            const int& fontPadding = 0);

    virtual ~ConfigurableLabel() { }

protected:
};
