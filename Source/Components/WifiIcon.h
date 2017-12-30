/*
  ==============================================================================

    WifiIcon.h
    Created: 29 Dec 2017 12:02:41pm
    Author:  anthony

  ==============================================================================
 */

#pragma once
#include "Basic/VectorImageButton.h"

class WifiIcon : public VectorImageButton, public ReferenceCountedObject {
public:
    WifiIcon();
    virtual ~WifiIcon();

    enum WifiIconImage {
        WIFI_OFF,
        WIFI_STRENGTH_0,
        WIFI_STRENGTH_1,
        WIFI_STRENGTH_2,
        WIFI_STRENGTH_3
    };

    typedef ReferenceCountedObjectPtr<WifiIcon> ReferencePtr;
    void setStatus(WifiIconImage wifiState);
private:
    void visibilityChanged() override;
    class WifiTimer : public Timer {
    public:
        WifiTimer(WifiIcon * wifiIcon);
        virtual ~WifiTimer();
        void removeTimer();
    private:
        void timerCallback();
        //NOT a reference pointer because the timer shouldn't keep the icon
        //from being destroyed.
        WifiIcon * wifiIcon;
        const static int frequency = 2000;
    };
    ScopedPointer<WifiTimer> wifiTimer;

};