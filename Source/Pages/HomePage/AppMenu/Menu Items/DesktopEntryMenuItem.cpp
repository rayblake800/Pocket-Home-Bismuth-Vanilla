#include "../../../../Utils.h"
#include "../../../../Configuration/MainConfigFile.h"
#include "DesktopEntryMenuItem.h"

DesktopEntryMenuItem::DesktopEntryMenuItem(const DesktopEntry& desktopEntry)
: desktopEntry(desktopEntry) { }

DesktopEntryMenuItem::~DesktopEntryMenuItem() { }

/**
 * @return true if this menu item is an application folder
 */
bool DesktopEntryMenuItem::isFolder() const
{
    return desktopEntry.getType() == DesktopEntry::directory;
}

/**
 * @return the display name of the associated application
 */
String DesktopEntryMenuItem::getAppName() const
{
    return desktopEntry.getValue(DesktopEntry::name);
}

/**
 * @return application shell command or directory path.
 */
String DesktopEntryMenuItem::getCommand() const
{
    String command = desktopEntry.getValue(DesktopEntry::exec);
    if (desktopEntry.getValue(DesktopEntry::terminal))
    {
        MainConfigFile config;
        command = config.getConfigValue<String>
                (MainConfigFile::termLaunchCommandKey) + String(" ") + command;
    }
    return command;
}

/**
 * @return true iff this menu item is an application that launches in
 * the terminal.
 */
bool DesktopEntryMenuItem::isTerminalApp() const
{
    return desktopEntry.getValue(DesktopEntry::terminal);
}

/**
 * @return all application categories linked to this menu item.
 */
Array<String> DesktopEntryMenuItem::getCategories() const
{
    return desktopEntry.getValue(DesktopEntry::categories);
}

/**
 * @return the name or path used to load the icon file. 
 */
String DesktopEntryMenuItem::getIconName() const
{
    return desktopEntry.getValue(DesktopEntry::icon);
}

/**
 * @return true, changes to this menu item change the current user's
 * .Desktop files
 */
bool DesktopEntryMenuItem::changesDesktopEntries() const
{
    return true;
}

/**
 * Get an appropriate title to use for a deletion confirmation window.
 */
String DesktopEntryMenuItem::getConfirmDeleteTitle() const
{
    return String("Remove link to ") + getAppName() + String("?");
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
String DesktopEntryMenuItem::getConfirmDeleteMessage() const
{
    return "This application will be hidden from pocket-home.";
}

/**
 * @return the title to display over an editor for this menu item. 
 */
String DesktopEntryMenuItem::getEditorTitle() const
{
    return "Edit Application";
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > DesktopEntryMenuItem::getEditorCallback()
{
    return [this](AppMenuPopupEditor * editor)
    {
        editEntry(editor->getNameField(), editor->getIconField(),
                editor->getCategories(), editor->getCommandField(),
                editor->launchInTerm());
    };
}

/**
 * Sets this menu items desktopEntry to not display in pocket-home for the
 * current user.
 */
bool DesktopEntryMenuItem::removeMenuItemSource()
{
    Array<String> notShowIn = desktopEntry.getValue(DesktopEntry::notShowIn);
    notShowIn.add("pocket-home");
    desktopEntry.setValue(DesktopEntry::notShowIn, notShowIn);
    desktopEntry.writeFile();
    return true;
}

/**
 * Update this button's desktopEntry. This writes to 
 * ~/.local/share/applications, so changes will only affect the current user.
 * @param name application display name
 * @param icon application icon
 * @param categories application categories
 * @param command application launch command
 * @param useTerminal sets if this launches in a terminal window
 */
void DesktopEntryMenuItem::editEntry(String name, String icon,
        Array<String> categories,
        String command, bool useTerminal)
{
    desktopEntry.setValue(DesktopEntry::name, name);
    desktopEntry.setValue(DesktopEntry::icon, icon);
    desktopEntry.setValue(DesktopEntry::categories, categories);
    desktopEntry.setValue(DesktopEntry::exec, command);
    desktopEntry.setValue(DesktopEntry::terminal, useTerminal);
    desktopEntry.writeFile();
}
