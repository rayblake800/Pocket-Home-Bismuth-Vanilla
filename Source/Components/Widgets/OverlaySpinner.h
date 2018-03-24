/**
 * @file OverlaySpinner.h
 * 
 * OverlaySpinner is a Spinner component that covers the entire window, and
 * optionally displays loading text.
 * 
 * @see Spinner.h
 */

#pragma once
#include "JuceHeader.h"
#include "ScalingLabel.h"
#include "Spinner.h"

class OverlaySpinner : public Spinner
{
public:

    enum ColourIds
    {
        backgroundColourId = 0x1900400,
        textColourId = 0x1900401
    };

    /**
     * @param config             Shared component configuration object needed 
     *                            by the loading text label.
     * 
     * @param secondsToTimeout   Sets how long the spinner will show itself 
     *                            before timing out. If this value is negative, 
     *                            the spinner will never time out.
     */
    OverlaySpinner(ComponentConfigFile& config, int secondsToTimeout = -1);
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
