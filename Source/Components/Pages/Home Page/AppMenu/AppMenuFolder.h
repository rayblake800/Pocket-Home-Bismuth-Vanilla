// Disabled until redesign
#if 0

#pragma once
#include "JuceHeader.h"
#include "LayoutManager.h"
#include "AppMenuItem.h"
#include "AppMenuButton.h"

/**
 * @file  AppMenuFolder.h 
 * 
 * @brief  Displays, organizes and manages one folder in an AppMenuComponent.
 * 
 * AppMenuFolder components take a set of AppMenuItems, create AppMenuButton
 * components for each of them, and positions those components on the screen.
 * The AppMenuFolder allows the user to select a single button in the menu,
 * edit menu items, add new items to the folder, or remove items from the 
 * folder.  AppMenuFolder also provides methods for setting menu dimensions,
 * margin, and padding.
 *
 * AppMenuFolder is abstract, allowing for multiple alternate folder types.
 * AppMenuFolder subclasses are responsible for constructing AppManuButton
 * component objects from AppMenuItem objects, and for arranging those buttons
 * within the folder layout.
 */

class AppMenuFolder : public juce::Component, protected AppMenuItem::Editor
{
public:
    /**
     * @brief  Creates a new folder component, loading menu buttons from a
     *         folder menu item.
     * 
     * @param folderItem     Provides all menu items for this folder.
     *
     * @param btnListener    Listens to all folder menu buttons.
     */
    AppMenuFolder(const AppMenuItem folderItem, 
            const MouseListener* btnListener);

    virtual ~AppMenuFolder() { }

    /**
     * @brief  Gets the menu item used to create this folder component.
     *
     * @return  The folder's source menu item.
     */
    AppMenuItem getFolderMenuItem() const;

    /**
     * @brief  Sets this folder's selected menu button.
     *
     * @param index  The menu button index to select.
     *
     * @return       True if the selection changed, false if the index was
     *               invalid or matched the current selection and no change
     *               occurred.
     */
    bool selectIndex(const int index);

    /**
     * @brief  Deselects the selected button, if one exists.
     */
    void deselect();

    /**
     * @brief  Reloads all folder menu buttons from their source menu item.
     */
    void reload();

private:
    /**
     * @brief  Creates an AppMenuButton component for an AppMenuItem.
     * 
     * @param menuItem  The menu item to assign to the new button.
     *
     * @return          A reference-counted pointer to the new button component.
     */
    virtual AppMenuButton* createMenuButton(const AppMenuItem menuItem) = 0;

    /**
     * @brief  Swaps the indices and positions of two buttons in the folder.
     * 
     * Both indices must be valid, or nothing will happen.
     *
     * @param btnIndex1  First button index.
     *
     * @param btnIndex2  Second button index.
     */
    void swapButtons(const int btnIndex1, const int btnIndex2);

    /**
     * @brief  Gets the number of menu buttons in the folder.
     *
     * @return  The number of AppMenuButton child components.
     */
    int getButtonCount() const;

    /**
     * @brief  Gets the currently selected button's index in the folder.
     *
     * @return  The index of the selected menu button, or -1 if no button
     *          is selected.
     */
    int getSelectedIndex() const;

protected:
    /**
     * @brief  Repositions folder buttons when folder bounds change.
     */
    void resized() override;

    /**
     * @return the maximum number of menu items to show on screen.
     */
    virtual int visibleButtonCount() const = 0;

private:
    /**
     * Given a list of folder buttons, return an appropriate layout
     * for positioning them in the folder component.
     * 
     * @param buttons
     * 
     * @return a Layout containing all items in the button array.
     */
    virtual LayoutManager::Layout buildFolderLayout
    (juce::Array<AppMenuButton::Ptr>& buttons) = 0;

    /**
     * Checks if a number is a valid button index.
     *
     * @param index  The folder index to check.
     *
     * @return       Whether the folder has a menu button with this index.
     */
    inline bool validBtnIndex(int index)
    {
        return index >= 0 && index < folderButtons.size();
    }

    /* Listener to assign to all button components */
    juce::MouseListener* btnListener = nullptr;
    /* Holds the folder menu item used to load this folder's menu items */
    AppMenuItem sourceFolderItem;
    /* Holds all menu buttons */
    juce::Array<AppMenuButton::Ptr> folderButtons;
    /* Tracks selected button index */
    int selectedIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppMenuFolder)
};

//Disabled until redesign
#endif
