#include "AppJSON.h"

// SharedResource object key
const juce::Identifier AppJSON::resourceKey = "AppJSON";
// JSON configuration file name
static const constexpr char * jsonFilename = "apps.json";
// JSON key to the application shortcut list.
static const constexpr char * favoritesKey = "favorites";
// JSON key to the application folder list.
static const constexpr char * foldersKey = "folders";

AppJSON::AppJSON() : ConfigJSON(resourceKey, jsonFilename)
{
    using namespace juce;
    //load favorites
    DBG("Loading AppConfigFile:");
    Array<var> favoriteList = initProperty<Array<var>>(favoritesKey);
    DBG("" << __func__ << ": Read " << favoriteList.size()
            << " favorites");
    for(const var& app : favoriteList)
    {
        AppItem fave = AppItem(app);
        if (!favoriteApps.contains(fave))
        {
            favoriteApps.add(fave);
        }
    }
    //load categories
    Array<var> categoryList = initProperty<Array<var>>(foldersKey);
    DBG("" << __func__ << ": Read " << categoryList.size()
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
 * Get the main list of application shortcuts.
 */
juce::Array<AppItem> AppJSON::getFavorites()
{
    return favoriteApps;
}

/**
 * Add a new app to the list of pinned favorite apps in the config file.
 */
void AppJSON::addFavoriteApp
(AppItem newApp, int index, bool writeChangesNow)
{
    favoriteApps.insert(index, newApp);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Remove an app from the list of favorite applications.
 */
void AppJSON::removeFavoriteApp(int index, bool writeChangesNow)
{
    if (index >= 0 && index < favoriteApps.size())
    {
        favoriteApps.remove(index);
        if (writeChangesNow)
        {
            writeChanges();
        }
    }
}

/**
 * Find the index of an AppItem in favorites.
 */
int AppJSON::getFavoriteIndex(AppItem toFind)
{
    return favoriteApps.indexOf(toFind);
}

/**
 * @return A list of folders to display in the AppMenu.
 */
juce::Array<AppFolder> AppJSON::getFolders()
{
    return categoryFolders;
}

/**
 * Add a new folder to the list of AppFolders in the config file.
 */
void AppJSON::addAppFolder
(AppFolder newFolder, int index, bool writeChangesNow)
{
    categoryFolders.insert(index, newFolder);
    if (writeChangesNow)
    {
        writeChanges();
    }
}

/**
 * Remove a folder from the list of AppFolders.
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
 * Find the index of an AppFolder in the list of folders.
 */
int AppJSON::getFolderIndex(AppFolder toFind)
{
    return categoryFolders.indexOf(toFind);
}



/**
 * Copy all config data to a json object.
 */
void AppJSON::writeDataToJSON()
{
    using namespace juce;
    //set favorites
    Array<var> favoriteArray;
    for (int i = 0; i < favoriteApps.size(); i++)
    {
        favoriteArray.add(var(favoriteApps[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(favoritesKey, favoriteArray);

    //set folders
    Array<var> categoryArray;
    for (int i = 0; i < categoryFolders.size(); i++)
    {
        categoryArray.add(var(categoryFolders[i].getDynamicObject()));
    }
    updateProperty<Array<var>>(foldersKey, categoryArray);
}
