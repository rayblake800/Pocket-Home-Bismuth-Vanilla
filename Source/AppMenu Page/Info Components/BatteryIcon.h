/* 
 * @file   BatteryIcon.h
 * @author Anthony Brown
 * 
 * BatteryIcon displays the current charge level and charging state of 
 * the PocketCHIP battery as a component icon, and writes the battery percentage
 * as text. While this icon is visible, it will periodically check battery state
 *  and update itself accordingly.
 *
*/

#pragma once
#include "../../Configuration/Configurables/ConfigurableImageComponent.h"
#include "../../Configuration/Configurables/ConfigurableLabel.h"
#include "BatteryMonitor.h"

class BatteryIcon : public Component {
public:
    BatteryIcon();
    virtual ~BatteryIcon();
    
    /**
     * Run applyConfigBounds on all child components, and update bounds to
     * fit children.
     */
    void applyConfigBounds();

private:
    //All tracked battery states.  Each corresponds with an image asset file
    //defined in config.json
    enum BatteryIconImage {
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
     * Set the icon's new display status.
     * @param batteryImage one of the battery resource files defined in
     * config.json
     * @param percent battery charge percentage
     */
    void setStatus(BatteryIconImage imageSelection, String percent);
 
    /**
     * Turn battery updates on when this component becomes visible, off
     * when it's hidden.
     */
    void visibilityChanged() override;

    //Shows the battery icon
    ConfigurableImageComponent batteryImage;
    //Shows battery percentage text
    ConfigurableLabel batteryPercent;
    //Gets battery info
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