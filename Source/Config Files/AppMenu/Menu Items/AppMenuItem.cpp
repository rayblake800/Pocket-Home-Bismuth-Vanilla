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

/**
 * Gets the menu item's displayed title.
 */
juce::String AppMenuItem::getTitle() const
{
    return dataSource->getTitle();
}

/*
 * Gets the menu item's icon name.
 */
juce::String AppMenuItem::getIconName() const
{
    return dataSource->getIconName();
}

/*
 * Gets all items within a folder menu item.
 */
juce::Array<AppMenuItem> AppMenuItem::getFolderItems() const
{
    using namespace juce;
    OwnedArray<MenuItemData> folderData;
    folderData.addArray(dataSource->getFolderItems());
    Array<AppMenuItem> folderItems;
    for(MenuItemData* dataItem : folderData)
    {
        folderItems.add(AppMenuItem(dataItem));
    }
    return folderItems;
}

/*
 * Gets any launch command associated with this menu item.
 */
juce::String AppMenuItem::getCommand() const
{
    return dataSource->getCommand();
}

/*
 * Gets all application categories associated with this menu item.
 */
juce::StringArray AppMenuItem::getCategories() const
{
    return dataSource->getCategories();
}


/*
 * Checks if this menu item is a terminal application.
 */
bool AppMenuItem::getLaunchedInTerm() const
{
    return dataSource->getLaunchedInTerm();
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
    return dataSource->isEditable(ConfigItemData::DataField::categories);
}

/*
 * Checks if the menu item has an editable command field.
 */
bool AppMenuItem::hasEditableCommand() const
{
    return dataSource->isEditable(ConfigItemData::DataField::command);
}

/*
 * Gets a title to use when editing this menu item.
 */
juce::String AppMenuItem::getEditorTitle() const
{
    return localeText(edit_menu_item);
}

/*
 * Gets a PopupEditorComponent callback function that will apply 
 * changes from an AppMenuPopupEditor to this menu item.
 */
std::function<void(AppMenuPopupEditor*) > AppMenuItem::getEditorCallback()
{
   return [this](AppMenuPopupEditor* editor)
   {
        dataSource->setTitle(editor->getNameField());
        dataSource->setIconName(editor->getIconField());
        if(hasEditableCommand())
        {
            dataSource->setCommand(editor->getCommandField());
            dataSource->setLaunchedInTerm(editor->launchInTerm());
        }
        if(hasEditableCategories())
        {
            dataSource->setCategories(editor->getCategories());
        }
        dataSource->updateSource();
   };
}

/*
 * Removes the source of this menu item's data.
 */
void AppMenuItem::removeMenuItemSource()
{
    dataSource->deleteFromSource();
}

/*
 * Gets the menu item's index within its folder.
 */
int AppMenuItem::getIndex() const
{
    return dataSource->getIndex();
}

    
/*
 * Gets the indices of this menu item and all its parents within the application
 * menu.
 */
juce::Array<int> AppMenuItem::getFullIndex() const
{
    return dataSource->getFullIndex();
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


