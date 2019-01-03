#pragma once
#include "JuceHeader.h"
#include "WindowFocusedTimer.h"
#include "ConfigurableLabel.h"
#include "Config/MainListener.h"

/**
 * @file ClockLabel.h
 * 
 * @brief  A configurable component that displays the current time.
 */

class ClockLabel : public ConfigurableLabel, public Config::MainListener,
        private WindowFocusedTimer
{
public:

    ClockLabel();
    
    virtual ~ClockLabel() { }

private:
    /**
     * @brief Updates the displayed time each minute.
     */
    void timerCallback();
    
    /**
     * @brief  Enables the timer when the component becomes visible, disables 
     *         it when visibility is lost.
     */
    void visibilityChanged() override;
    
    /**
     * @brief Receives notification whenever clock configuration values change.
     * 
     * @param key     The updated setting's key, either the clock visibility 
     *                setting key, or the 12h/24h mode key. 
     */
    void configValueChanged(const juce::Identifier& key) override;
    
    /* If true, use 24 hour time, if false, use 12 hour AM/PM time. */
    bool use24HrMode = false;
    /* Indicates if the time should be shown at all. */
    bool showClock = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockLabel)
};
