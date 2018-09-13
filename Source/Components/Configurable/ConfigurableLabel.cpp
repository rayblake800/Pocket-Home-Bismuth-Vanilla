#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(
        const juce::Identifier& componentKey,
        const juce::String &componentName,
        const juce::String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(componentKey) { }

