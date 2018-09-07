#include "AppShortcut.h"

AppShortcut::AppShortcut(const juce::String& name, 
const juce::String& icon, 
const juce::String& shell,
const bool launchInTerminal) :
    name(name),
    icon(icon),
    shell(shell),
    launchInTerminal(launchInTerminal) { }

/*
 * Load an AppShortcut from JSON file data.
 */
AppShortcut::AppShortcut(juce::var jsonObj) :
name(jsonObj.getProperty("name", "").operator juce::String()),
icon(jsonObj.getProperty("icon", "").operator juce::String()),
shell(jsonObj.getProperty("shell", "").operator juce::String()),
launchInTerminal(jsonObj.getProperty
        ("launch in terminal", false).operator bool()) { }

/*
 * Package AppShortcut data into a juce::DynamicObject.
 */
juce::DynamicObject* AppShortcut::getDynamicObject()
{
    using namespace juce;
    DynamicObject * appObject = new DynamicObject();
    appObject->setProperty("name", name);
    appObject->setProperty("icon", icon);
    appObject->setProperty("shell", shell);
    appObject->setProperty("launch in terminal", launchInTerminal);
    return appObject;
}

/*
 * Compare this AppShortcut with another.
 */
bool AppShortcut::operator==(const AppShortcut& rhs) const
{
    return icon == rhs.icon 
        && name == rhs.name 
        && shell == rhs.shell;
}

/*
 * Gets the application's display name.
 */
const juce::String AppShortcut::getDisplayName() const
{
    return name;
}

/*
 * Gets the name or path of the application's icon.
 */
const juce::String AppShortcut::getIconName() const
{
    return icon;
}

/*
 * Gets the command used to launch this application.
 */
const juce::String AppShortcut::getLaunchCommand() const
{
    return shell;
}

/*
 * Checks if this application is a terminal application.
 */
const bool AppShortcut::isTerminalApplication() const
{
    return launchInTerminal;
}
