#include "Utils.h"
#include "MainConfigFile.h"
#include "DesktopEntryFileError.h"
#include "DesktopEntryMenuItem.h"

DesktopEntryMenuItem::DesktopEntryMenuItem(const DesktopEntry& desktopEntry)
: Localized("DesktopEntryMenuItem"),
desktopEntry(desktopEntry) { }

/**
 * @return true if this menu item is an application folder
 */
bool DesktopEntryMenuItem::isFolder() const
{
    return desktopEntry.getType() == DesktopEntry::Type::directory;
}

/**
 * @return the display name of the associated application
 */
juce::String DesktopEntryMenuItem::getAppName() const
{
    return desktopEntry.getName();
}

/**
 * @return application shell command or directory path.
 */
juce::String DesktopEntryMenuItem::getCommand() const
{
    return desktopEntry.getLaunchCommand();
}

/**
 * @return true iff this menu item is an application that launches in
 * the terminal.
 */
bool DesktopEntryMenuItem::isTerminalApp() const
{
    return desktopEntry.getLaunchedInTerm();
}

/**
 * @return all application categories linked to this menu item.
 */
juce::StringArray DesktopEntryMenuItem::getCategories() const
{
    return desktopEntry.getCategories();
}

/**
 * @return the name or path used to load the icon file. 
 */
juce::String DesktopEntryMenuItem::getIconName() const
{
    return desktopEntry.getIcon();
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
juce::String DesktopEntryMenuItem::getConfirmDeleteTitle() const
{
    return localeText(remove_link_to) + getAppName()
            + localeText(question_mark);
}

/**
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String DesktopEntryMenuItem::getConfirmDeleteMessage() const
{
    return localeText(will_hide);
}

/**
 * @return the title to display over an editor for this menu item. 
 */
juce::String DesktopEntryMenuItem::getEditorTitle() const
{
    return localeText(edit_app);
}

/**
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) >
DesktopEntryMenuItem::getEditorCallback()
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
    using namespace juce;
    desktopEntry.setIfDisplayed(false);
    try
    {
        desktopEntry.writeFile();
    }
    catch(DesktopEntryFileError e)
    {
        return false;
    }
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
void DesktopEntryMenuItem::editEntry(juce::String name, juce::String icon,
        juce::StringArray categories,
        juce::String command, bool useTerminal)
{
    desktopEntry.setName(name);
    desktopEntry.setIcon(icon);
    desktopEntry.setCategories(categories);
    if (useTerminal)
    {
        command = command.fromLastOccurrenceOf
                (getTermLaunchPrefix(), false, false);
    }
    desktopEntry.setExec(command);
    desktopEntry.setLaunchedInTerm(useTerminal);
    desktopEntry.writeFile();
}
