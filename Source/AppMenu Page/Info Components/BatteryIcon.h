/*
  ==============================================================================

    BatteryIcon.h
    Created: 29 Dec 2017 12:02:24pm
    Author:  anthony
  ==============================================================================
 */

#pragma once
#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../Configuration/Configurables/ConfigurableLabel.h"

#include "BatteryMonitor.h"

/**
 * BatteryIcon displays the current battery state using an icon and percentage
 * text.
 */
class BatteryIcon : public Component {
public:
    BatteryIcon();
    virtual ~BatteryIcon();
    void applyConfigBounds();

private:
    //All tracked battery states.  Each corresponds with an image asset file
    //defined in config.json
    enum BatteryIconImage {
        BATTERY_0,
        BATTERY_1,
        BATTERY_2,
        BATTERY_3,
        CHARGING_0,
        CHARGING_1,
        CHARGING_2,
        CHARGING_3
    };
    /**
     * Set the icon's new display status.
     * @param batteryImage one of the battery resource files defined in
     * config.json
     * @param percent battery charge percentage
     */
    void setStatus(BatteryIconImage imageSelection, String percent);
    
    
    /**
     * @return the BatteryStatus thread object that's tracking battery state
     */
    const BatteryStatus& getBatteryStatus();
    void visibilityChanged() override;
    void resized() override;
    ConfigurableImageComponent batteryImage;
    ConfigurableLabel batteryPercent;
    BatteryMonitor batteryMonitor;
    /**
     * BatteryTimer periodically checks the battery monitor, and uses it
     * to update the battery icon image and percentage text.
     */
    class BatteryTimer : public Timer {
    public:
        BatteryTimer(BatteryIcon * batteryIcon);
        virtual ~BatteryTimer();
        void removeTimer();
    private:
        void timerCallback();
        //NOT a reference pointer because the timer shouldn't keep the icon
        //from being destroyed.
        BatteryIcon * batteryIcon;
        const static int frequency = 1000;
    };
    ScopedPointer<BatteryTimer> batteryTimer;

};