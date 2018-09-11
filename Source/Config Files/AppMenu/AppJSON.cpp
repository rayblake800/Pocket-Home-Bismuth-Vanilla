#include "AppJSON.h"
#include "ConfigListener.h"

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
    //load shortcuts
    Array<var> shortcutList = initProperty<Array<var>>(shortcutKey);
    DBG("AppJSON::" << __func__ << ": Read " << shortcuts.size()
            << " favorites");
    for(const var& app : shortcutList)
    {
        AppShortcut shortcut = AppShortcut(app);
        if (!shortcuts.contains(shortcut))
        {
            shortcuts.add(shortcut);
        }
    }
    //load categories
    Array<var> categoryList = initProperty<Array<var>>(foldersKey);
    DBG("AppJSON::" << __func__ << ": Read " << categoryList.size()
            << " categories");
    for (const var& folder : categoryList)
    {
        AppFolder menuFolder = AppFolder(folder);
        if (!categoryFolders.contains(menuFolder))
        {
            categoryFolders.add(menuFolder);
        }
    }
    loadJSONData();   
}

/**
 * Gets the main list of application shortcuts.
 */
juce::Array<AppShortcut> AppJSON::getShortcuts()
{
    return shortcuts;
}

/**
 * Adds a new shortcut to the list of pinned application shortcuts.
 */
void AppJSON::addShortcut
(AppShortcut newApp, int index, bool writeChangesNow)
{
    shortcuts.insert(index, newApp);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Removes a shortcut from the list of application shortcuts.
 */
void AppJSON::removeShortcut(int index, bool writeChangesNow)
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

/**
 * Finds the index of an application shortcut in the list.
 */
int AppJSON::getShortcutIndex(const AppShortcut& toFind)
{
    return shortcuts.indexOf(toFind);
}

/**
 * Gets the list of application folders.
 */
juce::Array<AppFolder> AppJSON::getFolders()
{
    return categoryFolders;
}

/**
 * Adds a new folder to the list of application folders.
 */
void AppJSON::addAppFolder
(const AppFolder& newFolder, int index, bool writeChangesNow)
{
    categoryFolders.insert(index, newFolder);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Removes a folder from the list of application folders. 
 */
void AppJSON::removeAppFolder(int index, bool writeChangesNow)
{
    int size = categoryFolders.size();
    categoryFolders.remove(index);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Finds the index of an AppFolder in the list of folders.
 */
int AppJSON::getFolderIndex(const AppFolder& toFind)
{
    return categoryFolders.indexOf(toFind);
}

/**
 * Copies all shortcuts and folders back to the JSON configuration file.
 */
void AppJSON::writeDataToJSON()
{
    using namespace juce;
    //set shortcuts
    Array<var> shortcutArray;
    for (int i = 0; i < shortcuts.size(); i++)
    {
        shortcutArray.add(var(shortcuts[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(shortcutKey, shortcutArray);

    //set folders
    Array<var> categoryArray;
    for (int i = 0; i < categoryFolders.size(); i++)
    {
        categoryArray.add(var(categoryFolders[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(foldersKey, categoryArray);
}
