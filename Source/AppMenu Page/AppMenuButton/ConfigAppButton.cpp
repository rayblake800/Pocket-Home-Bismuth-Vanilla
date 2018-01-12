/*
  ==============================================================================

    ConfigAppButton.cpp
    Created: 11 Jan 2018 6:58:24pm
    Author:  anthony

  ==============================================================================
 */
#include "../../Utils.h"
#include "ConfigAppButton.h"

ConfigAppButton::ConfigAppButton(AppConfigFile::AppItem appItem,
        int index, int column, IconThread& iconThread) :
AppMenuButton(appItem.name, index, column),
appItem(appItem)
{
    iconThread.loadIcon(this,appItem.icon);
}

/**
 * Check if this button is for an application folder
 * @return false
 */
bool ConfigAppButton::isFolder() const
{
    return false;
}

/**
 * @return the display name of the associated app
 */
String ConfigAppButton::getAppName() const
{
    return appItem.name;
}

/**
 * @return the application's launch command
 */
String ConfigAppButton::getCommand() const
{
    return appItem.shell;
}

/**
 * @return all application categories linked to this button.
 */
Array<String> ConfigAppButton::getCategories() const
{
    Array<String> categories;
    if (appItem.folder.isNotEmpty())
    {
        categories.add(appItem.folder);
    }
    return categories;
}