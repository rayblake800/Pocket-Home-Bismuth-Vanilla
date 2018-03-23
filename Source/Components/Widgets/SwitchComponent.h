/**
 * @file SwitchComponent.h
 * 
 * SwitchComponent is a ToggleButton drawn as a round handle that switches
 * off(left) or on(right) in front of a rounded rectangle.
 */
#pragma once
#include "JuceHeader.h"

class SwitchComponent : public ToggleButton, private Timer
{
public:

    enum ColourIds
    {
        backgroundColourId = 0x100f000,
        handleColourId = 0x100f001,
        handleOffColourId = 0x100f002
    };

    SwitchComponent();

    virtual ~SwitchComponent() { }

    /**
     * Behaves exactly like the same method in ToggleButton, except it also
     * updates the button handle.
     * 
     * @param shouldBeOn     Sets the button toggle state.
     * 
     * @param notification   Determines if listeners will be notified.
     * 
     * @param animate        If true, this will animate a change in toggle 
     *                        state. This may briefly delay the change in toggle 
     *                        state if the switch is currently animating.
     */
    void setToggleState(bool shouldBeOn, NotificationType notification =
            NotificationType::dontSendNotification, bool animate = false);

private:

    /**
     * Draws the switch background as a rounded rectangle.  This should only be
     * called by Juce library code.
     * 
     * @param g
     * 
     * @param isMouseOverButton
     * 
     * @param isButtonDown
     */
    void paintButton(Graphics &g,
            bool isMouseOverButton, bool isButtonDown) override;

    /**
     * Update the switch background and handle shapes to the new bounds,
     * without changing their aspect ratios.
     */
    void resized() override;

    /**
     * Animates the transition between on and off states, moving the handle
     * left or right as appropriate.
     */
    void clicked() override;

    /**
     * Used for drawing the circular switch handle.  This exists as a separate
     * component because Juce library animation only works through changing
     * component bounds or transparency.
     */
    class SwitchHandle : public Component
    {
    public:

        SwitchHandle() { }

        virtual ~SwitchHandle() { }

        /**
         * Sets the switch fill color.
         * 
         * @param colour
         */
        void setColour(Colour colour)
        {
            this->colour = colour;
        }
    private:
        /**
         * Draws a filled circle using the color provided through setColour().
         * 
         * @param g
         */
        void paint(Graphics& g) override;
        //fill colour
        Colour colour;
    };

    /**
     * Handles switch transitions delayed by waiting for wifi.
     */
    void timerCallback() override;

    //defines the background width:height ratio as 1.5:1
    float widthByHeight = 1.5;
    
    //switch animation duration in milliseconds
    int animationDuration = 150;
    
    //draws the switch handle as a filled circle. 
    SwitchHandle handle;
    
    //defines the bounds of the switch handle in both button states
    Rectangle<int> handleBoundsOff, handleBoundsOn;
    
    //defines the switch's rounded rectangle background
    Rectangle<int> backgroundShape;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SwitchComponent)
};
