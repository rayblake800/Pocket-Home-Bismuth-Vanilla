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
 * Sets the maximum height of the label text.
 */
void ScalingLabel::setMaximumTextSize(ComponentConfigFile::TextSize maxSize)
{
    this->maxSize = maxSize;
    resized();
}

/**
 * Updates font size when label bounds change.
 */
void ScalingLabel::resized()
{
    ComponentConfigFile config;
    int fontHeight = std::min(config.getFontHeight(maxSize),
        config.getFontHeight(getLocalBounds(), getText()));
    setFont(getFont().withHeight(fontHeight));
}
