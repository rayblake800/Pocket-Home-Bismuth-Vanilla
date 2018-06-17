#include "Utils.h"
#include "Audio.h"
#include "Display.h"
#include "PokeLookAndFeel.h"
#include "SettingsPage.h"

SettingsPage::SettingsPage() :
Localized("SettingsPage"),
WindowFocusedTimer("SettingsPage"),
PageComponent("SettingsPage"),
wifiComponent([this]()
{

    pushPageToStack(PageType::WifiSettings);
}),
//bluetoothComponent([this]
//{
//
//    pushPageToStack(PageType::BluetoothSettings);
//}),
screenBrightnessSlider("brightnessIconLo.svg", "brightnessIconHi.svg"),
volumeSlider("volumeIconLo.svg", "volumeIconHi.svg"),
advancedPageButton(localeText(advanced_settings))
{

#    if JUCE_DEBUG
    setName("SettingsPage");
#    endif
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(10, { RowItem(&wifiComponent) } ),
        //Row(10, { RowItem(&bluetoothComponent) } ),
        Row(10, { RowItem(&screenBrightnessSlider) } ),
        Row(10, { RowItem(&volumeSlider) } ),
        Row(10, { RowItem(&advancedPageButton) } )
    });
    layout.setYMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
    advancedPageButton.addListener(this);
    screenBrightnessSlider.setRange(1, 10, 1);
    screenBrightnessSlider.setValue(Display::getBrightness());
    screenBrightnessSlider.addListener(this);
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setValue(Audio::getVolumePercent());
    volumeSlider.addListener(this);
}

/**
 * Cancels the slider timer when visibility is lost.
 */
void SettingsPage::visibilityChanged()
{
    if (!isVisible())
    {
        stopTimer();
        changingSlider = nullptr;
    }
}

/**
 * Used to update the sliders while they're being dragged.
 */
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

/**
 * Opens the advanced settings page when its button is clicked.
 */
void SettingsPage::pageButtonClicked(Button *button)
{
    if (button == &advancedPageButton)
    {
        pushPageToStack(PageType::AdvancedSettings);
    }
}

/**
 * Starts a timer to update the slider values as its being dragged.
 */
void SettingsPage::sliderDragStarted(Slider* slider)
{
    if (!isTimerRunning())
    {
        changingSlider = slider;
        startTimer(200);
    }
}

/**
 * Stops the timer and immediately updates slider values.
 */
void SettingsPage::sliderDragEnded(Slider* slider)
{
    changingSlider = slider;
    timerCallback();
    stopTimer();
    changingSlider = nullptr;
}


