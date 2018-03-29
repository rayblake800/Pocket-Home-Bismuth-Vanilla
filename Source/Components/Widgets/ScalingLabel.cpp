#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ScalingLabel.h"

ScalingLabel::ScalingLabel(
        const String &componentName,
        const String &labelText,
        const int& fontPadding)
: Label(componentName, labelText),
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
    ComponentConfigFile config;
    setFont(getFont().withHeight(config.getFontHeight
            (getLocalBounds(), getText())));
}
