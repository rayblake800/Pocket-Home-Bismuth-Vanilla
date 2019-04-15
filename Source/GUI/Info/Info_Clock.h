#pragma once
/**
 * @file  Info_Clock.h
 *
 * @brief  Provides a component that displays the current time.
 */

#include "JuceHeader.h"
#include "Windows_FocusedTimer.h"
#include "Config_MainListener.h"

namespace Info { class Clock; }

/**
 * @brief  A configurable label component that displays the current time.
 */
class Info::Clock : public juce::Label, public Config::MainListener,
        public Windows::FocusedTimer
{
public:
    /**
     * @brief  Loads clock settings from Config::MainFile.
     */
    Clock();

    virtual ~Clock() { }

private:
    /**
     * @brief  Sets the label text to the current time.
     */
    void updateTime();

    /**
     * @brief  Updates the displayed time each minute.
     */
    virtual void timerCallback() override;

    /**
     * @brief  Enables the timer when the component becomes visible, disables
     *         it when visibility is lost.
     */
    void visibilityChanged() override;

    /**
     * @brief  Receives notification whenever clock configuration values change.
     *
     * @param key     The updated setting's key, either the clock visibility
     *                setting key, or the 12h/24h mode key.
     */
    void configValueChanged(const juce::Identifier& key) override;

    // If true, use 24 hour time, if false, use 12 hour AM/PM time.
    bool use24HrMode = false;

    // Indicates if the time should be shown at all.
    bool showClock = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Clock)
};
