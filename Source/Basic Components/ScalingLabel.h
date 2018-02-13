/**
 * @File ScalingLabel.h
 * 
 * ScalingLabel is a basic Juce Label that adjusts its text height to fit its
 * bounds whenever it is resized.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class ScalingLabel : public Label {
public:
    
    /**
     * 
     * @param componentName component's internal name value 
     * @param labelText initial label display text
     * @param fontPadding font height will be kept at this many pixels
     * less than the component height.
     */
    ScalingLabel(const String &componentName = String(),
            const String &labelText = String(),
            const int& fontPadding = 0);
    virtual ~ScalingLabel();
private:
    void resized() override;
    int fontPadding;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScalingLabel)
};