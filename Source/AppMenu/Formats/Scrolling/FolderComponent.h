/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/Components/FolderComponent.h"

/**
 * @file   FolderComponent.h
 *
 * @brief  Manages the folder buttons in a scrolling AppMenu folder.
 *
 * The Scrolling::FolderComponent arranges its menu buttons in a simple
 * vertical list.  To assist the Scrolling::MenuComponent in creating the folder
 * layout, it also shares how wide it needs to be to fit the widest menu item
 * title, and the pixel offset of the current selected menu item.
 */
class AppMenu::Scrolling::FolderComponent : public AppMenu::FolderComponent
{
public:
    /**
     * @brief  Creates a new scrolling folder component.
     *
     * @param folderItem  The MenuItem that defines the folder.
     */
    FolderComponent(MenuItem folderItem);

    virtual ~FolderComponent() { }

    /**
     * @brief  Finds the closest index to a spot on the folder that was clicked.
     *
     * @param xPos  The x-coordinate of the spot that was clicked, measured in
     *              pixels from the left edge of the folder component.
     * 
     * @param yPos  The y-coordinate of the spot that was clicked, measured in
     *              pixels from the top edge of the folder component.
     *
     * @return      The closest menu index to the clicked spot, to use when
     *              inserting new menu items.
     */
    virtual int closestIndex(const int xPos, const int yPos) 
        const final override;

    /**
     * @brief  Creates a button component for one of the folder's child menu
     *         items.
     *
     * @param folderItem  The menu item data that will define the new button.
     *
     * @return            The button component, initialized as a 
     *                    AppMenu::Scrolling::MenuButton.
     */
    virtual AppMenu::MenuButton* createMenuButton(const MenuItem folderItem)
        const final override;

    /**
     * @brief  Updates the sizes and positions of all buttons in the menu.
     */
    virtual void updateButtonLayout() final override;

    /**
     * @brief  Gets the distance between the top of the folder component and
     *         its selected menu item.
     *
     * @return  The distance in pixels between the top of the folder and the top
     *          of the selected item, or zero if no folder item is selected.
     */
    int getSelectedItemYOffset() const;

    /**
     * @brief  Gets the minimum width needed to fit this folder's menu items.
     *
     * @return  The minimum folder width, in pixels.
     */
    int getMinimumWidth() const;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
