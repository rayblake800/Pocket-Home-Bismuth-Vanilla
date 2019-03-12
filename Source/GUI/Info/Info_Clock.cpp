#include "Info_Clock.h"
#include "Config_MainFile.h"
#include "Config_MainKeys.h"
#include "Layout_Component_ConfigFile.h"
#include "Layout_Component_JSONKeys.h"

/*
 * Loads clock settings from Config::MainFile.
 */
Info::Clock::Clock() :
juce::Label("Info::Clock", ""),
WindowFocusedTimer("Info::Clock")
{
#if JUCE_DEBUG
    setName("ClockLabel");
#endif
    addTrackedKey(Config::MainKeys::use24HrModeKey);
    addTrackedKey(Config::MainKeys::showClockKey);
    setJustificationType(juce::Justification::centredRight);
    setColour(juce::Label::textColourId, findColour(textColourId));
    loadAllConfigProperties();
    if(showClock)
    {
        timerCallback();
    }
}

/*
 * Sets the label text to the current time.
 */
void Info::Clock::updateTime()
{
    if(showClock)
    {
        using juce::String;
        juce::Time timeNow = juce::Time::getCurrentTime();
        String hours = String(use24HrMode
                ? timeNow.getHours() : timeNow.getHoursInAmPmFormat());
        String minutes = String(timeNow.getMinutes());
        while (minutes.length() < 2)
        {
            minutes = String("0") + minutes;
        }
        String timeStr = hours + String(":") + minutes;
        if (!use24HrMode)
        {
            timeStr += String(timeNow.isAfternoon() ? " PM" : " AM");
        }
        setText(timeStr, juce::NotificationType::dontSendNotification);
    }
    else
    {
        setText("", juce::NotificationType::dontSendNotification);
    }
}

/*
 * Updates the displayed time each minute.
 */
void Info::Clock::timerCallback()
{
    updateTime();
    startTimer(60000 - juce::Time::getCurrentTime().getSeconds() * 1000);
}

/*
 * Enable the timer when the component becomes visible, disable it when
 * visibility is lost.
 */
void Info::Clock::visibilityChanged()
{
    if (isVisible())
    {
        if (!showClock)
        {
            setAlpha(0);
            stopTimer();
        }
        else if (!isTimerRunning())
        {
            startTimer(1);
        }
    }
    else
    {
        stopTimer();
    }
}

/*
 * Receives notification whenever clock configuration values change
 */
void Info::Clock::configValueChanged(const juce::Identifier& key)
{
    Config::MainFile config;
    if (key == Config::MainKeys::showClockKey)
    {
        showClock = config.getShowClock();
        juce::MessageManager::callAsync([this]
        {
            setAlpha(showClock ? 1 : 0);
            if(showClock && !isTimerRunning())
            {
                startTimer(1);
            }
            else if(!showClock && isTimerRunning())
            {
                stopTimer();
            }
        });
    }
    else if (key == Config::MainKeys::use24HrModeKey)
    {
        use24HrMode = config.get24HourEnabled();
        updateTime();
    }
}
