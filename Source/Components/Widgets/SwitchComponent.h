/**
 * @file SwitchComponent.h
 * 
 * TODO: finish documentation.
 */
#pragma once
#include "JuceHeader.h"

class SwitchComponent : public ToggleButton {
public:

    enum ColourIds {
        backgroundColourId = 0x100f000,
        handleColourId = 0x100f001,
        handleOffColourId = 0x100f002
    };

    SwitchComponent();
    virtual ~SwitchComponent() {}

private:
    /**
     * 
     * @param g
     * @param isMouseOverButton
     * @param isButtonDown
     */
    void paintButton(Graphics &g,
            bool isMouseOverButton, bool isButtonDown) override;
    
    /**
     * 
     */
    void resized() override;
    
    /**
     * 
     */
    void clicked() override;
    
    /**
     * 
     */
    void updateHandlePath();
    
    
    float widthByHeight = 1.5;
    DrawablePath handle;
    Rectangle<int> handleBoundsOff, handleBoundsOn;
    Rectangle<int> pillBounds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SwitchComponent)
};
