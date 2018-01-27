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
    batteryMonitor.updateStatus();
    batteryMonitor.startThread();
    batteryTimer = new BatteryTimer(this);
    batteryTimer->startTimer(1);
}

BatteryIcon::~BatteryIcon()
{
    batteryTimer->removeTimer();
    if (batteryMonitor.isThreadRunning())
    {
        batteryMonitor.stopThread(1000);
    }
}

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

const BatteryStatus& BatteryIcon::getBatteryStatus()
{
    return batteryMonitor.getCurrentStatus();
}

void BatteryIcon::visibilityChanged()
{
    if (batteryTimer != nullptr)
    {
        if (isVisible())
        {
            if (!batteryTimer->isTimerRunning())
            {
                batteryTimer->startTimer(10);
            }
        } else
        {
            batteryTimer->stopTimer();
        }
    }
}

void BatteryIcon::resized()
{
}

void BatteryIcon::setStatus(BatteryIconImage imageSelection, String percent)
{
    batteryImage.setImageAssetIndex((int) imageSelection);
    batteryPercent.setText(percent, dontSendNotification);
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
        const BatteryStatus & batteryStatus = batteryIcon->getBatteryStatus();
        int batteryPercent = (int) (batteryStatus.percentage);
        int status = round(((float) batteryPercent) / 100.0f * 3.0f);
        if (status < 0)
        {
            status = 0;
        } else if (status > 2)
        {
            status = 2;
        }
        if (batteryStatus.isCharging)
        {
            status += (int) CHARGING_0;
        }
        batteryIcon->setStatus((BatteryIconImage) status,
                String(batteryPercent) + String("%"));
        int currentFrequency = getTimerInterval();
        if (currentFrequency != frequency && currentFrequency > 0)
        {
            startTimer(frequency);
        }
    } else
    {
        stopTimer();
    }
}
