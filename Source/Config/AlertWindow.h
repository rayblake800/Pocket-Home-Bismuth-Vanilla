#pragma once
#include "Localized.h"
#include "Config.h"
#include "JuceHeader.h"

/**
 * @file AlertWindow.h
 * 
 * @brief Displays pop-up error messages when something goes wrong with reading
 *        or writing JSON configuration files.
 * 
 * TODO: Localized error messages for:
 *      -Bad permissions on config files
 *      -Inability to create new files
 *      -Inappropriate data types in config files
 *      -Missing default config files
 */
class Config::AlertWindow : private Localized
{
public:
    AlertWindow() : Localized("ConfigAlertWindows") { }
    
    virtual ~AlertWindow() { }
    
    /**
     * Shows a simple alert window with error text.  This should be replaced 
     * when ConfigErrorMessage is properly implemented.
     */
    void showPlaceholderError(juce::String errorMessage)
    {
        juce::AlertWindow::showMessageBoxAsync(
                juce::AlertWindow::AlertIconType::WarningIcon,
                "ConfigFile Error",
                errorMessage);
    }
};
