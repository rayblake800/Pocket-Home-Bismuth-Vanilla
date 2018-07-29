#include "Utils.h"
#include "Audio.h"
#include "Display.h"
#include "PokeLookAndFeel.h"
#include "QuickSettingsPage.h"

QuickSettingsPage::QuickSettingsPage() :
Localized("QuickSettingsPage"),
WindowFocusedTimer("QuickSettingsPage"),
PageComponent("QuickSettingsPage"),
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
    setName("QuickSettingsPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
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
    advancedPageButton.setMaxTextScale(ComponentConfigFile::mediumText);
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
void QuickSettingsPage::visibilityChanged()
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
void QuickSettingsPage::timerCallback()
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
void QuickSettingsPage::pageButtonClicked(juce::Button *button)
{
    if (button == &advancedPageButton)
    {
        pushPageToStack(PageType::SettingsList);
    }
}

/**
 * Starts a timer to update the slider values as its being dragged.
 */
void QuickSettingsPage::sliderDragStarted(juce::Slider* slider)
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
void QuickSettingsPage::sliderDragEnded(juce::Slider* slider)
{
    changingSlider = slider;
    timerCallback();
    stopTimer();
    changingSlider = nullptr;
}


