#include "MainConfigKeys.h"
#include "ComponentConfigKeys.h"
#include "ClockLabel.h"

ClockLabel::ClockLabel() :
WindowFocusedTimer("ClockLabel"),
ConfigurableLabel(ComponentConfigKeys::clockLabelKey, "clockLabel", "00:00")
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("ClockLabel");
#    endif
    addTrackedKey(MainConfigKeys::use24HrModeKey);
    addTrackedKey(MainConfigKeys::showClockKey);
    setJustificationType(Justification::centredRight);
    loadAllConfigProperties();
    startTimer(1);
}

/*
 * Updates the displayed time each minute.
 */
void ClockLabel::timerCallback()
{
    using namespace juce;
    Time timeNow = Time::getCurrentTime();
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
    setText(timeStr, NotificationType::dontSendNotification);
    startTimer(60000 - timeNow.getSeconds()*1000);
}

/*
 * Enable the timer when the component becomes visible, disable it when
 * visibility is lost.
 */
void ClockLabel::visibilityChanged()
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
void ClockLabel::configValueChanged(const juce::Identifier& key)
{
    using namespace juce;
    MainConfigFile config;
    if (key == MainConfigKeys::showClockKey)
    {
        showClock = config.getConfigValue<bool>(key);
        MessageManager::callAsync([this]
        {
            setAlpha(showClock ? 1 : 0);
            if (showClock && !isTimerRunning())
            {
                startTimer(1);
            }
            else if (!showClock && isTimerRunning())
            {
                stopTimer();
            }
        });
    }
    else if (key == MainConfigKeys::use24HrModeKey)
    {
        use24HrMode = config.getConfigValue<bool>(key);
        if (isVisible() && getAlpha() != 0)
        {
            stopTimer();
            startTimer(1);
        }
    }
}

