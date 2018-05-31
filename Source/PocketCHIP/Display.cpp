#include "JuceHeader.h"
#include "Utils.h"
#include "Display.h"

/*
 * Gets the current display brightness level.
 */
int Display::getBrightness()
{
    File brightnessFile("/sys/class/backlight/backlight/brightness");
    if (brightnessFile.existsAsFile())
    {
        return brightnessFile.loadFileAsString().getIntValue();
    }
    return 0;
}

/*
 * Sets the display brightness level.
 */
void Display::setBrightness(const int& brightness)
{
    File brightnessFile("/sys/class/backlight/backlight/brightness");
    if (brightnessFile.existsAsFile())
    {
        brightnessFile.appendText(String(median<int>(1, brightness, 10)));
    }
}

/**
 * Gets the size of the active display.
 * 
 * @return the size of the display, measured in pixels.
 */
Rectangle<int> Display::getDisplaySize(){
    return Desktop::getInstance().getDisplays().getMainDisplay().userArea;
}

/**
 * Gets the size of the single application window.
 */
Rectangle<int> Display::getWindowSize()
{
    Component * windowComp = Desktop::getInstance().getComponent(0);
    if (windowComp == nullptr)
    {
        return Rectangle<int>();
    }
    return windowComp->getLocalBounds();
}

