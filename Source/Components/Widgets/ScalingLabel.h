/**
 * @File ScalingLabel.h
 * 
 * ScalingLabel is a basic Juce Label that adjusts its text height to fit its
 * bounds whenever it is resized.
 */

#pragma once
#include "ComponentConfigFile.h"
#include "JuceHeader.h"

class ScalingLabel : public Label
{
public:

    /**
     * @param config         Holds user settings for preferred text height,
     *                        relative to the window height.
     * 
     * @param componentName  Sets the component's internal name value. 
     * 
     * @param labelText      Sets the initial label display text.
     * 
     * @param fontPadding    Sets the minimum number of pixels to leave between
     *                        the text top and bottom and the label edges.
     */
    ScalingLabel(ComponentConfigFile& config,
            const String& componentName = String(),
            const String& labelText = String(),
            const int& fontPadding = 0);

    virtual ~ScalingLabel() { }

    private:
    /**
     * Updates font size when label bounds change.
     */
    void resized() override;

    //Pixels of vertical space to leave between text and component bounds.
    int fontPadding = 0;
    //This reference is used for loading up to date font configuration whenever
    //the label is resized.
    ComponentConfigFile& config;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScalingLabel)
};
