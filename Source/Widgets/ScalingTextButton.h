#pragma once
#include "Layout_Component_TextSize.h"
#include "JuceHeader.h"

/**
 * @brief  A Juce TextButton that can have its minimum text scale set to one of
 *         the text height values set in the ComponentConfigFile.
 *
 * Provides basic getter and setter methods to save a 
 * ComponentConfigFile::TextSize that sets this button's maximum text scale.
 * Since PokeLookAndFeel already handles TextButton drawing, that class will
 * also be responsible for detecting if a TextButton is also a
 * ScalingTextButton, and enforcing the button's maximum text height.
 */

class ScalingTextButton : public juce::TextButton
{
public:
    //Inherit all of TextButton's constructors.
    using juce::TextButton::TextButton;
    
    /**
     * Sets the maximum text height scale to use when rendering this button's
     * text.
     * 
     * @param textScale  One of the text height options defined in the
     *                  ComponentConfigFile.
     */
    void setMaxTextScale(Layout::Component::TextSize textScale);
    
    /**
     * Gets the maximum text height scale to use when rendering this button's
     * text.
     * 
     * @return  A maximum text height scale defined in the ComponentConfigFile.
     */
    Layout::Component::TextSize getMaxTextScale();
    
private:
    Layout::Component::TextSize maxTextScale =
                Layout::Component::TextSize::largeText;
};
