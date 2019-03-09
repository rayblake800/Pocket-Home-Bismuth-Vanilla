#pragma once
/**
 * @file  Settings_VolumeSlider.h
 *
 * @brief  A UI component used to adjust the system volume.
 */

#include "Widgets_DelayedIconSlider.h"

namespace Settings { class VolumeSlider; }

/**
 * @brief  Updates the system volume, avoiding overly frequent updates.
 */
class Settings::VolumeSlider : public Widgets::DelayedIconSlider
{
public:
    /**
     * @brief  Constructs the volume slider using an appropriate update
     *         frequency and display icons.
     */
    VolumeSlider();

    virtual ~VolumeSlider() { }

private:
    /**
     * @brief  Changes the system volume when the slider value changes.
     *
     * @param newValue  The updated slider value.
     */
    virtual void valueChanged(const double newValue) override;
};
