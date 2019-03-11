#include "JuceHeader.h"
#include "Display.h"
#include "Util_Commands.h"
#include "Util_Math.h"

/*
 * Gets the current display brightness level.
 */
int Display::getBrightness()
{
    Util::Commands systemCommands;
    juce::String brightness = systemCommands.runTextCommand(
            Util::CommandTypes::Text::getBrightness);
    if(brightness.isNotEmpty())
    {
        return brightness.getIntValue();
    }
    return 0;
}

/*
 * Sets the display brightness level.
 */
void Display::setBrightness(const int& brightness)
{
    juce::String newBrightness(Util::Math::median<int>(1, brightness, 10));
    Util::Commands systemCommands;
    systemCommands.runActionCommand(Util::CommandTypes::Action::setBrightness,
            newBrightness);
}

/*
 * Gets the size of the main display.
 */
juce::Rectangle<int> Display::getSize()
{
    return juce::Desktop::getInstance().getDisplays().getMainDisplay().userArea;
}

/*
 * Gets the width of the main display.
 */
int Display::getWidth()
{
    return getSize().getWidth();
}

/*
 * Gets the height of the main display.
 */
int Display::getHeight()
{
    return getSize().getHeight();
}
