#include "../../PocketHomeApplication.h"
#include "../../Utils.h"
#include "../../PokeLookAndFeel.h"
#include "SettingsPage.h"

SettingsPage::SettingsPage() :
PageComponent("SettingsPage",{
    {1,
        {
            {&wifiCategoryItem, 1}
        }},
    {1,
        {
            {&screenBrightnessSlider, 1}
        }},
    {1,
        {
            {&volumeSlider, 1}
        }},
    {1,
        {
            {&advancedPageButton, 1}
        }}
}, true),
wifiCategoryItem(),
screenBrightnessSlider("brightnessIconLo.svg", "brightnessIconHi.svg"),
volumeSlider("volumeIconLo.svg", "volumeIconHi.svg"),
advancedPageButton("Advanced Settings")
{
    addAndShowLayoutComponents();
    setColour(backgroundColourId, Colour(0xffd23c6d));
    advancedPageButton.addListener(this);
    brightness = 8;
    volume = 90;
#if JUCE_LINUX
    // Get initial brightness value
    ChildProcess child;
    if (child.start("cat /sys/class/backlight/backlight/brightness"))
    {
        String result{child.readAllProcessOutput()};
        brightness = result.getIntValue();
    };
    // Get initial volume value
    StringArray cmd{ "amixer", "sget", "Power Amplifier"};
    if (child.start(cmd))
    {
        const String result(child.readAllProcessOutput());
        int resultIndex = result.indexOf("[") + 1;
        child.waitForProcessToFinish(5 * 1000);
        char buff[4];
        for (int i = 0; i < 4; i++)
        {
            char c = result[resultIndex + i];
            if (c >= '0' && c <= '9')
            {
                buff[i] = c;
            }
            else
            {
                buff[i] = (char) 0;
            }
        }
        String newVol = String(buff);
        volume = newVol.getIntValue();
    }
#endif
    PocketHomeApplication::getInstance()
            ->getWifiStatus().addListener(&wifiCategoryItem);
    screenBrightnessSlider.setValue(brightness);
    screenBrightnessSlider.addListener(this);
    volumeSlider.setValue(volume);
    volumeSlider.addListener(this);
}

SettingsPage::~SettingsPage() { }

void SettingsPage::visibilityChanged()
{
    if (!isVisible())
    {
        stopTimer();
        changingSlider = nullptr;
    }
}

void SettingsPage::timerCallback()
{
    if (screenBrightnessSlider.ownsSlider(changingSlider))
    {
        setScreenBrightness();
    }
    if (volumeSlider.ownsSlider(changingSlider))
    {
        setSoundVolume();
    }
    changingSlider = nullptr;
}

void SettingsPage::pageButtonClicked(Button *button)
{
    if (button == &advancedPageButton)
    {
        PocketHomeApplication::getInstance()->getMainStack().pushPage
                (&advancedSettingsPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    }
}

void SettingsPage::setSoundVolume()
{
    volume = volumeSlider.getValue();
#if JUCE_LINUX
    StringArray cmd{"amixer", "sset", "Power Amplifier", (String(volume) + "%").toRawUTF8()};
    ChildProcess child;
    if (child.start(cmd))
    {

        String result{child.readAllProcessOutput()};
    }

#endif
}

void SettingsPage::setScreenBrightness()
{
    brightness = 1 + (screenBrightnessSlider.getValue()*0.09);
#if JUCE_LINUX
    ChildProcess child;
    StringArray cmd{"sh", "-c", (String("echo ") + String(brightness) + String(" > /sys/class/backlight/backlight/brightness")).toRawUTF8()};
    if (child.start(cmd))
    {

        String result{child.readAllProcessOutput()};
        DBG(result);
    }
#endif
}

void SettingsPage::sliderDragStarted(Slider* slider)
{
    if (!isTimerRunning())
    {
        changingSlider = slider;
        startTimer(200);
    }
}

void SettingsPage::sliderDragEnded(Slider* slider)
{
    if (screenBrightnessSlider.ownsSlider(slider) && isTimerRunning())
    {
        stopTimer();
        setScreenBrightness();
    }
    else if (volumeSlider.ownsSlider(slider) && isTimerRunning())
    {
        stopTimer();
        setSoundVolume();
    }
    changingSlider = nullptr;
}


