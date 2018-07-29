#include "MainConfigFile.h"
#include "ClockLabel.h"

ClockLabel::ClockLabel() :
WindowFocusedTimer("ClockLabel"),
ConfigurableLabel(ComponentConfigFile::clockLabelKey, "clockLabel", "00:00")
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("ClockLabel");
#    endif
    MainConfigFile config;
    config.addListener(this,{
        MainConfigFile::use24HrModeKey,
        MainConfigFile::showClockKey
    });
    setJustificationType(Justification::centredRight);
    loadAllConfigProperties();
    startTimer(1);
}

/**
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

/**
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

/**
 * Receives notification whenever clock configuration values change
 */
void ClockLabel::extraConfigValueChanged(juce::String key)
{
    using namespace juce;
    MainConfigFile config;
    if (key == MainConfigFile::showClockKey)
    {
        showClock = config.getConfigValue<bool>
                (MainConfigFile::showClockKey);
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
    else if (key == MainConfigFile::use24HrModeKey)
    {
        use24HrMode = config.getConfigValue<bool>
                (MainConfigFile::use24HrModeKey);
        if (isVisible() && getAlpha() != 0)
        {
            stopTimer();
            startTimer(1);
        }
    }
}

