#ifndef APPMENU_IMPLEMENTATION
  #error File included outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file   AppMenu_Paged_FolderComponent.h
 *
 * @brief  Manages the folder buttons in a paged AppMenu folder.
 */

#include "AppMenu_FolderComponent.h"

namespace AppMenu { namespace Paged { class FolderComponent; } }

/**
 *  The Paged::FolderComponent divides its menu items over several folder pages,
 * only one of which may be visible at a time. The visible folder page is
 * usually the one containing the selected menu item, but any folder page may
 * be set as visible if no menu item is selected. The Paged::MenuComponent
 * is responsible for positioning the FolderComponent so that its active folder
 * page is visible.
 *
 *  Menu items within each folder page are arranged in a grid. The dimensions of
 * this grid are defined in the apps.json file, accessable through 
 * AppMenu/Settings. Paged::FolderComponent provides several methods for 
 * finding a menu item's position in its page grid, and of finding the index
 * of a menu item at a specific position on the page. These are supplied so
 * that the Paged::InputHandler can more easily provide navigational controls.
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
    virtual void updateButtonLayout() final override;
    
    /**
     * @brief  Gets the number of folder pages used by this FolderComponent.
     *
     * @return  The number of pages this folder needs to display all menu
     *          buttons.
     */
    int getNumFolderPages() const;

    /**
     * @brief  Gets the index of the folder page that is currently being shown.
     *
     * @return  The index of the visible folder page.
     */
    int getCurrentFolderPage() const;

    /**
     * @brief  Sets the index of the folder page that should be visible.
     *
     * @param indexToShow  The index of the folder page that should be visible.
     */
    void setCurrentFolderPage(const int indexToShow);

    /**
     * @brief  Gets the page column index of the selected menu item.
     *
     * @return  The column index of the selected item within its folder page, or
     *          -1 if there is no selection.
     */
    int getSelectionColumn() const;

    /**
     * @brief  Gets the page row index of the selected menu item.
     *
     * @return  The row index of the selected item within its folder page, or -1
     *          if there is no selection.
     */
    int getSelectionRow() const;

    /**
     * @brief  Finds what index value a menu item would have at a particular 
     *         position within the folder.
     * 
     * @param page    The menu item's folder page index.
     * 
     * @param column  The menu item's column index in the folder page.
     * 
     * @param row     The menu item's row index in the folder page.
     * 
     * @return        The menu item's index within the entire folder, or -1 if
     *                the page, column, or row values are out of bounds.
     */
    int positionIndex(const int page, const int column, const int row) const;

    /**
     * @brief  Selects a menu item based on its position in the folder 
     *         component.  
     *
     * The selection will not change if there isn't a button located at the 
     * given position.
     *
     * @param page    A menu item's folder page index.
     * 
     * @param column  A menu item's column index within its folder page.
     * 
     * @param row     A menu item's row index within its folder page.
     * 
     * @return        Whether a menu item at the given position was located and
     *                selected.
     */
    bool setSelectedPosition(const int page, const int column, const int row);

private:
    /**
     * @brief  Gets the number of menu items that fit in one folder page.
     *
     * @return  The maximum number of menu items visible at one time.
     */
    int maxPageItemCount() const;

    /**
     * @brief  Gets the index of the selected menu item within its folder page.
     *
     * @return  The selected item's folder page index, or -1 if no menu item is
     *          selected.
     */
    int selectedIndexInPage() const;
    
    /* Tracks which folder page is currently shown. */
    int activeFolderPage = 0;
};
