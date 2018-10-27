/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "MenuComponents/FolderComponent.h"
#include "MenuData/MenuItem.h"
#include "AppMenu.h"
#include "JuceHeader.h"

/**
 * @file MenuComponent.h
 *
 * @brief  Creates and arranges application menu folder components.
 */
class AppMenu::MenuComponent : public juce::Component
{
public:
    MenuComponent();

    virtual ~MenuComponent() { }

    /**
     * @brief  Gets the MenuItem for the current active folder component.
     *
     * @return  An object holding the last opened menu folder's data.
     */
    MenuItem getActiveFolder() const;

    /**
     * @brief  Creates, shows, and activates a new folder component.
     *
     * @param folderItem  The new folder's menu data.
     */
    void openFolder(MenuItem folderItem);

    /**
     * @brief  Closes the current active folder as long as more than one
     *         folder is open. 
     */
    void closeActiveFolder();

    /**
     * @brief  Updates the positions and sizes of all open folder components.
     */
    virtual void updateMenuLayout() = 0;

    /**
     * @brief  Updates the menu's bounds when its parent component's bounds
     *         change.
     *
     * @param parentBounds  The new bounds of the menu's parent component.
     */
    virtual void parentResized(const juce::Rectangle<int> parentBounds) = 0;

    /**
     * @brief  Gets the number of open folder components.
     *
     * @return   The open folder count.
     */
    int openFolderCount() const;

    /**
     * @brief  Gets an open folder component from the list of open folders.
     *
     * @param folderIndex  The index of a folder component in the list of open
     *                     folders.
     *
     * @return             The component's address, or nullptr if the index is
     *                     out of bounds.
     */
    FolderComponent* getOpenFolder(const int folderIndex) const;

private:
    /**
     * @brief  Creates a new folder component.
     *
     * @param folderItem  The folder item that will define the new folder
     *                    component.
     *
     * @return            The new component, as some concrete subclass of
     *                    AppMenu::FolderComponent.
     */
    virtual FolderComponent* createFolderComponent(MenuItem folderItem) 
        const = 0;

    /**
     * @brief  Updates the menu layout when the component is resized.
     */
    virtual void resized() final override;
    
    /* The list of open folder components */
    juce::OwnedArray<FolderComponent> openFolders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuComponent)
};

/* Only include this file directly in the AppMenu implementation! */
#endif
