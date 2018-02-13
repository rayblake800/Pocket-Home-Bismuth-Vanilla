#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(String componentKey,
        const String &componentName,
        const String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(componentKey)
{
    loadAllConfigProperties();
}

ConfigurableLabel::~ConfigurableLabel()
{}