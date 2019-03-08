#include "Settings_VolumeSlider.h"
#include "Theme_Image_JSONKeys.h"
#include "Audio.h"

/* Frequency in milliseconds to run volume updates while the slider is 
 * dragged: */
static const constexpr int updateFreq = 200;

/* Slider value range: */
static const constexpr int minValue = 1;
static const constexpr int maxValue = 100;
/*
 * Constructs the volume slider frequency and display icons.
 */
Settings::VolumeSlider::VolumeSlider() :
Widgets::DelayedIconSlider(Theme::Image::JSONKeys::volumeSlider, updateFreq)
{
    setRange(minValue, maxValue, 1);
    setValue(Audio::getVolumePercent());
}

/*
 * Changes the system volume when the slider value changes.
 */
void Settings::VolumeSlider::valueChanged(const double newValue)
{
    Audio::setVolume(newValue);
}
