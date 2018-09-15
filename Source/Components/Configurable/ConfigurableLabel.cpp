#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(
        const juce::Identifier& componentKey,
        const juce::String &componentName,
        const juce::String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(componentKey)
{
    applyConfigSettings();
}

/*
 * Applies color settings to this label.
 */
void ConfigurableLabel::applyConfigAssets(const juce::StringArray& assetNames,
        const juce::Array<juce::Colour>& colours)
{
    if(!colours.isEmpty())
    {
        setColour(ScalingLabel::textColourId, colours[0]);
    }
}

