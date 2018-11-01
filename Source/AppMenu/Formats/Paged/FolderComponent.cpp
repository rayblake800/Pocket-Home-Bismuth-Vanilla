#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Settings.h"
#include "AppMenu/Formats/Paged/MenuButton.h"
#include "AppMenu/Formats/Paged/FolderComponent.h"

/* Amount of empty space to leave on the left and right sides of each folder
   page, as a fraction of the page's width. */
static const constexpr float xMarginFraction = 0.05;

/* Amount of empty space to leave on the top and bottom of the folder, as a
   fraction of the folder's height. */
static const constexpr float yMarginFraction = 0.05;

/* Amount of empty space to leave on the left and right sides of each menu 
   button, as a fraction of the button's width. */
static const constexpr float buttonXPaddingFraction = 0.05;

/* Amount of empty space to leave on the top and bottom of each menu button, as
   a fraction of the button's height. */
static const constexpr float buttonYPaddingFraction = 0.05;

/*
 * Creates a new paged folder component.
 */
AppMenu::Paged::FolderComponent::FolderComponent(MenuItem folderItem) :
     AppMenu::FolderComponent(folderItem) { } 
     

/*
 * Finds the closest index to a spot on the folder that was clicked.
 */
int AppMenu::Paged::FolderComponent::closestIndex
(const int xPos, const int yPos) const 
{
    const int pageCount  = getNumFolderPages();
    const int pageWidth  = getWidth() / getNumFolderPages();
    const int pageX      = xPos % pageWidth;
    const int pageIndex  = xPos / pageWidth;
    const int pageColumn = pageX / Settings::getPagedMenuColumns(); 
    const int pageRow    = yPos / Settings::getPagedMenuRows();
    return positionIndex(pageIndex, pageColumn, pageRow);
}

/*
 * Creates a button component for one of the folder's child menu items.
 */
AppMenu::MenuButton* AppMenu::Paged::FolderComponent::createMenuButton
(const MenuItem folderItem) const 
{
    return new AppMenu::Paged::MenuButton(folderItem);
}

/*
 * Updates the sizes and positions of all buttons in the menu.
 */
void AppMenu::Paged::FolderComponent::updateButtonLayout()
{
    const int numPages    = getNumFolderPages();
    const int pageColumns = Settings::getPagedMenuColumns();
    const int pageRows    = Settings::getPagedMenuRows();
    const int pageWidth   = getWidth() / numPages;
    const int xMargin     = pageWidth * xMarginFraction;
    const int yMargin     = getHeight() * yMarginFraction;
    
    const int buttonXArea    = (pageWidth   - 2 * xMargin) / pageColumns;
    const int buttonYArea    = (getHeight() - 2 * yMargin) / pageRows;
    const int buttonXPadding = buttonXArea * buttonXPaddingFraction;
    const int buttonYPadding = buttonYArea * buttonYPaddingFraction;
    const int buttonWidth    = buttonXArea - (2 * buttonXPadding);
    const int buttonHeight   = buttonYArea - (2 * buttonYPadding);

    for(int i = 0; i < getFolderSize(); i++)
    {
        const int pageNum   = i / numPages;
        const int pageIndex = i % numPages;
        const int column    = pageIndex % pageColumns;
        const int row       = pageIndex / pageColumns;

        const int xPos = xMargin + column * buttonWidth 
            + (column * 2 + 1) * buttonXPadding;
        const int yPos = yMargin + row * buttonHeight
            + (row * 2 + 1) * buttonYPadding;

        getButtonComponent(i)->setBounds(xPos, yPos, buttonWidth, buttonHeight);
    }
}

/*
 * Gets the number of folder pages used by this FolderComponent.
 */
int AppMenu::Paged::FolderComponent::getNumFolderPages() const
{
    const int pageSize = maxPageItemCount();
    return getFolderSize() / pageSize + ((getFolderSize() % pageSize > 0) ?
        1 : 0);
}

/*
 * Gets the index of the folder page containing the selected menu item.
 */
int AppMenu::Paged::FolderComponent::getCurrentFolderPage() const
{
    const int selected = getSelectedIndex();
    if(selected <= 0)
    {
        return 0;
    }
    return selected / maxPageItemCount();
}

/*
 * Gets the page column index of the selected menu item.
 */
int AppMenu::Paged::FolderComponent::getSelectionColumn() const
{
    const int pageIndex = selectedIndexInPage();
    if(pageIndex == -1)
    {
        return -1;
    }
    return pageIndex % Settings::getPagedMenuColumns();
}

/*
 * Gets the page row index of the selected menu item.
 */
int AppMenu::Paged::FolderComponent::getSelectionRow() const
{
    const int pageIndex = selectedIndexInPage();
    if(pageIndex == -1)
    {
        return -1;
    }
    return pageIndex / Settings::getPagedMenuColumns();
}

/*
 * Finds what index value a menu item would have at a particular position within
 * the folder.
 */
int AppMenu::Paged::FolderComponent::positionIndex
(const int page, const int column, const int row) const
{
    const int maxRows = Settings::getPagedMenuRows();
    const int maxColumns = Settings::getPagedMenuColumns();
    if(page < 0 || column < 0 || row < 0 
            || column >= maxColumns || row >= maxRows)
    {
        return -1;
    }
    return page * maxRows * maxColumns 
        + row * maxColumns 
        + column;
}

/*
 * Selects a menu item based on its position in the folder component.  
 */
bool AppMenu::Paged::FolderComponent::setSelectedPosition
(int page, int column, int row)
{
    const int newIndex = positionIndex(page, column, row);
    if(newIndex < 0 || newIndex == getSelectedIndex())
    {
        return false;
    }
    setSelectedIndex(newIndex);
    return true;
}

/*
 * Gets the number of menu items that fit in one folder page.
 */
int AppMenu::Paged::FolderComponent::maxPageItemCount() const
{
    return Settings::getPagedMenuColumns() * Settings::getPagedMenuRows();
}

/*
 * Gets the index of the selected menu item within its folder page.
 */
int AppMenu::Paged::FolderComponent::selectedIndexInPage() const
{
    const int selectedIndex = getSelectedIndex();
    if(selectedIndex < 0)
    {
        return -1;
    }
    return selectedIndex % maxPageItemCount();
}
