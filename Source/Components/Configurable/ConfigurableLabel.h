#pragma once
#include "ScalingLabel.h"
#include "ConfigurableComponent.h"

/**
 * @file   ConfigurableLabel.h
 * 
 * @brief  A scaling label that loads its properties from the component 
 *         configuration file.
 */

class ConfigurableLabel : public ScalingLabel, public ConfigurableComponent {
public:
    /**
     * @param componentKey   The label's key in the ComponentConfigFile.
     * 
     * @param componentName  The component's internal name value.
     *  
     * @param labelText      The initial label display text.
     *
     * @param fontPadding    The font height will not exceed 
     *                       (getHeight() - fontPadding).
     */
    ConfigurableLabel(
            const juce::Identifier& componentKey,
            const juce::String &componentName = juce::String(),
            const juce::String &labelText = juce::String(),
            const int& fontPadding = 0);

    virtual ~ConfigurableLabel() { }
};
