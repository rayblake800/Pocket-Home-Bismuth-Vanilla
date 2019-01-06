#include "QuickSettingsPage.h"
#include "Utils.h"
#include "Audio.h"
#include "Display.h"
#include "Theme_LookAndFeel.h"
#include "Theme_Image_JSONKeys.h"
#include "Layout_Component_JSONKeys.h"

/* Slider image assets.  TODO: define these in components.json. */
static const constexpr char* brightnessLowIcon = "brightnessIconLo.svg";
static const constexpr char* brightnessHighIcon = "brightnessIconHi.svg";
static const constexpr char* volumeLowIcon = "volumeIconLo.svg";
static const constexpr char* volumeHighIcon = "volumeIconHi.svg";

QuickSettingsPage::QuickSettingsPage() :
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
screenBrightnessSlider(brightnessLowIcon, brightnessHighIcon),
volumeSlider(volumeLowIcon, volumeHighIcon),
settingsListBtn(Theme::Image::JSONKeys::settingsListBtn),
listButtonManager(&settingsListBtn, 
        Layout::Component::JSONKeys::settingsListBtn)
{

#    if JUCE_DEBUG
    setName("QuickSettingsPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(10, { RowItem(&wifiComponent) } ),
        //Row(10, { RowItem(&bluetoothComponent) } ),
        Row(10, { RowItem(&screenBrightnessSlider) } ),
        Row(10, { RowItem(&volumeSlider) } )
    });

    layout.setXMarginFraction(
            1.0 - listButtonManager.getLayout().getXFraction());
    layout.setYMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    
    addAndMakeVisible(settingsListBtn);
    settingsListBtn.addListener(this);
    screenBrightnessSlider.setRange(1, 10, 1);
    screenBrightnessSlider.setValue(Display::getBrightness());
    screenBrightnessSlider.addListener(this);
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setValue(Audio::getVolumePercent());
    volumeSlider.addListener(this);
}

/*
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

/*
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

/*
 * Updates the advanced settings button when the page is resized.
 */
void QuickSettingsPage::pageResized()
{
    listButtonManager.applyConfigBounds();
}

/*
 * Opens the advanced settings page when its button is clicked.
 */
void QuickSettingsPage::pageButtonClicked(juce::Button *button)
{
    if (button == &settingsListBtn)
    {
        pushPageToStack(PageType::SettingsList);
    }
}

/*
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

/*
 * Stops the timer and immediately updates slider values.
 */
void QuickSettingsPage::sliderDragEnded(juce::Slider* slider)
{
    changingSlider = slider;
    timerCallback();
    stopTimer();
    changingSlider = nullptr;
}
