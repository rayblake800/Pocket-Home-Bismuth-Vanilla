#pragma once
/**
 * @file  Settings_BrightnessSlider.h
 *
 * @brief  A UI component used to adjust the display brightness.
 */

#include "Widgets_DelayedIconSlider.h"

namespace Settings { class BrightnessSlider; }

/**
 * @brief  Updates the display brightness, avoiding overly frequent updates.
 */
class Settings::BrightnessSlider : public Widgets::DelayedIconSlider
{
public:
    /**
     * @brief  Constructs the brightness slider using an appropriate update
     *         frequency and display icons.
     */
    BrightnessSlider();

    virtual ~BrightnessSlider() { }

private:
    /**
     * @brief  Changes the display's brightness when the slider value changes.
     *
     * @param newValue  The updated slider value.
     */
    virtual void valueChanged(const double newValue) override;
};
