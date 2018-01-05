/*
  ==============================================================================

    BatteryIcon.cpp
    Created: 29 Dec 2017 12:02:24pm
    Author:  anthony

  ==============================================================================
 */
#include "BatteryIcon.h"
#include "../../Configuration/ConfigFile.h"
#include "../../Utils.h"

BatteryIcon::BatteryIcon() :
VectorImageButton(ConfigFile::getInstance()->
getComponentSettings(BATTERY), "Battery")
{
    setInterceptsMouseClicks(false, false);
    setWantsKeyboardFocus(false);
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings iconSettings =
            config->getComponentSettings(BATTERY);
    ConfigFile::ComponentSettings textSettings =
            config->getComponentSettings(BATTERY_PERCENT);
    batteryLabel = new Label("percentage", "-%");
    batteryLabel->setJustificationType(Justification::centredLeft);
    Rectangle<int> bounds = iconSettings.getBounds();
    Rectangle<int> textBounds = textSettings.getBounds();
    bounds = bounds.getUnion(textBounds);
    Component * parent = getParentComponent();
    if (parent != nullptr)
    {
        Point<int> parentPos = parent->getScreenPosition();
        bounds = bounds - parentPos;
    }
    setBounds(bounds);
    batteryLabel->setBounds(textBounds - bounds.getPosition());
    batteryLabel->setFont(Font(textBounds.getHeight()));
    batteryLabel->setJustificationType(Justification::centredLeft);
    addAndMakeVisible(batteryLabel);
    Colour textColour;
    try
    {
        textColour = textSettings.getColours().at(0);
    } catch (std::out_of_range e)
    {
        textColour = Colours::white;
    }
    batteryLabel->setColour(Label::ColourIds::textColourId, textColour);
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
    resizeImage();
    ConfigFile * config = ConfigFile::getInstance();
    ConfigFile::ComponentSettings textSettings =
            config->getComponentSettings(BATTERY_PERCENT);
    Rectangle<int> textBounds = textSettings.getBounds() - getPosition();
    batteryLabel->setBounds(textBounds);
    //DBG(String("Text bounds set to ")+textBounds.toString());
    batteryLabel->setFont(fontResizedToFit(batteryLabel->getFont(),
            batteryLabel->getText(false), textBounds));
    if (!getBounds().contains(textBounds))
    {
        setBounds(getBounds().getUnion(textBounds));
    }

}

void BatteryIcon::setStatus(BatteryIconImage batteryImage, String percent)
{
    ((VectorImageButton*)this)->setImage((int) batteryImage);
    batteryLabel->setText(percent, dontSendNotification);
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
