#pragma once
/**
 * @file  Windows_MainWindow.h
 *
 * @brief  A juce::Window class used as a basis for primary application window
 *         classes.
 */

#include "JuceHeader.h"

namespace Windows { class MainWindow; }

/**
 * @brief  A basis for a main application window class that ensures all
 *         FocusListener objects are updated when the window gains or loses
 *         focus.
 */
class Windows::MainWindow : public juce::DocumentWindow 
{
public:
    /**
     * @brief  Ensures the focus tracker is initialized and sets initial window
     *         properties.
     *
     * @param title              Sets the window's title as printed on the title
     *                           bar.
     * 
     * @param backgroundColour   The background color to draw behind window
     *                           components.
     * 
     * @param requiredButtons    Sets which title bar buttons will be shown.
     */
    MainWindow(const juce::String& title, juce::Colour backgroundColour, 
            int requiredButtons);

    /**
     * @brief  Closes the window's connection to the FocusTracker resource.
     */
    virtual ~MainWindow();
    
private:
    /**
     * @brief  Ensures all listeners are notified when the window focus state 
     *         changes.
     */
    void activeWindowStatusChanged() override;
};
