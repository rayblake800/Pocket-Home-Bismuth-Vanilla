#include "JuceHeader.h"
#include "Utils.h"
#include "Display.h"

/**
 * @return display brightness, as an integer between 0 and 10
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

/**
 * Set display brightness
 * @param brightness should be an integer between 0 and 10. Values outside
 * of this range will be rounded to the closest valid integer.
 */
void Display::setBrightness(int brightness)
{
    brightness = median<int>(1, brightness, 10);

    File brightnessFile("/sys/class/backlight/backlight/brightness");
    if (brightnessFile.existsAsFile())
    {
        brightnessFile.appendText(String(brightness));
    }
}
