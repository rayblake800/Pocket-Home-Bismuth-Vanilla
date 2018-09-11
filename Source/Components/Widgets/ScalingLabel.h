#pragma once
#include "JuceHeader.h"
#include "ComponentConfigFile.h"
/**
 * @File  ScalingLabel.h
 * 
 * @brief  A Juce Label component that adjusts its text height to fit its bounds 
 *         whenever it is resized.
 */

class ScalingLabel : public juce::Label
{
public:

    /**
     * @param componentName  Sets the component's internal name value. 
     * 
     * @param labelText      Sets the initial label display text.
     * 
     * @param fontPadding    Sets the minimum number of pixels to leave between
     *                        the text top and bottom and the label edges.
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
    void setMaximumTextSize(ComponentConfigFile::TextSize maxSize);

private:
    /**
     * @brief  Updates font size when label bounds change.
     */
    void resized() override;
    
    /**
     * Listens for changes to text size configuration, and updates the
     * ScalingLabel in response.  I would normally have ScalingLabel directly
     * inherit from ConfigFile::Listener, but doing that would leave
     * ScalingLabel subclasses unable to inherit ConfigurableComponent.
     */
    class SizeListener : public ComponentConfigFile::Listener
    {
    public:
        /**
         * @param label  Passes in the address of the ScalingLabel that owns
         *               this SizeListener.
         */
        SizeListener(ScalingLabel* label);
        
        virtual ~SizeListener() { }
        
    private:
        /**
         * @brief  Updates the ScalingLabel component when text size 
         *         configuration changes.
         * 
         * @param propertyKey  One of the text size keys defined by 
         *                     ComponentConfigFile.
         */
        virtual void configValueChanged(const juce::Identifier& propertyKey)
        override;
        
        ScalingLabel* label = nullptr;
    };
    SizeListener sizeListener;

    /* Pixels of vertical space to leave between text and component bounds. */
    int fontPadding = 0;

    /* Maximum text height setting. */
    ComponentConfigFile::TextSize maxSize = ComponentConfigFile::largeText;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScalingLabel)
};
