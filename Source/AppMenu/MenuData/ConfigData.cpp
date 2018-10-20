#define APPMENU_IMPLEMENTATION_ONLY
#include "ConfigData.h"
#include "DesktopEntryData.h"
#include "ConfigKeys.h"

/* Localized text keys: */
static const constexpr char * remove_APP = "remove_APP";
static const constexpr char * from_favorites = "from_favorites";
static const constexpr char * will_remove_link = "will_remove_link";
static const constexpr char * edit_app = "edit_app";

/*
 * Recursively creates a menu item and all its child folder items.
 */
AppMenu::ConfigData::ConfigData() :
Localized("ConfigItemData")
{
    pendingCallbackID = 0;
}

/*
 * Recursively initializes menu item data, creating and initializing all its 
 * child folder items.
 */
void AppMenu::ConfigData::initMenuData(juce::var& menuData)
{
    using juce::var;
    using juce::String;
    if(initialized)
    {
        DBG("AppMenu::ConfigData::" << __func__ <<
                ": initialization called more than once!");
        return;
    }
    initialized = true;
    if(menuData.isVoid())
    {
        DBG("AppMenu::ConfigData::" << __func__ << ": Menu data is void");
        return;
    }

    var folderItems;
    if(menuData.isObject())
    {
        title = menuData.getProperty(titleKey, String());
        iconName = menuData.getProperty(iconKey, String());
        command = menuData.getProperty(commandKey, String());
        launchInTerm = menuData.getProperty(launchInTermKey, false);
        var categoryVar = menuData.getProperty(categoryKey, var());
        if(categoryVar.isArray())
        {
            for(const var& category : *categoryVar.getArray())
            {
                categories.add(category.operator String());
            }
        }
        folderItems = menuData.getProperty(folderItemKey, var());
    }
    else if(menuData.isArray())
    {
        folderItems = menuData;
    }
    if(folderItems.isArray())
    {
        for(var& folderItem : *folderItems.getArray())
        {
            ConfigData::Ptr child = createChildItem();
            child->initMenuData(folderItem);
            insertChild(child, getFolderSize());
        }
    }
    loadDesktopEntryItems();
}
/*
 * Cancels any desktop entry menu items waiting to load before destroying this 
 * menu item.
 */
AppMenu::ConfigData::~ConfigData()
{
    if(pendingCallbackID.get())
    {
        DesktopEntryLoader entryLoader;
        entryLoader.clearCallback(pendingCallbackID.get());
    }
}

/*
 * Gets the menu item's displayed title.
 */
juce::String 
AppMenu::ConfigData::getTitle() const
{
    return title;
}
/*
 * Gets the name or path used to load the menu item's icon file.
 */
juce::String 
AppMenu::ConfigData::getIconName() const
{
    return iconName;
}

/*
 * Gets the menu item's application launch command.
 */
juce::String 
AppMenu::ConfigData::getCommand() const
{
    return command;
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool 
AppMenu::ConfigData::getLaunchedInTerm() const
{
    return launchInTerm;
}

/*
 * Gets the application categories used to load this item's desktop entry child 
 * folder items.
 */
juce::StringArray 
AppMenu::ConfigData::getCategories() const
{
    return categories;
}

/*
 * Sets the menu item's displayed title.
 */
void 
AppMenu::ConfigData::setTitle(const juce::String& title)
{
    this->title = title;
    signalDataChanged();
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void AppMenu::ConfigData::setIconName
(const juce::String& iconName)
{
    this->iconName = iconName;
    signalDataChanged();
}

/*
 * Sets the menu item's application launch command.
 */
void AppMenu::ConfigData::setCommand
(const juce::String& command)
{
    this->command = command;
    signalDataChanged();
}

/*
 * Sets if this menu item runs its command in a new terminal window.
 */
void AppMenu::ConfigData::setLaunchedInTerm
(const bool launchInTerm)
{
    this->launchInTerm = launchInTerm;
    signalDataChanged();
}

/*
 * Sets the application categories used to load this item's desktop entry child 
 * folder items.
 */
void AppMenu::ConfigData::setCategories
(const juce::StringArray& categories)
{
    this->categories = categories;
    signalDataChanged();
}

/*
 * Gets the number of folder items held by this menu item that can be reordered.
 */
int AppMenu::ConfigData::getMovableChildCount() const
{
    int maxPossible = getFolderSize();
    for(int i = 0; i < maxPossible; i++)
    {
        if(!getChild(i)->isMovable())
        {
            return i;
        }
    }
    return maxPossible;
}

/*
 * Checks if this menu item could be moved within its folder.
 */
bool AppMenu::ConfigData::isMovable() const 
{
    return true;
}

/*
 * Gets an appropriate title to use for a deletion confirmation window.
 */
juce::String 
AppMenu::ConfigData::getConfirmDeleteTitle() const
{
    return localeText(remove_APP) 
            + getTitle() + localeText(from_favorites);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation window.
 */
juce::String 
AppMenu::ConfigData::getConfirmDeleteMessage() const
{
    return localeText(will_remove_link);
}

/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String 
AppMenu::ConfigData::getEditorTitle() const
{
    return localeText(edit_app);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool 
AppMenu::ConfigData::isEditable
(const DataField dataField) const
{
    switch(dataField)
    {
        case DataField::categories:
            return getFolderSize() > 0;
        case DataField::command:
        case DataField::termLaunchOption:
            return getFolderSize() == 0;
        case DataField::icon:
        case DataField::title:
            return true;
    }
    return false;
}

/*
 * Loads all desktop entry child menu items defined by the menu item's category
 * list.
 */
void AppMenu::ConfigData::loadDesktopEntryItems()
{
    using namespace juce;
    if(!categories.isEmpty() && pendingCallbackID.get() == 0
            && getFolderSize() <= getMovableChildCount())
    {
        DesktopEntryLoader entryLoader;
        pendingCallbackID = entryLoader.waitUntilLoaded([this]()
        {
            if(getFolderSize() > getMovableChildCount())
            {
                DBG("AppMenu::ConfigData::"
                        << "Desktop entries already loaded, skipping callback");
                return;
            }
            DesktopEntryLoader entryLoader;
            Array<DesktopEntry> entries 
                    = entryLoader.getCategoryListEntries(categories);
            for(const DesktopEntry& entry : entries)
            {
                ItemData::Ptr newItem = new DesktopEntryData(entry);
                insertChild(newItem, getFolderSize());
            }
            pendingCallbackID = 0;
        });
    }
}
