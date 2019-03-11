#include "Window_Info.h"
#include "JuceHeader.h"

/*
 * Gets the size of the main application window.
 */
juce::Rectangle<int> Window::Info::getBounds()
{
    juce::Component * windowComp = juce::Desktop::getInstance().getComponent(0);
    if (windowComp == nullptr)
    {
        return juce::Rectangle<int>();
    }
    return windowComp->getBounds();
}

/*
 * Gets the width of the main application window.
 */
int Window::Info::getWidth()
{
    return getBounds().getWidth();
}

/*
 * Gets the height of the main application window.
 */
int Window::Info::getHeight()
{
    return getBounds().getHeight();
}
