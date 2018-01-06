#include "../../PokeLookAndFeel.h"
#include "../../Configuration/MainConfigFile.h"
#include "../../PocketHomeApplication.h"
#include "ClockMonitor.h"

ClockMonitor::ClockMonitor() :
Configurable(static_cast<ConfigFile*>
(&PocketHomeApplication::getInstance()->getConfig()),{

    MainConfigFile::showClockKey, MainConfigFile::use24HrModeKey
}),
Thread("Clock"),

clock(new Label("clock")),
use24HrMode(false)
{
    clock->setFont(Font(16.5f));
    //clock->setColour(Label::backgroundColourId, PokeLookAndFeel::chipPurple);
    clock->setColour(Label::textColourId, Colours::white);
    clock->setAlwaysOnTop(true);
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
    MainConfigFile * mainConfig = static_cast<MainConfigFile*> (config);
    if (key == MainConfigFile::showClockKey)
    {
        bool visible = mainConfig->getConfigBool(MainConfigFile::showClockKey);
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
    } else if (key == MainConfigFile::use24HrModeKey)
    {
        use24HrMode = mainConfig->getConfigBool(MainConfigFile::use24HrModeKey);
    }
}

Label& ClockMonitor::getLabel()
{
    return *clock;
}
