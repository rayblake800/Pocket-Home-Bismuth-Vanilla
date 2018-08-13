#pragma once
#include "Localized.h"
#include "JuceHeader.h"

/**
 * @file ConfigAlertWindows.h
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
class ConfigAlertWindows : private Localized
{
public:
    ConfigAlertWindows() : Localized("ConfigAlertWindows") { }
    
    virtual ~ConfigAlertWindows() { }
    
    /**
     * Shows a simple alert window with error text.  This should be replaced 
     * when ConfigErrorMessage is properly implemented.
     */
    void showPlaceholderError(juce::String errorMessage)
    {
        using namespace juce;
        AlertWindow::showMessageBoxAsync(
                AlertWindow::AlertIconType::WarningIcon,
                "ConfigFile Error",
                errorMessage);
    }
};