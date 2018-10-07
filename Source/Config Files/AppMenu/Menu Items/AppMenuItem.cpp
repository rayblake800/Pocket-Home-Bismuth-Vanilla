#include "MainConfigKeys.h"
#include "MainConfigFile.h"
#include "Utils.h"
#include "AppMenuItem.h"

/* Localized text keys: */
static const constexpr char * localizedClassKey = "AppMenuItem";
static const constexpr char * delete_APP = "delete_APP";
static const constexpr char * question_mark = "question_mark";
static const constexpr char * really_delete = "really_delete";
static const constexpr char * edit_menu_item = "edit_menu_item";

/*
 * Creates a menu item from some source of menu data.
 */
AppMenuItem::AppMenuItem(MenuItemData* dataSource) : 
Localized(localizedClassKey),
dataSource(dataSource->clone()) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenuItem::AppMenuItem(const AppMenuItem& toCopy) :
Localized(localizedClassKey),
dataSource(toCopy.dataSource->clone()) { }


/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenuItem::isFolder() const
{
    return dataSource->isFolder();
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int AppMenuItem::getFolderSize()
{
    return dataSource->getFolderSize();
}

/**
 * Gets the menu item's displayed title.
 */
juce::String AppMenuItem::getTitle() const
{
    return dataSource->getTitle();
}

/*
 * Sets the menu item's displayed title.
 */
void AppMenuItem::setTitle(const juce::String& title)
{
    if(dataSource->isEditable(MenuItemData::DataField::title))
    {
        dataSource->setTitle(title);
    }
}

/*
 * Gets the menu item's icon name.
 */
juce::String AppMenuItem::getIconName() const
{
    return dataSource->getIconName();
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenuItem::setIconName(const juce::String& iconName)
{
    if(dataSource->isEditable(MenuItemData::DataField::icon))
    {
        dataSource->setIconName(iconName);
    }
}

/*
 * Gets any launch command associated with this menu item.
 */
juce::String AppMenuItem::getCommand() const
{
    return dataSource->getCommand();
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenuItem::setCommand(const juce::String& newCommand)
{
    if(dataSource->isEditable(MenuItemData::DataField::command))
    {
        dataSource->setCommand(newCommand);
    }
}

/*
 * Gets all application categories associated with this menu item.
 */
juce::StringArray AppMenuItem::getCategories() const
{
    return dataSource->getCategories();
}

/*
 * Sets the application categories connected to this menu item.
 */
void AppMenuItem::setCategories(const juce::StringArray& categories)
{
    if(dataSource->isEditable(MenuItemData::DataField::categories))
    {
        dataSource->setCategories(categories);
    }
}

/*
 * Checks if this menu item is a terminal application.
 */
bool AppMenuItem::getLaunchedInTerm() const
{
    return dataSource->getLaunchedInTerm();
}

/*
 * Sets if this menu item runs its command in a new terminal window.
 */
void AppMenuItem::setLaunchedInTerm(const bool termLaunch)
{
    if(dataSource->isEditable(MenuItemData::DataField::termLaunchOption))
    {
        dataSource->setLaunchedInTerm(termLaunch);
    }
}

/*
 * Compares this menu item with another.
 */
bool AppMenuItem::operator==(const AppMenuItem& toCompare) const
{
    return getTitle() == toCompare.getTitle()
            && getIconName() == toCompare.getIconName()
            && getCategories() == toCompare.getCategories()
            && getCommand() == toCompare.getCommand()
            && getLaunchedInTerm() == toCompare.getLaunchedInTerm();
}

/*
 * Displays an alert to the user asking if this item should be removed from the
 * menu, and deletes the menu item if the user confirms.
 */
void AppMenuItem::deleteOnConfim(const std::function<void()> onConfirm)
{
    confirmAction(
            localeText(delete_APP) + getTitle() + localeText(question_mark),
            localeText(really_delete),
            [this, onConfirm]()
            {
                dataSource->deleteFromSource();
                onConfirm();
            });
}

/*
 * Checks if this menu item has categories that may be edited.
 */
bool AppMenuItem::hasEditableCategories() const
{
    return dataSource->isEditable(MenuItemData::DataField::categories);
}

/*
 * Checks if the menu item has an editable command field.
 */
bool AppMenuItem::hasEditableCommand() const
{
    return dataSource->isEditable(MenuItemData::DataField::command);
}

/*
 * Gets a title to use when editing this menu item.
 */
juce::String AppMenuItem::getEditorTitle() const
{
    return localeText(edit_menu_item);
}

/*
 * Removes the source of this menu item's data.
 */
void AppMenuItem::deleteFromSource()
{
    dataSource->deleteFromSource();
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenuItem::updateSource()
{
    dataSource->updateSource();
}

/*
 * Gets the menu item's index within its folder.
 */
int AppMenuItem::getIndex() const
{
    return dataSource->getIndex();
}

    
/*
 * Gets the index of the menu folder holding this menu item.
 */
const juce::Array<int>& AppMenuItem::getFolderIndex() const
{
    return dataSource->getFolderIndex();
}

/*
 * Checks if this menu item has an index that can be moved by a given amount.
 */
bool AppMenuItem::canMoveIndex(const int offset) const
{
    return dataSource->canMoveIndex(offset);
}

/*
 * Attempts to change the index of this menu item by some offset amount.
 */
bool AppMenuItem::moveDataIndex(int offset)
{
    return dataSource->moveIndex(offset);
}


