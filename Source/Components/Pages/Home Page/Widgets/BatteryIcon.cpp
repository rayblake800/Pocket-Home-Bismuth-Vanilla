#include "BatteryIcon.h"
#include "Utils.h"

BatteryIcon::BatteryIcon() : WindowFocusedTimer("BatteryIcon"),
batteryImage(ComponentConfigFile::batteryIconKey),
batteryPercent(ComponentConfigFile::batteryPercentKey)
{
    using namespace juce;

#    if JUCE_DEBUG
    setName("BatteryIcon");
#    endif
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);
    batteryPercent.setJustificationType(Justification::centredLeft);
    addAndMakeVisible(batteryPercent);
    addAndMakeVisible(batteryImage);
    startTimer(1);
}

/**
 * Run applyConfigBounds on all child components, and update bounds to
 * fit children.
 */
void BatteryIcon::applyConfigBounds()
{
    using namespace juce;
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
void BatteryIcon::setStatus
(BatteryIconImage imageSelection, juce::String percent)
{
    using namespace juce;
    batteryImage.setImageAssetIndex((int) imageSelection);
    batteryPercent.setText(percent, dontSendNotification);
}

/**
 * Turn battery updates on when this component becomes visible, off
 * when it's hidden.
 */
void BatteryIcon::visibilityChanged()
{
    if (isVisible())
    {
        if (!isTimerRunning())
        {
            startTimer(1);
        }
    }
    else
    {
        stopTimer();
    }
}

/**
 * Clear cached battery percentages when the timer is disabled, so that
 * the values will catch up more quickly on resume.
 */
void BatteryIcon::onSuspend()
{
    batteryPercents.clear();
}

void BatteryIcon::timerCallback()
{
    using namespace juce;
    BatteryMonitor::BatteryStatus batteryStatus =
            batteryMonitor.getBatteryStatus();
    int batteryPercent = batteryStatus.percent;
    if (batteryPercent < 0)
    {//no battery info loaded
        setStatus(noBattery, "");
    }
    else
    {
        batteryPercents.add(batteryPercent);
        batteryPercent = 0;
        for (const int& percent : batteryPercents)
        {
            batteryPercent += percent;
        }
        batteryPercent /= batteryPercents.size();
        if (batteryPercents.size() > 10)
        {
            batteryPercents.remove(0);
        }
        int status = round(((float) batteryPercent) / 100.0f * 3.0f);
        jassert(status >= 0 && status < 4);
        if (batteryStatus.isCharging)
        {
            status += (int) charging0;
        }
        setStatus((BatteryIconImage) status,
                String(batteryPercent) + String("%"));
    }
    if (getTimerInterval() != frequency)
    {
        startTimer(frequency);
    }
}
