#pragma once
/** 
 * @file  Info_BatteryIcon.h
 * 
 * @brief  Displays the current charge level and charging state of the battery
 *         as an icon, and writes the battery percentage as text.
 */

#include "Theme_Image_Component.h"
#include "Widgets_DrawableImage.h"
#include "Layout_Component_Manager.h"
#include "WindowFocusedTimer.h"
#include "BatteryMonitor.h"

namespace Info { class BatteryIcon; }

/** 
 * @brief  An icon component and label displaying the current battery level.
 *
 *  While this icon is visible, it will periodically check battery state and 
 * update itself accordingly. To reduce inaccuracies, a rolling average of the
 * last several detected battery percentages is used to create the reported
 * battery percentage.
 */
class Info::BatteryIcon : public juce::Component, private WindowFocusedTimer
{
public:
    BatteryIcon();

    virtual ~BatteryIcon() { }

    /* Juce ColourId values: */
    enum ColourIds
    {
        textColourId = 0x1900800
    };

    /**
     * @brief  Runs applyConfigBounds on all child components, and updates
     *         bounds to fit the battery text and icon.
     */
    void applyConfigBounds();

private:
    /**
     * @brief  All tracked battery states. Each corresponds with an image asset 
     *         file that should be defined in imageAssets.json.
     */
    enum BatteryIconImage
    {
        battery0,
        battery1,
        battery2,
        battery3,
        charging0,
        charging1,
        charging2,
        charging3,
        noBattery
    };

    /**
     * @brief  Sets the icon's new display status.
     * 
     * @param batteryImage  One of the battery resource files loaded from
     *                      the ComponentConfigFile.
     *
     * @param percent       The battery charge percentage.
     */
    void setStatus(const BatteryIconImage imageSelection, 
            const juce::String percent);

    /**
     * @brief  Enables battery updates when this component becomes visible, and
     *         disables them when it's hidden.
     */
    void visibilityChanged() override;
    
    /**
     * @brief  Clears cached battery percentages when the timer is disabled, so 
     *         that the values will catch up more quickly on resume.
     */
    void onSuspend() override;

    /**
     * @brief  Updates the battery percentage.
     */
    virtual void timerCallback() override;

    /* Shows the battery icon */
    Theme::Image::Component<> batteryImage;
    Layout::Component::Manager batteryImageLayout;
    
    /* Shows battery percentage text */
    juce::Label batteryPercent;
    Layout::Component::Manager batteryPercentLayout;
    
    /* Gets battery info */
    BatteryMonitor batteryMonitor;

    /* Holds the last several recorded battery percentages */
    juce::Array<int> batteryPercents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BatteryIcon)
};
