#pragma once
/**
 * @file OverlaySpinner.h
 * 
 * @brief  Provides an overlay component that displays a loading spinner and
 *         optional loading text.
 */

#include "Spinner.h"
#include "JuceHeader.h"
#include "Widgets_BoundedLabel.h"


/**
 * @brief A UI component displaying a Spinner and optional loading text over
 *        a full-screen overlay.
 */
class OverlaySpinner : public juce::Component
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
    void setLoadingText(juce::String newText);

private:

    /**
     * Fills in the overlay background.  This method should never be called
     * directly.
     * 
     * @param g
     */
    void paint(juce::Graphics & g) override;

    /**
     * Update the spinner and loading text to fit overlay bounds.
     */
    void resized() override;

    //Displays optional loading text below the spinner.
    Widgets::BoundedLabel loadingText;
    
    Spinner spinner;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OverlaySpinner)
};
