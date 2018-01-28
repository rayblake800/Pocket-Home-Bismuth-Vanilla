#include "../../Configuration/ComponentConfigFile.h"
#include "../../PocketHomeApplication.h"
#include "ClockLabel.h"

ClockLabel::ClockLabel() :
ConfigurableLabel(ComponentConfigFile::clockLabelKey, "clockLabel", "00:00"),
use24HrMode(false)
{
    ComponentConfigFile& config =
            PocketHomeApplication::getInstance()->getComponentConfig();
    config.registerConfigurable(this,{
        ComponentConfigFile::showClockKey, ComponentConfigFile::use24HrModeKey
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
        if (!isTimerRunning())
        {
            startTimer(1);
        }

    } else
    {
        stopTimer();
    }
}


/**
 * Receives notification whenever clock configuration values change
 */
void ClockLabel::loadConfigProperties(ConfigFile * config, String key)
{
    ComponentConfigFile * componentConfig =
            static_cast<ComponentConfigFile*> (config);
    if (key == ComponentConfigFile::showClockKey)
    {
        bool visible = componentConfig->getConfigBool
                (ComponentConfigFile::showClockKey);
        MessageManager::callAsync([this, visible]
        {
            setAlpha(visible ? 1 : 0);
            if (visible && !isTimerRunning())
            {
                startTimer(1);
            } else if (!visible && isTimerRunning())
            {
                stopTimer();
            }
        });
    } else if (key == ComponentConfigFile::use24HrModeKey)
    {
        use24HrMode = componentConfig->getConfigBool
                (ComponentConfigFile::use24HrModeKey);
    } else
    {
        this->ConfigurableLabel::loadConfigProperties(config, key);
    }
}

