#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(String componentKey,
        ComponentConfigFile& config,
        const String &componentName,
        const String &labelText,
        const int& fontPadding) :
ScalingLabel(config, componentName, labelText, fontPadding),
ConfigurableComponent(componentKey, config)
{
    loadAllConfigProperties();
}


/**
 * Applies text color settings to a configurable label.
 */
void ConfigurableLabel::applyConfigAssets(StringArray assetNames,
        Array<Colour> colours)
{
    if(colours.size() > 1)
    {
        setColour(Label::backgroundColourId, colours[1]);
    }
    if(colours.size() > 0)
    {
        setColour(Label::textColourId, colours[0]);
    }
}