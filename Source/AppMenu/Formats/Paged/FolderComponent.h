/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu/MenuComponents/FolderComponent.h"

/**
 * @file   FolderComponent.h
 *
 * @brief  Manages the folder buttons in a paged AppMenu folder.
 */
class AppMenu::Paged::FolderComponent : public AppMenu::FolderComponent
{
public:
    /**
     * @brief  Creates a new paged folder component.
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
     *                    AppMenu::Paged::MenuButton.
     */
    virtual AppMenu::MenuButton* createMenuButton(const MenuItem folderItem)
        const final override;

    /**
     * @brief  Updates the sizes and positions of all buttons in the menu.
     */
    virtual void updateButtonLayout() const final override;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
