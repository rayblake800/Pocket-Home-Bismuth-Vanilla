#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(
        juce::String componentKey,
        const juce::String &componentName,
        const juce::String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(componentKey) { }


/**
 * Applies text color settings to a configurable label.
 */
void ConfigurableLabel::applyConfigAssets(juce::StringArray assetNames,
        juce::Array<juce::Colour> colours)
{
    if(colours.size() > 1)
    {
        setColour(juce::Label::backgroundColourId, colours[1]);
    }
    if(colours.size() > 0)
    {
        setColour(juce::Label::textColourId, colours[0]);
    }
}
