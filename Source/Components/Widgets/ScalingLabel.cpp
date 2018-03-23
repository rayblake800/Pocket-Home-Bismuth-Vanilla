#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ScalingLabel.h"

ScalingLabel::ScalingLabel(ComponentConfigFile& config,
        const String &componentName,
        const String &labelText, const int& fontPadding)
: Label(componentName, labelText),
config(config),
fontPadding(fontPadding)
{

#    if JUCE_DEBUG
    setName(String("ScalingLabel:") + labelText);
#    endif
}

/**
 * Updates font size when label bounds change.
 */
void ScalingLabel::resized()
{
    setFont(getFont().withHeight(config.getFontHeight
            (getLocalBounds(), getText())));
}
