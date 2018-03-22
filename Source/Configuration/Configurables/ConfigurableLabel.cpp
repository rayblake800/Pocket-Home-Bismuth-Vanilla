#include "ConfigurableLabel.h"

ConfigurableLabel::ConfigurableLabel(ComponentConfigFile& config,
        String componentKey,
        const String &componentName,
        const String &labelText,
        const int& fontPadding) :
ScalingLabel(componentName, labelText, fontPadding),
ConfigurableComponent(config,componentKey)
{
    loadAllConfigProperties();
}