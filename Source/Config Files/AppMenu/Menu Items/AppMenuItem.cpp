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
AppMenuItem::AppMenuItem(MenuItemData::Ptr dataSource) : 
Localized(localizedClassKey),
dataSource(dataSource) { }

/*
 * Creates a menu item copying data from another menu item. 
 */
AppMenuItem::AppMenuItem(const AppMenuItem& toCopy) :
Localized(localizedClassKey),
dataSource(toCopy.dataSource) { }


/*
 * Checks if this menu item represents a folder within the menu.
 */
bool AppMenuItem::isFolder() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->isFolder();
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int AppMenuItem::getFolderSize()
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getFolderSize();
}

/**
 * Gets the menu item's displayed title.
 */
juce::String AppMenuItem::getTitle() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getTitle();
}

/*
 * Sets the menu item's displayed title.
 */
void AppMenuItem::setTitle(const juce::String& title)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
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
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getIconName();
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenuItem::setIconName(const juce::String& iconName)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
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
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getCommand();
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenuItem::setCommand(const juce::String& newCommand)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
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
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getCategories();
}

/*
 * Sets the application categories connected to this menu item.
 */
void AppMenuItem::setCategories(const juce::StringArray& categories)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
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
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getLaunchedInTerm();
}

/*
 * Sets if this menu item runs its command in a new terminal window.
 */
void AppMenuItem::setLaunchedInTerm(const bool termLaunch)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
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
    const juce::ScopedReadLock readLock(dataSource->getLock());
    const juce::ScopedReadLock otherReadLock(toCompare.dataSource->getLock());
    if(dataSource == toCompare.dataSource)
    {
        return true;
    }
    return getTitle() == toCompare.getTitle()
            && getIconName() == toCompare.getIconName()
            && getCategories() == toCompare.getCategories()
            && getCommand() == toCompare.getCommand()
            && getLaunchedInTerm() == toCompare.getLaunchedInTerm();
}

/*
 * Compares this menu item with another.
 */
bool AppMenuItem::operator<(const AppMenuItem& toCompare) const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    const juce::ScopedReadLock otherReadLock(toCompare.dataSource->getLock());
    return dataSource < toCompare.dataSource;
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
                const juce::ScopedWriteLock writeLock(dataSource->getLock());
                dataSource->deleteFromSource();
                onConfirm();
            });
}

/*
 * Checks if this menu item has categories that may be edited.
 */
bool AppMenuItem::hasEditableCategories() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->isEditable(MenuItemData::DataField::categories);
}

/*
 * Checks if the menu item has an editable command field.
 */
bool AppMenuItem::hasEditableCommand() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->isEditable(MenuItemData::DataField::command);
}

/*
 * Gets a title to use when editing this menu item.
 */
juce::String AppMenuItem::getEditorTitle() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return localeText(edit_menu_item);
}

/*
 * Removes the source of this menu item's data.
 */
void AppMenuItem::deleteFromSource()
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
    dataSource->deleteFromSource();
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenuItem::updateSource()
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
    dataSource->updateSource();
}

/*
 * Gets the menu item's index within its folder.
 */
const MenuIndex& AppMenuItem::getIndex() const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->getIndex();
}
    
/*
 * Checks if this menu item has an index that can be moved by a given amount.
 */
bool AppMenuItem::canMoveIndex(const int offset) const
{
    const juce::ScopedReadLock readLock(dataSource->getLock());
    return dataSource->canMoveIndex(offset);
}

/*
 * Attempts to change the index of this menu item by some offset amount.
 */
bool AppMenuItem::moveDataIndex(const int offset)
{
    const juce::ScopedWriteLock writeLock(dataSource->getLock());
    return dataSource->moveIndex(offset);
}


