/**
 * @file ClockLabel.h
 * 
 * ClockLabel is a component managed by the ComponentConfigFile that displays
 * the current time.
 */

#pragma once
#include "../../../../JuceLibraryCode/JuceHeader.h"
#include "../../../WindowFocusedTimer.h"
#include "../../../Configuration/Configurables/ConfigurableLabel.h"

class ClockLabel : public ConfigurableLabel, private WindowFocusedTimer{
public:
    ClockLabel();
    ~ClockLabel();

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
     * Receives notification whenever clock configuration values change
     * @param config the main config file
     * @param key is the key for the clock visibility setting or
     * for the 12h/24h mode toggle
     */
    void loadExtraConfigProperties(ConfigFile* config,String key) override;
    
    //If true, use 24 hour time, if false, use 12 hour AM/PM time.
    bool use24HrMode = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClockLabel)
};
