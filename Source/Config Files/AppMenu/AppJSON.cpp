#include "AppJSON.h"

/* SharedResource object key */
const juce::Identifier AppJSON::resourceKey = "AppJSON";
/* JSON configuration file name */
static const constexpr char * jsonFilename = "apps.json";
/* JSON key to the application shortcut list. */
static const juce::Identifier shortcutKey = "shortcuts";
/* JSON key to the application folder list. */
static const juce::Identifier  foldersKey = "folders";

AppJSON::AppJSON() : ConfigJSON(resourceKey, jsonFilename)
{
    using namespace juce;
    // Load shortcuts:
    Array<var> shortcutList = initProperty<Array<var>>(shortcutKey);
    DBG("AppJSON::" << __func__ << ": Read " << shortcuts.size()
            << " shortcuts.");
    for(const var& app : shortcutList)
    {
        AppShortcut shortcut = AppShortcut(app);
        if (!shortcuts.contains(shortcut))
        {
            shortcuts.add(shortcut);
        }
    }
    // Load folders:
    Array<var> folderList = initProperty<Array<var>>(foldersKey);
    DBG("AppJSON::" << __func__ << ": Read " << folderList.size()
            << " categories");
    for (const var& folder : folderList)
    {
        AppFolder menuFolder = AppFolder(folder);
        if (!folders.contains(menuFolder))
        {
            folders.add(menuFolder);
        }
    }
    loadJSONData();   
}

/*
 * Gets the main list of application shortcuts.
 */
juce::Array<AppShortcut> AppJSON::getShortcuts() const
{
    return shortcuts;
}

/*
 * Adds a new shortcut to the list of pinned application shortcuts.
 */
void AppJSON::addShortcut
(const AppShortcut& newApp, const int index, const bool writeChangesNow)
{
    shortcuts.insert(index, newApp);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/*
 * Removes a shortcut from the list of application shortcuts.
 */
void AppJSON::removeShortcut(const int index, const bool writeChangesNow)
{
    if (index >= 0 && index < shortcuts.size())
    {
        shortcuts.remove(index);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/*
 * Finds the index of an application shortcut in the list.
 */
int AppJSON::getShortcutIndex(const AppShortcut& toFind) const
{
    return shortcuts.indexOf(toFind);
}

/*
 * Gets the list of application folders.
 */
juce::Array<AppFolder> AppJSON::getFolders() const
{
    return folders;
}

/*
 * Adds a new folder to the list of application folders.
 */
void AppJSON::addAppFolder
(const AppFolder& newFolder, const int index, const bool writeChangesNow)
{
    folders.insert(index, newFolder);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/*
 * Removes a folder from the list of application folders. 
 */
void AppJSON::removeAppFolder(const int index, const bool writeChangesNow)
{
    int size =folders.size();
    folders.remove(index);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/*
 * Finds the index of an AppFolder in the list of folders.
 */
int AppJSON::getFolderIndex(const AppFolder& toFind) const
{
    return folders.indexOf(toFind);
}

/*
 * Copies all shortcuts and folders back to the JSON configuration file.
 */
void AppJSON::writeDataToJSON()
{
    using namespace juce;
    // Set shortcuts:
    Array<var> shortcutArray;
    for (int i = 0; i < shortcuts.size(); i++)
    {
        shortcutArray.add(var(shortcuts[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(shortcutKey, shortcutArray);

    // Set folders:
    Array<var> categoryArray;
    for (int i = 0; i < folders.size(); i++)
    {
        categoryArray.add(var(folders[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(foldersKey, categoryArray);
}
