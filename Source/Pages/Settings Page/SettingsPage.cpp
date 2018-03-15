#include "Utils.h"
#include "Audio.h"
#include "Display.h"
#include "PokeLookAndFeel.h"
#include "SettingsPage.h"

SettingsPage::SettingsPage() :
WindowFocusedTimer("SettingsPage"),
PageComponent("SettingsPage",{
    {1,
        {
            {&wifiComponent, 1}
        }},
    //    {1,
    //        {
    //            {&bluetoothComponent, 1}
    //        }},
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
wifiComponent([this]
{

    pushPageToStack(&wifiSettingsPage,
            PageStackComponent::kTransitionTranslateHorizontal);
}),
//bluetoothComponent([this]
//{
//
//    pushPageToStack(&bluetoothSettingsPage,
//            PageStackComponent::kTransitionTranslateHorizontal);
//}),
screenBrightnessSlider("brightnessIconLo.svg", "brightnessIconHi.svg"),
volumeSlider("volumeIconLo.svg", "volumeIconHi.svg"),
advancedPageButton("Advanced Settings")
{

#    if JUCE_DEBUG
    setName("SettingsPage");
#    endif

    addAndShowLayoutComponents();
    setColour(backgroundColourId, Colour(0xffd23c6d));
    advancedPageButton.addListener(this);
    brightness = Display::getBrightness();
    volume = Audio::getVolumePercent();
    screenBrightnessSlider.setRange(0,10,1);
    screenBrightnessSlider.setValue(brightness);
    screenBrightnessSlider.addListener(this);   
    volumeSlider.setRange(0,100,1);
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
        Display::setBrightness(changingSlider->getValue());
    }
    if (volumeSlider.ownsSlider(changingSlider))
    {
        Audio::setVolume(changingSlider->getValue());
    }
    startTimer(200);
}

void SettingsPage::pageButtonClicked(Button *button)
{
    if (button == &advancedPageButton)
    {
        pushPageToStack(&advancedSettingsPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    }
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
    changingSlider = slider;
    timerCallback();
    stopTimer();
    changingSlider = nullptr;
}


