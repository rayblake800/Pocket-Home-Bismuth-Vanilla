#include "Utils.h"
#include "ConfigItemData.h"

/* Menu item title key. */
static const juce::Identifier titleKey("name");

/* Menu item icon key. */
static const juce::Identifier iconKey("icon");

/* Application launch command key. */
static const juce::Identifier commandKey("command");

/* Terminal application key. */
static const juce::Identifier launchInTermKey("launch in terminal");

/* Application/directory category key. */
static const juce::Identifier categoryKey("categories");

/* Folder item list key. */
static const juce::Identifier folderItemKey("folder items");

ConfigItemData::ConfigItemData(juce::var& jsonData, const int index,
        const ConfigItemData* parent) :
Localized("ConfigItemData"),
MenuItemData(index, parent),
jsonData(jsonData) { }

/*
 * Creates a copy of this object.
 */
MenuItemData* ConfigItemData::clone() const
{
    juce::var dataCopy = jsonData.clone();
    const ConfigItemData* parent = 
        dynamic_cast<const ConfigItemData*>(getParent());
    return new ConfigItemData(dataCopy, getIndex(), parent);
}

/*
 * Gets the menu item's displayed title.
 */
juce::String ConfigItemData::getTitle() const
{
    return jsonData.getProperty(titleKey, "");
}

/*
 * Sets the menu item's displayed title.
 */
void ConfigItemData::setTitle(const juce::String& title)
{
}

/*
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String ConfigItemData::getIconName() const
{
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void ConfigItemData::setIconName(const juce::String& iconName)
{
}

/*
 * Gets the application categories connected to this menu item.
 */
juce::StringArray ConfigItemData::getCategories()
{
}

/*
 * Sets the application categories connected to this menu item.
 */
void setCategories(const juce::StringArray& categories)
{
}

/*
 * Gets the menu item's application launch command.
 */
juce::String ConfigItemData::getCommand() const
{
}

/*
 * Sets the menu item's application launch command.
 */
void ConfigItemData::setCommand(const juce::String& newCommand)
{
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool ConfigItemData::getLaunchedInTerm() const
{
}

/*
 * Sets if this menu item runs its command in a new terminal window. */
void ConfigItemData::setLaunchedInTerm(const bool termLaunch)
{
}

/*
 * Deletes this menu item data from its source JSON file.
 */
void ConfigItemData::deleteFromSource()
{
    // TODO: Implement after AppConfigFile redesign.
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void ConfigItemData::updateSource()
{
    // TODO: Implement after AppConfigFile redesign.
}


/*
 * Checks if this menu item can be moved within its menu folder.
 */
bool ConfigItemData::canMoveIndex(const int offset)
{
    // TODO: Implement after AppConfigFile redesign.
    return false;
}


/*
 * Attempts to move this menu item within its menu folder.
 *                moved by the given offset value.
 */
bool ConfigItemData::moveIndex(const int offset)
{
    // TODO: Implement after AppConfigFile redesign.
    return false;
}

/*
 * Gets an appropriate title to use for a deletion confirmation 
 *         window.
 */
juce::String ConfigItemData::getConfirmDeleteTitle() const
{
    return localeText(remove_APP) 
            + getTitle() + localeText(from_favorites);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation 
 *         window.
 */
juce::String ConfigItemData::getConfirmDeleteMessage() const
{
    return localeText(will_remove_link);
}



/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String ConfigItemData::getEditorTitle() const
{
    return localeText(edit_app);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool ConfigItemData::isEditable(const DataField dataField)
{
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int folderItemCount()
{
}


/*
 * Gets a single menu item in the folder this menu item would open.
 */
MenuItemData* getFolderItem(int index)
{
}


/*
 * Gets all menu items in the folder this menu item would open.
 */
juce::Array<MenuItemData*> getFolderItems()
{
}


