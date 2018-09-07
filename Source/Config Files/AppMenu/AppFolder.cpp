#include "Source/Config Files/AppMenu/AppFolder.h"

AppFolder::AppFolder(
        const juce::String name,
        const juce::StringArray& categories,
        const juce::String icon) :
name(name),
categories(categories),
icon(icon) { }

/*
 * Create an AppFolder from a JSON object stored in a juce::var.
 */
AppFolder::AppFolder(juce::var jsonObj)
{
    using namespace juce;
    name = jsonObj.getProperty("name", "");
    icon = jsonObj.getProperty("icon", "");

    var catList = jsonObj["categories"];
    if (catList.isArray())
    {
        for (var category : *catList.getArray())
        {
            categories.add(category);
        }
    }
}

/*
 * Package the AppFolder as a juce::DynamicObject.
 */
juce::DynamicObject* AppFolder::getDynamicObject()
{
    using namespace juce;
    DynamicObject * folderObject = new DynamicObject();
    folderObject->setProperty("name", name);
    folderObject->setProperty("icon", icon);

    Array<var> categoryArray;
    for (int i = 0; i < categories.size(); i++)
    {
        categoryArray.add(categories[i]);
    }
    folderObject->setProperty(String("categories"), categoryArray);
    return folderObject;
}

/*
 * Gets the folder's display name.
 */
const juce::String& AppFolder::getName() const
{
    return name;
}

/*
 * Gets the folder's DesktopEntry application categories.
 */
const juce::StringArray& AppFolder::getCategories() const
{
    return categories;
}

/*
 * Gets the folder's displayed icon.
 */
const juce::String& AppFolder::getIcon() const
{
    return icon;
}

/*
 * Sets the folder's display name.
 */
void AppFolder::setName(const juce::String newName)
{
    name = newName;
}

/*
 * Sets the folder's DesktopEntry application categories.
 */
void AppFolder::setCategories(const juce::StringArray& newCategories)
{
    categories = newCategories;
}

/*
 * Sets the folder's displayed icon.
 */
void AppFolder::setIcon(const juce::String newIcon)
{
    icon = newIcon;
}

 /*
 * Compares this folder with another folder.
 */
bool AppFolder::operator==(const AppFolder& rhs) const
{
    return name == rhs.name
        && categories == rhs.categories
        && icon == rhs.icon;
}
