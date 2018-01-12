/*
  ==============================================================================

    desktopEntryButton.cpp
    Created: 11 Jan 2018 6:57:42pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "../../PocketHomeApplication.h"
#include "DesktopEntryButton.h"

DesktopEntryButton::DesktopEntryButton
(DesktopEntry desktopEntry, int index, int column,IconThread& iconThread)
: AppMenuButton
(desktopEntry.getValue(DesktopEntry::name), index, column),
desktopEntry(desktopEntry)
{
    iconThread.loadIcon(this,desktopEntry.getValue(DesktopEntry::icon));
}

/**
 * @return true if this button is for an application folder
 */
bool DesktopEntryButton::isFolder() const
{
    return desktopEntry.getType() == DesktopEntry::directory;
}

/**
 * @return the display name of the associated application
 */
String DesktopEntryButton::getAppName() const
{
    return desktopEntry.getValue(DesktopEntry::name);
}

/**
 * @return application shell command or directory path.
 */
String DesktopEntryButton::getCommand() const
{
    String command = desktopEntry.getValue(DesktopEntry::exec);
    if (desktopEntry.getValue(DesktopEntry::terminal))
    {
        MainConfigFile& config = PocketHomeApplication::getInstance()
                ->getConfig();
        command = config.getConfigString
                (MainConfigFile::termLaunchCommandKey) + command;
    }
    return command;
}

/**
 * @return all application categories linked to this button.
 */
Array<String> DesktopEntryButton::getCategories() const
{
    return desktopEntry.getValue(DesktopEntry::categories);
}