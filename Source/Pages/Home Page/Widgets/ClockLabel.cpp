#include "ComponentConfigFile.h"
#include "ClockLabel.h"

ClockLabel::ClockLabel() :
WindowFocusedTimer("ClockLabel"),
ConfigurableLabel(ComponentConfigFile::clockLabelKey, "clockLabel", "00:00")
{
    
#if JUCE_DEBUG
    setName("ClockLabel");
#endif
    ComponentConfigFile config;
    addTrackedKeys({
        ComponentConfigFile::use24HrModeKey, ComponentConfigFile::showClockKey
    });
    setJustificationType(Justification::centredRight);
    loadAllConfigProperties();
    startTimer(1);
}

ClockLabel::~ClockLabel()
{
    stopTimer();
}

/**
 * Updates the displayed time each minute.
 */
void ClockLabel::timerCallback()
{
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
        ComponentConfigFile config;
        if (!config.getConfigValue<bool>(ComponentConfigFile::showClockKey))
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
void ClockLabel::loadExtraConfigProperties
(ConfigFile* config, String key)
{
    ComponentConfigFile* componentConfig =
            dynamic_cast<ComponentConfigFile*> (config);
    if (componentConfig != nullptr)
    {
        if (key == ComponentConfigFile::showClockKey)
        {
            bool visible = componentConfig->getConfigValue<bool>
                    (ComponentConfigFile::showClockKey);
            MessageManager::callAsync([this, visible]
            {
                setAlpha(visible ? 1 : 0);
                if (visible && !isTimerRunning())
                {
                    startTimer(1);
                }
                else if (!visible && isTimerRunning())
                {
                    stopTimer();
                }
            });
        }
        else if (key == ComponentConfigFile::use24HrModeKey)
        {
            use24HrMode = componentConfig->getConfigValue<bool>
                    (ComponentConfigFile::use24HrModeKey);
            if (isVisible() && getAlpha() != 0)
            {
                stopTimer();
                startTimer(1);
            }
        }
    }
}

