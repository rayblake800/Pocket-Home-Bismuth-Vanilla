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

    /**
     * @brief  Applies color settings to this label.
     *
     * @param assetNames  Ignored parameter, ConfigurableLabel objects don't use
     *                    asset files.
     *
     * @param colours     If not empty, the first element in this list will be
     *                    used as the text color.  Any other colors in the list
     *                    will be ignored.
     */
    virtual void applyConfigAssets(const juce::StringArray& assetNames,
            const juce::Array<juce::Colour>& colours) override;
};
