#include "ScalingLabel.h"
#include "Layout_Component_ConfigFile.h"
#include "Utils.h"

ScalingLabel::ScalingLabel(
        const juce::String &componentName,
        const juce::String &labelText,
        const int& fontPadding)
: Label(componentName, labelText),
fontPadding(fontPadding)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName(String("ScalingLabel:") + labelText);
#    endif
}

/*
 * Sets the maximum height of the label text.
 */
void ScalingLabel::setMaximumTextSize(Layout::Component::TextSize maxSize)
{
    this->maxSize = maxSize;
    resized();
}

/*
 * Updates font size when label bounds change.
 */
void ScalingLabel::resized()
{
    Layout::Component::ConfigFile config;
    int fontHeight = std::min(config.getFontHeight(maxSize),
            config.getFontHeight(getLocalBounds(), getText()));
    setFont(getFont().withHeight(fontHeight));
}

