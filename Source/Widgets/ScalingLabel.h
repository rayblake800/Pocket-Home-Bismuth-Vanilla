#pragma once
/**
 * @File  ScalingLabel.h
 * 
 * @brief  A Juce Label component that adjusts its text height to fit its bounds 
 *         whenever it is resized.
 */

#include "JuceHeader.h"
#include "Layout_Component_TextSize.h"

class ScalingLabel : public juce::Label
{
public:
    /**
     * @param componentName  Sets the component's internal name value. 
     * 
     * @param labelText      Sets the initial label display text.
     * 
     * @param fontPadding    Sets the minimum number of pixels to leave between
     *                       the text top and bottom and the label edges.
     */
    ScalingLabel(
            const juce::String& componentName = juce::String(),
            const juce::String& labelText = juce::String(),
            const int& fontPadding = 0);

    virtual ~ScalingLabel() { }

    /**
     * @brief  Sets the maximum height of the label text.
     *
     * @param maxSize  One of the text height values set in ComponentConfigFile.
     */
    void setMaximumTextSize(Layout::Component::TextSize maxSize);

private:
    /**
     * @brief  Updates font size when label bounds change.
     */
    void resized() override;
    
    /* Pixels of vertical space to leave between text and component bounds. */
    int fontPadding = 0;

    /* Maximum text height setting. */
    Layout::Component::TextSize maxSize 
        = Layout::Component::TextSize::largeText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScalingLabel)
};
