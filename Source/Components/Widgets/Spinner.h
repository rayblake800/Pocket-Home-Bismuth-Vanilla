/**
 * @file Spinner.h
 * 
 * Spinner is a Component that shows a small spinning circle animation meant to
 * be used as a loading indicator.  Spinner only animates while visible, and
 * can be set to automatically lose visibility after a set amount of time.
 */

#pragma once
#include "DrawableImageComponent.h"
#include "WindowFocusedTimer.h"
#include "JuceHeader.h"

class Spinner : public Component, private WindowFocusedTimer {
public:
    /**
     * @param secondsToTimeout Sets how many seconds should pass after enabling
     *                          the spinner before the spinner is automatically 
     *                          disabled.  If this value is negative, the 
     *                          spinner will keep running until destroyed or 
     *                          turned off with setEnabled().
     */
    Spinner(int secondsToTimeout = -1);
    
    ~Spinner();
    
    /**
     * Changes the color of the spinner icon.
     * 
     * @param spinnerColour  A new color to apply in place of the default
     *                        white.
     */
    void setColour(Colour spinnerColour);

protected:
    /**
     * Sets the internal image component's bounds within the Spinner.
     * 
     * @param imageBounds
     */
    void setImageBounds(Rectangle<int> imageBounds);
    
private:
    /**
     * Disables animation when losing visibility, enables animation when
     * gaining visibility.
     */
    void visibilityChanged() override;

    /**
     * Sets spinner image bounds to match the component.
     */
    void resized() override;

    /**
     * Shows the next frame of the spinner animation, and disables the spinner
     * if runtime exceeds the timeout period.
     */
    void timerCallback();

    //current shown spinner image
    DrawableImageComponent spinnerImage;
    
    //the spinner cycles through these images to display a simple animation
    Array<Image> spinnerImages;
    
    //the current spinnerImages index loaded into spinnerImage
    int imageIndex = 0;
    
    //The amount of time, in milliseconds, that the spinner has been visible
    int runtime = 0;
    
    //The amount of time, in milliseconds, between switching image frames
    const int timerInterval = 500;
    
    //The number of milliseconds to allow the spinner to run, or -1 to let
    //the spinner run indefinitely.
    int timeout;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spinner)
};
