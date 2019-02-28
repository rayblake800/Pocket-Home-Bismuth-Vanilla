#include "QuickSettingsPage.h"
#include "Utils.h"
#include "Audio.h"
#include "Display.h"
#include "Theme_LookAndFeel.h"
#include "Theme_Image_JSONKeys.h"
#include "Layout_Component_JSONKeys.h"
#include "Page_Type.h"

QuickSettingsPage::QuickSettingsPage() :
WindowFocusedTimer("QuickSettingsPage"),
pageListener(*this),
wifiComponent([this]()
{
    pushPageToStack(Page::Type::wifiConnection);
}),
screenBrightnessSlider(Theme::Image::JSONKeys::brightnessSlider),
volumeSlider(Theme::Image::JSONKeys::volumeSlider),
settingsListBtn(Theme::Image::JSONKeys::settingsListBtn),
listButtonManager(&settingsListBtn, 
        Layout::Component::JSONKeys::settingsListBtn)
{
#if JUCE_DEBUG
    setName("QuickSettingsPage");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(10, { RowItem(&wifiComponent) } ),
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
    settingsListBtn.addListener(&pageListener);
    screenBrightnessSlider.setRange(1, 10, 1);
    screenBrightnessSlider.setValue(Display::getBrightness());
    screenBrightnessSlider.addListener(&pageListener);
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setValue(Audio::getVolumePercent());
    volumeSlider.addListener(&pageListener);
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
    if (screenBrightnessSlider == changingSlider)
    {
        Display::setBrightness(changingSlider->getValue());
    }
    else if(volumeSlider == changingSlider)
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
void QuickSettingsPage::PageListener::buttonClicked(juce::Button *button)
{
    if (button == &settingsPage.settingsListBtn)
    {
        settingsPage.pushPageToStack(Page::Type::settingsList);
    }
}

/*
 * Starts a timer to update the slider values as its being dragged.
 */
void QuickSettingsPage::PageListener::sliderDragStarted(juce::Slider* slider)
{
    if (!settingsPage.isTimerRunning())
    {
        settingsPage.changingSlider = slider;
        settingsPage.startTimer(200);
    }
}

/*
 * Stops the timer and immediately updates slider values.
 */
void QuickSettingsPage::PageListener::sliderDragEnded(juce::Slider* slider)
{
    settingsPage.changingSlider = slider;
    settingsPage.timerCallback();
    settingsPage.stopTimer();
    settingsPage.changingSlider = nullptr;
}
