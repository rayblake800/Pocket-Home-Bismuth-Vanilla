/**
 * @file OverlaySpinner.h
 * 
 * TODO: finish documentation.
 */

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class OverlaySpinner : public Component {
public:

    OverlaySpinner();
    ~OverlaySpinner();
    
    void setVisible(bool shouldBeVisible);
    void setLoadingText(String newText);
    
    enum ColourIds{
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };
private:
    void paint(Graphics &) override;
    void resized() override;

    ScopedPointer<ImageComponent> spinnerImage;
    Array<Image> spinnerImages;
    Label loadingText;

    //TODO:inherit from Timer instead of holding a custom Timer class
    class OverlaySpinnerTimer : public Timer {
    public:
        OverlaySpinnerTimer(OverlaySpinner *os) : overlaySpinner(os){
        };
    private:
        void timerCallback();

        OverlaySpinner* overlaySpinner;
        int imageIndex = 0;
        int runtime = 0;
        const int timeout = 30 * 1000;
    };
    OverlaySpinnerTimer overlaySpinnerTimer;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
