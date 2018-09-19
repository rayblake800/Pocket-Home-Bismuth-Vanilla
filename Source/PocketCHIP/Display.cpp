#include "JuceHeader.h"
#include "Utils.h"
#include "Display.h"
#include "SystemCommands.h"

/*
 * Gets the current display brightness level.
 */
int Display::getBrightness()
{
    using namespace juce;
    SystemCommands systemCommands;
    String brightness = systemCommands.runTextCommand
        (SystemCommands::TextCommand::getBrightness);
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
    using namespace juce;
    String newBrightness = String(median<int>(1, brightness, 10));
    SystemCommands systemCommands;
    systemCommands.runActionCommand
        (SystemCommands::ActionCommand::setBrightness, newBrightness);
}


