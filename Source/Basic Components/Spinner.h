/**
 * @file Spinner.h
 * 
 * Spinner is a Component that shows a small spinning circle animation meant to
 * be used as a loading indicator.  Spinner only animates while visible, and
 * can be set to automatically lose visibility after a set amount of time.
 */

#pragma once
#include "DrawableImageComponent.h"
#include "../WindowFocusedTimer.h"
#include "../../JuceLibraryCode/JuceHeader.h"

class Spinner : public Component, private WindowFocusedTimer {
public:
    /**
     * @param secondsToTimeout sets how many seconds should pass after enabling
     * the spinner before the spinner is automatically disabled.  If this value
     * is negative, the spinner will keep running until destroyed or turned off
     * with setEnabled()
     */
    Spinner(int secondsToTimeout = -1);
    ~Spinner();
    
    void setColour(Colour spinnerColour);

protected:
    /**
     * Set the internal image component's bounds within the Spinner
     */
    void setImageBounds(Rectangle<int> imageBounds);
    
private:
    /**
     * Disable animation when losing visibility, enable animation when
     * gaining visibility.
     */
    void visibilityChanged() override;

    void resized() override;

    /**
     * Show the next frame of the spinner animation, and disable if runtime 
     * exceeds the timeout period.
     */
    void timerCallback();

    DrawableImageComponent spinnerImage;
    Array<Image> spinnerImages;
    int imageIndex = 0;
    int runtime = 0;
    const int timerInterval = 500;
    int timeout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spinner)
};
