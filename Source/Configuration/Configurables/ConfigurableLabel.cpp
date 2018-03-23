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