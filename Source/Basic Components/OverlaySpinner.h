/**
 * @file OverlaySpinner.h
 * 
 * OverlaySpinner is a Spinner component that covers the entire window, and
 * optionally displays loading text.
 * @see Spinner.h
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "ScalingLabel.h"
#include "Spinner.h"

class OverlaySpinner : public Spinner {
public:
    enum ColourIds{
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };
    
    OverlaySpinner(int secondsToTimeout = -1);
    ~OverlaySpinner();
    
    void setLoadingText(String newText);
    
private:
    void paint(Graphics &) override;
    void resized() override;
    
    ScalingLabel loadingText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
