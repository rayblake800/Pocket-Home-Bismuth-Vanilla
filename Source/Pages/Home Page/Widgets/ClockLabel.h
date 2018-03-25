/**
 * @file ClockLabel.h
 * 
 * ClockLabel is a component managed by the ComponentConfigFile that displays
 * the current time.
 */

#pragma once
#include "JuceHeader.h"
#include "WindowFocusedTimer.h"
#include "ConfigurableLabel.h"

class ClockLabel : public ConfigurableLabel, private WindowFocusedTimer{
public:

   /**
    * @param config  Shared component settings that define the label's bounds
    *                and behavior.
    */
    ClockLabel(ComponentConfigFile& config);
    
    virtual ~ClockLabel() { }

private:
    /**
     * Updates the displayed time each minute.
     */
    void timerCallback();
    
    /**
     * Enable the timer when the component becomes visible, disable it when
     * visibility is lost.
     */
    void visibilityChanged() override;
    
    /**
     * Receives notification whenever clock configuration values change.
     * 
     * @param config  The main config file object.
     * 
     * @param key     This should be either the key for the clock visibility 
     *                setting, or for the 12h/24h mode toggle
     */
    void extraConfigValueChanged(ConfigFile* config,String key) override;
    
    //If true, use 24 hour time, if false, use 12 hour AM/PM time.
    bool use24HrMode = false;
    //Indicates if the time should be shown at all.
    bool showClock = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockLabel)
};
