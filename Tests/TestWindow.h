#pragma once
#include "WindowFocus.h"


/**
 * @file  TestWindow.h
 *
 * @brief  Creates a basic window for testing a single component.
 */
class TestWindow : public WindowFocus::BroadcastWindow
{
public:
    /**
     * @brief  Creates and shows a window holding a test component.
     *
     * @param testName       The name of the tested component, to use as the
     *                       window title.
     *
     * @param testComponent  The component that the window will display.
     *
     * @param xPos           The initial x-coordinate position of the window.
     *
     * @param yPos           The initial y-coordinate position of the window.
     *
     * @param width          The initial width of the window.
     *
     * @param height         The initial height of the window.
     */
    TestWindow(const juce::String testName,
            juce::Component* const testComponent,
            const int xPos,
            const int yPos,
            const int width,
            const int height);

    virtual ~TestWindow() { }
};
