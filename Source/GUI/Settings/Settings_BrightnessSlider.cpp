#include "Settings_BrightnessSlider.h"
#include "Theme_Image_JSONKeys.h"
#include "Display.h"

/* Frequency in milliseconds to run brightness updates while the slider is
 * dragged: */
static const constexpr int updateFreq = 200;

/* Slider value range: */
static const constexpr int minValue = 1;
static const constexpr int maxValue = 10;

/*
 * Constructs the brightness slider frequency and display icons.
 */
Settings::BrightnessSlider::BrightnessSlider() : 
Widgets::DelayedIconSlider(Theme::Image::JSONKeys::brightnessSlider, updateFreq)
{
    setRange(minValue, maxValue, 1);
    setValue(Display::getBrightness());
}

/*
 * Changes the display's brightness when the slider value changes.
 */
void Settings::BrightnessSlider::valueChanged(const double newValue)
{
    Display::setBrightness(newValue);
}
