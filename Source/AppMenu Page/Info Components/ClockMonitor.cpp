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

clock(new Label("clock")),
use24HrMode(false)
{
    clock->setFont(Font(16.5f));
    //clock->setColour(Label::backgroundColourId, PokeLookAndFeel::chipPurple);
    clock->setColour(Label::textColourId, Colours::white);
    loadAllConfigProperties();
}

ClockMonitor::~ClockMonitor()
{
}

void ClockMonitor::run()
{
    while (!threadShouldExit())
    {
        struct timeval tv;
        int error = gettimeofday(&tv, nullptr);
        if (error) perror("Time of the day");
        struct tm res;
        localtime_r(&tv.tv_sec, &res);
        if (use24HrMode)
            sprintf(formatted, "%02d:%02d", res.tm_hour, res.tm_min);
        else
        {
            const char* moment = (res.tm_hour > 12) ? "pm" : "am";
            int hour = (res.tm_hour > 12) ? res.tm_hour - 12 : res.tm_hour;
            sprintf(formatted, "%02d:%02d %s", hour, res.tm_min, moment);
        }
        MessageManager::callAsync([this]
        {
            clock->setText(String(formatted),
                    NotificationType::dontSendNotification);
            //int width = Font(16.5f).getStringWidth(clock->getText(false));
            //int height = Font(16.5f).getHeight();
            //Rectangle<int> bounds = clock->getBounds();
            //clock->setBounds(bounds.withSizeKeepingCentre(width, height));
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
            clock->setAlpha(visible ? 1 : 0);
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

Label& ClockMonitor::getLabel()
{
    return *clock;
}
