#pragma once
#include "JuceHeader.h"

/**
 * @file AppShortcut.h
 *
 * @brief Represents an application shortcut.
 */
class AppShortcut
{
public:
    /**
     * @param name               The application's display name.
     *
     * @param icon               The application's icon name or path.
     *
     * @param shell              The application's launch command.
     *
     * @param launchInTerminal   Whether or not the application should launch in
     *                           a terminal window.
     */
    AppShortcut(const juce::String& name, 
            const juce::String& icon, 
            const juce::String& shell,
            const bool launchInTerminal = false);

    virtual ~AppShortcut() { };

    /**
     * Load an AppShortcut from JSON file data.
     * 
     * @param jsonObj  A juce::var containing AppShortcut data.
     */
    AppShortcut(juce::var jsonObj);

    /**
     * Package AppShortcut data into a juce::DynamicObject.
     *
     * @return  The AppShortcut's data as a DynamicObject.
     */
    juce::DynamicObject* getDynamicObject();

    /**
     * Compare this AppShortcut with another.
     *
     * @param rhs  Another AppShortcut to compare with this one.
     *
     * @return  True iff the two AppShortcuts have equivalent data values.
     */
    bool operator==(const AppShortcut& rhs) const;
   
    /**
     * Gets the application's display name.
     *
     * @return  The display name.
     */
    const juce::String getDisplayName() const;

    /**
     * Gets the name or path of the application's icon.
     *
     * @return  The icon name or path.
     */
    const juce::String getIconName() const;

    /**
     * Gets the command used to launch this application.
     *
     * @return  The application launch command.
     */
    const juce::String getLaunchCommand() const;

    /**
     * Checks if this application is a terminal application.
     *
     * @return  True iff this application should launch in a terminal window.
     */
    const bool isTerminalApplication() const;

private:
    // Application display name
    const juce::String name;
    // Application icon name/path
    const juce::String icon;
    // Application launch command
    const juce::String shell;
    // Set if application launches in the terminal
    const bool launchInTerminal;
};
