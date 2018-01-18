#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include "../../PokeLookAndFeel.h"
#include "../../Configuration/ComponentConfigFile.h"
#include "../../PocketHomeApplication.h"
#include "ClockMonitor.h"

ClockMonitor::ClockMonitor() :
Configurable(static_cast<ConfigFile*>
(&PocketHomeApplication::getInstance()->getComponentConfig()),{
    ComponentConfigFile::showClockKey, ComponentConfigFile::use24HrModeKey
}),
Thread("Clock"),
clockLabel(ComponentConfigFile::clockLabelKey,"clockLabel","00:00"),
use24HrMode(false)
{
    clockLabel.setJustificationType(Justification::centredRight);
    loadAllConfigProperties();
}

ClockMonitor::~ClockMonitor()
{
}

void ClockMonitor::run()
{
    while (!threadShouldExit())
    {
        Time timeNow = Time::getCurrentTime();
        String hours = String(use24HrMode 
                ? timeNow.getHours() : timeNow.getHoursInAmPmFormat());
        String minutes= String(timeNow.getMinutes());
        while(minutes.length()<2)
        {
            minutes=String("0")+minutes;
        }
        String timeStr = hours + String(":")+minutes;
        if(!use24HrMode)
        {
            timeStr += String(timeNow.isAfternoon() ? " PM" : " AM");
        } 
        MessageManager::callAsync([this,&timeStr]
        {
            clockLabel.setText(timeStr,
                    NotificationType::dontSendNotification);
        });
        Thread::sleep(1000);
    }
}

/**
 * Receives notification whenever clock configuration values change
 */
void ClockMonitor::loadConfigProperties(ConfigFile * config, String key)
{
    ComponentConfigFile * componentConfig =
            static_cast<ComponentConfigFile*> (config);
    if (key == ComponentConfigFile::showClockKey)
    {
        bool visible = componentConfig->getConfigBool
                (ComponentConfigFile::showClockKey);
        MessageManager::callAsync([this, visible]
        {
            clockLabel.setAlpha(visible ? 1 : 0);
            bool threadRunning = isThreadRunning();
            if (visible && !threadRunning)
            {
                startThread();
            } else if (!visible && threadRunning)
            {
                stopThread(1000);
            }
        });
    } else if (key == ComponentConfigFile::use24HrModeKey)
    {
        use24HrMode = componentConfig->getConfigBool
                (ComponentConfigFile::use24HrModeKey);
    }
}

ConfigurableLabel& ClockMonitor::getLabel()
{
    return clockLabel;
}
