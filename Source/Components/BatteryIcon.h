/*
  ==============================================================================

    BatteryIcon.h
    Created: 29 Dec 2017 12:02:24pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "Basic/VectorImageButton.h"
#include "../BatteryMonitor.h"

class BatteryIcon : public VectorImageButton, public ReferenceCountedObject {
public:
    BatteryIcon();
    virtual ~BatteryIcon();

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

    typedef ReferenceCountedObjectPtr<BatteryIcon> ReferencePtr;
    void setStatus(BatteryIconImage batteryImage, String percent);
    const BatteryStatus& getBatteryStatus();
    void stopUpdates();
private:
    void visibilityChanged() override;
    void resized() override;
    ScopedPointer<Label> batteryLabel;
    BatteryMonitor batteryMonitor;

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