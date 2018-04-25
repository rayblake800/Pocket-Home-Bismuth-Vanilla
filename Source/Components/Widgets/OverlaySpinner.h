#pragma once
#include "JuceHeader.h"
#include "ScalingLabel.h"
#include "Spinner.h"

/**
 * @file OverlaySpinner.h
 * 
 * @brief A UI component displaying a Spinner and optional loading text over
 *        a full-screen overlay.
 * 
 * @see Spinner.h
 */

class OverlaySpinner : public Component
{
public:

    enum ColourIds
    {
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };

    /**
     * @param secondsToTimeout   Sets how long the spinner will show itself 
     *                            before timing out. If this value is negative, 
     *                            the spinner will never time out.
     */
    OverlaySpinner(int secondsToTimeout = -1);
    
    virtual ~OverlaySpinner() { }

    /**
     * Sets text to display below the spinner.
     * 
     * @param newText
     */
    void setLoadingText(String newText);

private:

    /**
     * Fills in the overlay background.  This method should never be called
     * directly.
     * 
     * @param g
     */
    void paint(Graphics & g) override;

    /**
     * Update the spinner and loading text to fit overlay bounds.
     */
    void resized() override;

    //Displays optional loading text below the spinner.
    ScalingLabel loadingText;
    
    Spinner spinner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
