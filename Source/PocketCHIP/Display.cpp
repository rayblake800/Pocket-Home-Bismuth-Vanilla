#include "JuceHeader.h"
#include "Utils.h"
#include "Display.h"

/*
 * Gets the current display brightness level.
 */
int Display::getBrightness()
{
    using namespace juce;
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
    using namespace juce;
    File brightnessFile("/sys/class/backlight/backlight/brightness");
    if (brightnessFile.existsAsFile())
    {
        brightnessFile.appendText(String(median<int>(1, brightness, 10)));
    }
}


