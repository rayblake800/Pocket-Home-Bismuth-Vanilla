#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/AppMenu.h"
#include "AppMenu/ConfigFile.h"
#include "AppMenu/Formats/Paged/MenuButton.h"
#include "AppMenu/Formats/Paged/FolderComponent.h"

/* Amount of empty space to leave on the left and right sides of each folder
   page, as a fraction of the page's width. */
static const constexpr float xMarginFraction = 0.05;

/* Amount of empty space to leave on the top and bottom of the folder, as a
   fraction of the folder's height. */
static const constexpr float yMarginFraction = 0.12;

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
    ConfigFile formatConfig;
    const int pageCount  = getNumFolderPages();
    const int pageWidth  = getWidth() / getNumFolderPages();
    const int pageX      = xPos % pageWidth;
    const int pageIndex  = xPos / pageWidth;
    const int columnSize = pageWidth / formatConfig.getPagedMenuColumns();
    const int rowSize    = getHeight() / formatConfig.getPagedMenuRows();
    const int pageColumn = pageX / columnSize;
    const int pageRow    = yPos / rowSize;
    int index = positionIndex(pageIndex, pageColumn, pageRow);
    if((pageX % columnSize) > (columnSize / 2))
    {
        index++;
    }
    DBG("AppMenu::Paged::FolderComponent::" << __func__
            << ": Click at (" << xPos << ", " << yPos << ") = page "
            << pageIndex << ", column " << pageColumn << ", row "
            << pageRow << ", folder index " << index);
    return index;
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
    ConfigFile formatConfig;
    const int numPages    = getNumFolderPages();
    const int pageColumns = formatConfig.getPagedMenuColumns();
    const int pageRows    = formatConfig.getPagedMenuRows();
    const int pageWidth   = getWidth() / numPages;
    const int xMargin     = pageWidth * xMarginFraction;
    const int yMargin     = getHeight() * yMarginFraction;
    
    const int buttonsPerPage = pageRows * pageColumns;
    const int buttonXArea    = (pageWidth   - 2 * xMargin) / pageColumns;
    const int buttonYArea    = (getHeight() - 2 * yMargin) / pageRows;
    const int buttonXPadding = buttonXArea * buttonXPaddingFraction;
    const int buttonYPadding = buttonYArea * buttonYPaddingFraction;
    const int buttonWidth    = buttonXArea - (2 * buttonXPadding);
    const int buttonHeight   = buttonYArea - (2 * buttonYPadding);

    for(int i = 0; i < getFolderSize(); i++)
    {
        const int pageNum   = i / buttonsPerPage;
        const int pageIndex = i % buttonsPerPage;
        const int column    = pageIndex % pageColumns;
        const int row       = pageIndex / pageColumns;

        const int xPos = pageNum * pageWidth
            + xMargin + column * buttonWidth 
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
    const int folderSize = getFolderSize();
    const int pageSize   = maxPageItemCount();
    int numPages = folderSize / pageSize;
    if((folderSize % pageSize) > 0 || numPages == 0)
    {
        numPages++;
    }
    return numPages;
}

/*
 * Gets the index of the folder page containing the selected menu item.
 */
int AppMenu::Paged::FolderComponent::getCurrentFolderPage() const
{
    return activeFolderPage;
}

/*
 * Sets the index of the folder page that should be visible.
 */
void AppMenu::Paged::FolderComponent::setCurrentFolderPage
(const int indexToShow)
{
    activeFolderPage = indexToShow;
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
    ConfigFile formatConfig;
    return pageIndex % formatConfig.getPagedMenuColumns();
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
    ConfigFile formatConfig;
    return pageIndex / formatConfig.getPagedMenuColumns();
}

/*
 * Finds what index value a menu item would have at a particular position within
 * the folder.
 */
int AppMenu::Paged::FolderComponent::positionIndex
(const int page, const int column, const int row) const
{
    ConfigFile formatConfig;
    const int maxRows = formatConfig.getPagedMenuRows();
    const int maxColumns = formatConfig.getPagedMenuColumns();
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
(const int page, const int column, const int row)
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
    ConfigFile formatConfig;
    return formatConfig.getPagedMenuColumns() * formatConfig.getPagedMenuRows();
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
