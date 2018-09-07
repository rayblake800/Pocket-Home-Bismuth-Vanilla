#pragma once
#include "JuceHeader.h"

/**
 * @file AppFolder.h
 *
 * @brief Represents an application folder.
 */
class AppFolder
{
public:
    /**
     * Create an empty folder, with no name or icon set.
     */
    AppFolder() { }

    /**
     * @param name        The display name of the application folder.
     *
     * @param categories  DesktopEntry application categories to include in the
     *                    folder.
     *
     * @param icon        The name or path of the folder's icon.
     */
    AppFolder(const juce::String name,
            const juce::StringArray& categories,
            const juce::String icon);

    /**
     * Create an AppFolder from a JSON object stored in a juce::var.
     *
     * @param jsonObj  A juce::var containing a JSON object.
     */
    AppFolder(juce::var jsonObj);

    /**
     * Package the AppFolder as a juce::DynamicObject.
     *
     * @return  All folder data stored in a DynamicObject.
     */
    juce::DynamicObject* getDynamicObject();

    /**
     * Gets the folder's display name.
     *
     * @return  The display name.
     */
    const juce::String& getName() const;

    /**
     * Gets the folder's DesktopEntry application categories.
     *
     * @return  The list of application categories to include in the folder.
     */
    const juce::StringArray& getCategories() const;

    /**
     * Gets the folder's displayed icon.
     *
     * @return  The name or path of a folder icon.
     */
    const juce::String& getIcon() const;

    /**
     * Sets the folder's display name.
     *
     * @param newName  The new display name to assign to the folder.
     */
    void setName(const juce::String newName);

    /**
     * Sets the folder's DesktopEntry application categories.
     *
     * @param newCategories  A new list of categories to replace the existing
     *                       category list.
     */
    void setCategories(const juce::StringArray& newCategories);

    /**
     * Sets the folder's displayed icon.
     *
     * @param newIcon  The name or path of a new folder icon.
     */
    void setIcon(const juce::String newIcon);

    /**
     * Compares this folder with another folder.
     *
     * @param rhs  The folder being compared with this one.
     *
     * @return  True iff this folder and rhs have the same name, icon, and 
     *          categories.
     */
    bool operator==(const AppFolder& rhs) const;

private:
    // Folder display name
    juce::String name;
    // DesktopEntry application categories
    juce::StringArray categories;
    // Icon name or path
    juce::String icon;
};
