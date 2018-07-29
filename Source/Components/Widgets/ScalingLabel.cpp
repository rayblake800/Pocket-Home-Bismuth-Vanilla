#include "Utils.h"
#include "ComponentConfigFile.h"
#include "ScalingLabel.h"

ScalingLabel::ScalingLabel(
        const juce::String &componentName,
        const juce::String &labelText,
        const int& fontPadding)
: Label(componentName, labelText),
sizeListener(this),
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
void ScalingLabel::setMaximumTextSize(ComponentConfigFile::TextSize maxSize)
{
    this->maxSize = maxSize;
    resized();
}

/*
 * Updates font size when label bounds change.
 */
void ScalingLabel::resized()
{
    ComponentConfigFile config;
    int fontHeight = std::min(config.getFontHeight(maxSize),
            config.getFontHeight(getLocalBounds(), getText()));
    setFont(getFont().withHeight(fontHeight));
}

ScalingLabel::SizeListener::SizeListener(ScalingLabel* label) : label(label)
{
    ComponentConfigFile config;
    config.addListener(this,{
        ComponentConfigFile::smallTextKey,
        ComponentConfigFile::mediumTextKey,
        ComponentConfigFile::largeTextKey
    });
}

/*
 * Updates the ScalingLabel component when text size configuration changes.
 */
void ScalingLabel::SizeListener::configValueChanged(juce::String propertyKey)
{
    if(label != nullptr)
    {
        label->resized();
    }
}
