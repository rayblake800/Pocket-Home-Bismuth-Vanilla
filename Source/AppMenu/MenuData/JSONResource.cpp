#define APPMENU_IMPLEMENTATION_ONLY
#include "JSONResource.h"
#include "ConfigKeys.h"

/* SharedResource object key */
const juce::Identifier AppMenu::JSONResource::resourceKey
    ("AppMenu::JSONResource");

/* JSON configuration file name */
static const constexpr char * jsonFilename = "apps.json";

AppMenu::JSONResource::JSONResource() : Config::FileResource
(resourceKey, jsonFilename)
{
    using namespace juce;
    // Load menu:
    ConfigData::Ptr rootItem = new ConfigData();
    rootFolderItem = MenuItem(rootItem);
    loadJSONData(); 
    var rootFolder = initProperty<var>(folderItemKey);
    ((ConfigData*) rootItem.get())->initMenuData(rootFolder); 
}


/*
 * Destroys all menu data on destruction.
 */
AppMenu::JSONResource::~JSONResource()
{
    rootFolderItem = MenuItem();
}

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenu::MenuItem AppMenu::JSONResource::getRootFolderItem() const
{
    return rootFolderItem;
}

/**
 * @brief  Recursively copies a menu item and all of its child folder items
 *         into a juce::var object.
 *
 * @param menuItem  A non-null item located in the application menu.
 *
 * @return          All menu item data packaged in an object var. 
 */
static juce::var itemToVar(const AppMenu::MenuItem& menuItem)
{
    using namespace juce;
    DynamicObject::Ptr itemObject = new DynamicObject();
    itemObject->setProperty(AppMenu::titleKey, menuItem.getTitle());
    itemObject->setProperty(AppMenu::iconKey, menuItem.getIconName());
    if(menuItem.isFolder())
    {
        int configChildCount = menuItem.getMovableChildCount();
        if(configChildCount > 0)
        {
            Array<var> folderItems;
            for(int i = 0; i < configChildCount; i++)
            {
                AppMenu::MenuItem folderItem = menuItem.getFolderItem(i);
                if(!folderItem.isNull())
                {
                    folderItems.add(itemToVar(folderItem));
                }
            }
            itemObject->setProperty(AppMenu::folderItemKey, folderItems);
        }
        StringArray categories = menuItem.getCategories();
        if(!categories.isEmpty())
        {
            itemObject->setProperty(AppMenu::categoryKey, categories);
        }
    }
    else
    {
        itemObject->setProperty(AppMenu::commandKey, menuItem.getCommand());
        itemObject->setProperty(AppMenu::launchInTermKey, 
                menuItem.getLaunchedInTerm());
    }
    return var(itemObject);
}

/*
 * Copies all menu data back to the JSON configuration file.
 */
void AppMenu::JSONResource::writeDataToJSON()
{
    using namespace juce;
    MenuItem rootItem = getRootFolderItem();
    int numItems = rootItem.getMovableChildCount();
    if(numItems > 0)
    {
        Array<var> rootFolder;
        for(int i = 0; i < numItems; i++)
        {
            MenuItem folderItem = rootItem.getFolderItem(i);
            if(!folderItem.isNull())
            {
                rootFolder.add(itemToVar(folderItem));
            }
        }
        updateProperty<Array<var>>(folderItemKey, rootFolder);
    }
}

/**
 * Adds a new menu item to the list of menu items.
 */
AppMenu::MenuItem AppMenu::JSONResource::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        MenuItem& parentFolder,
        const int index)
{
    if(parentFolder.isNull() || index < 0 
            || index > parentFolder.getMovableChildCount())
    {
        return MenuItem();
    }
    using namespace juce;
    
    ItemData::Ptr newData = new ConfigData();
    newData->setTitle(title);
    newData->setIconName(icon);
    newData->setCommand(command);
    newData->setLaunchedInTerm(launchInTerm);
    if(!categories.isEmpty())
    {
        newData->setCategories(categories);
        static_cast<ConfigData*>(newData.get())->loadDesktopEntryItems();
    }
    MenuItem newItem(newData);
    if(!parentFolder.insertChild(newItem, index))
    {
        return MenuItem();
    }
    newItem.saveChanges();
    return newItem;
}


/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenu::JSONResource::ConfigData::saveChanges()
{
    JSONWriter jsonWriter;
    jsonWriter.writeChanges();
}

/*
 * Removes this menu item from JSONResource's config file.
 */
void AppMenu::JSONResource::ConfigData::deleteFromSource()
{
    // This should only be called after a call to remove()
    jassert(getIndex() == -1 && getParentFolder() == nullptr);
    saveChanges();
}


/*
 * Creates an empty child menu item.
 */
AppMenu::ConfigData::Ptr 
AppMenu::JSONResource::ConfigData::createChildItem()
{
    return new ConfigData;
}

/*
 * Writes all config-defined menu data back to the JSON file.
 */
void AppMenu::JSONResource::ConfigData::JSONWriter::writeChanges()
{
    auto appJSON = getWriteLockedResource();
    appJSON->writeDataToJSON();
}

