/*
  ==============================================================================

    BatteryIcon.cpp
    Created: 29 Dec 2017 12:02:24pm
    Author:  Anthony Brown

  ==============================================================================
 */
#include "BatteryIcon.h"
#include "../../PocketHomeApplication.h"
#include "../../Utils.h"

BatteryIcon::BatteryIcon() :
batteryImage(ComponentConfigFile::batteryIconKey),
batteryPercent(ComponentConfigFile::batteryPercentKey),
batteryTimer(new BatteryTimer(this))
{
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);
    batteryPercent.setJustificationType(Justification::centredLeft);
    addAndMakeVisible(batteryPercent);
    addAndMakeVisible(batteryImage);
    batteryTimer = new BatteryTimer(this);
    batteryTimer->startTimer(1);
}

BatteryIcon::~BatteryIcon()
{
    batteryTimer->removeTimer();
}

/**
 * Run applyConfigBounds on all child components, and update bounds to
 * fit children.
 */
void BatteryIcon::applyConfigBounds()
{
    batteryImage.applyConfigBounds();
    batteryPercent.applyConfigBounds();
    Rectangle<int> childBounds = batteryImage.getBounds()
            .getUnion(batteryPercent.getBounds());
    childBounds.setLeft(0);
    childBounds.setTop(0);
    if (childBounds != getBounds())
    {
        setBounds(childBounds);
    }
}

/**
 * Set the icon's new display status.
 */
void BatteryIcon::setStatus(BatteryIconImage imageSelection, String percent)
{
    batteryImage.setImageAssetIndex((int) imageSelection);
    batteryPercent.setText(percent, dontSendNotification);
}

/**
 * Turn battery updates on when this component becomes visible, off
 * when it's hidden.
 */
void BatteryIcon::visibilityChanged()
{
    if (batteryTimer != nullptr)
    {
        if (isVisible())
        {
            if (!batteryTimer->isTimerRunning())
            {
                batteryTimer->startTimer(1);
            }
        } else
        {
            batteryTimer->stopTimer();
        }
    }
}

BatteryIcon::BatteryTimer::BatteryTimer(BatteryIcon * batteryIcon) :
batteryIcon(batteryIcon)
{
}

BatteryIcon::BatteryTimer::~BatteryTimer()
{
    removeTimer();
}

void BatteryIcon::BatteryTimer::removeTimer()
{
    batteryIcon = nullptr;
    stopTimer();
}

void BatteryIcon::BatteryTimer::timerCallback()
{
    if (batteryIcon != nullptr)
    {
        BatteryMonitor::BatteryStatus batteryStatus =
                batteryIcon->batteryMonitor.getBatteryStatus();
        int batteryPercent = batteryStatus.percent;
        if (batteryPercent < 0)
        {//no battery info loaded
            batteryIcon->setStatus(noBattery, "");
        } else
        {
            int status = round(((float) batteryPercent) / 100.0f * 3.0f);
            jassert(status >= 0 && status < 4);
            if (batteryStatus.isCharging)
            {
                status += (int) charging0;
            }
            batteryIcon->setStatus((BatteryIconImage) status,
                    String(batteryPercent) + String("%"));
        }
        if (getTimerInterval() != frequency)
        {
            startTimer(frequency);
        }

    } else
    {
        stopTimer();
    }
}
