#include "Info_BatteryIcon.h"
#include "Layout_Component_JSONKeys.h"
#include "Theme_Image_JSONKeys.h"

// Battery update frequency in milliseconds:
static const constexpr int timerFrequency = 2000;

// Number of battery percentages to average to get the reported percent:
static const constexpr int percentageCount = 10;

// Number of available images representing battery percentages:
static const constexpr int percentageLevels = 4;

Info::BatteryIcon::BatteryIcon() :
batteryImage(Theme::Image::JSONKeys::batteryIcon),
batteryImageLayout(&batteryImage, Layout::Component::JSONKeys::batteryIcon),
batteryPercentLayout(&batteryPercent,
        Layout::Component::JSONKeys::batteryPercent)
{
#if JUCE_DEBUG
    setName("Info::BatteryIcon");
#endif
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);
    batteryPercent.setJustificationType(juce::Justification::centredLeft);
    if (batteryMonitor.isBatteryAvailable())
    {
        addAndMakeVisible(batteryPercent);
        addAndMakeVisible(batteryImage);
        startTimer(1);
    }
}


// Updates the colour of the BatteryIcon's text.
void Info::BatteryIcon::setTextColour(const juce::Colour newColour)
{
    batteryPercent.setColour(juce::Label::textColourId, newColour);
    batteryPercent.repaint();
}


// Runs applyConfigBounds on all child components, and updates bounds to fit
// the battery text and icon.
void Info::BatteryIcon::applyConfigBounds()
{
    if (batteryMonitor.isBatteryAvailable())
    {
        batteryImageLayout.applyConfigBounds();
        batteryPercentLayout.applyConfigBounds();
        juce::Rectangle<int> childBounds = batteryImage.getBounds()
                .getUnion(batteryPercent.getBounds());
        childBounds.setLeft(0);
        childBounds.setTop(0); if (childBounds != getBounds())
        {
            setBounds(childBounds);
        }
    }
}


// Set the icon's new display status.
void Info::BatteryIcon::setStatus
(BatteryIconImage imageSelection, juce::String percent)
{
    if (batteryMonitor.isBatteryAvailable())
    {
        batteryImage.setImageAssetIndex( (int) imageSelection);
        batteryPercent.setText(percent,
                juce::NotificationType::dontSendNotification);
    }
}


// Enables battery updates when this component becomes visible, and disables
// them when it's hidden.
void Info::BatteryIcon::visibilityChanged()
{
    if (batteryMonitor.isBatteryAvailable())
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
}


// Clears cached battery percentages when the timer is disabled, so that the
// values will catch up more quickly on resume.
void Info::BatteryIcon::onSuspend()
{
    batteryPercents.clear();
}


// Updates the battery percentage.
void Info::BatteryIcon::timerCallback()
{
    Hardware::Battery::Status batteryStatus
            = batteryMonitor.getBatteryStatus();
    int batteryPercent = batteryStatus.percent;
    if (batteryPercent < 0)
    {
        //no battery detected
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
        if (batteryPercents.size() > percentageCount)
        {
            batteryPercents.remove(0);
        }
        int status = std::min<int>(percentageLevels - 1,
                batteryPercent / (100 / percentageLevels));
        jassert(status >= 0 && status < percentageLevels);
        if (batteryStatus.isCharging)
        {
            status += (int) charging0;
        }
        juce::MessageManager::getInstance()->callAsync(
        [this, status, batteryPercent]()
        {
            setStatus((BatteryIconImage) status,
                    juce::String(batteryPercent) + juce::String("%"));
        });
    }
    if (getTimerInterval() != timerFrequency)
    {
        startTimer(timerFrequency);
    }
}
