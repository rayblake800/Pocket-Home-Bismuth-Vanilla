#include "MainConfigFile.h"
#include "Utils.h"
#include "AppMenuFolder.h"

AppMenuFolder::AppMenuFolder
(AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap,
        IconThread& iconThread) :
sourceFolderItem(folderItem),
btnListener(btnListener),
buttonNameMap(buttonNameMap),
iconThread(iconThread)
{
    MainConfigFile config;
    maxRows = config.getConfigValue<int>(MainConfigFile::maxRowsKey);
    maxColumns = config.getConfigValue<int>(MainConfigFile::maxColumnsKey);
#if JUCE_DEBUG
    setName(String("AppMenuFolder") + folderItem->getAppName());
#endif
}

AppMenuFolder::~AppMenuFolder() { }

/**
 * Sets the button grid row and column sizes, updating button layout
 * if the values change
 */
void AppMenuFolder::updateGridSize(int maxRows, int maxColumns)
{
    if (maxRows != this->maxRows || maxColumns != this->maxColumns)
    {
        this->maxRows = std::max<int>(maxRows, 1);
        this->maxColumns = std::max<int>(maxColumns, 1);
        DBG("AppMenuFolder::" << __func__ << ": grid size set to " << maxRows
                << " rows, " << maxColumns << " columns");
        layoutButtons();
    }
}

/**
 * Reload all folder menu buttons from their source menu item. 
 */
void AppMenuFolder::reload()
{
    removeAllChildren();
    Array<AppMenuItem::Ptr> menuItems = sourceFolderItem->getFolderItems();
    for (AppMenuItem::Ptr menuItem : menuItems)
    {
        insertButton(menuItem, folderButtons.size(), false);
    }
    DBG("AppMenuFolder::" << __func__ << ": added " << folderButtons.size() 
            << " buttons from " << menuItems.size() << " menu items");
    layoutButtons();
}

/**
 * Set this folder's selected menu button
 */
bool AppMenuFolder::selectIndex(int index)
{
    if (validBtnIndex(index))
    {
        deselect();
        DBG("AppMenuFolder::" << __func__  << ": selecting button " << index);
        selectedIndex = index;
        folderButtons[selectedIndex]->setSelected(true);
        return true;
    }
    return false;
}

/**
 * Deselects the selected button, if one exists
 */
void AppMenuFolder::deselect()
{
    if (validBtnIndex(selectedIndex))
    {
        folderButtons[selectedIndex]->setSelected(false);
        selectedIndex = -1;
    }
}

/**
 * Creates or reloads a button for a menu item, inserting it into
 * the folder at a specific index. This shifts forward any buttons at 
 * indices equal or greater than the index. 
 */
void AppMenuFolder::insertButton
(AppMenuItem::Ptr newItem, int index, bool updateLayout)
{
    AppMenuButton::Ptr menuButton = nullptr;
    String buttonName = newItem->getAppName();
    while (buttonNameMap.count(buttonName) > 0)
    {
        AppMenuButton::Ptr mappedButton = buttonNameMap[buttonName];
        if (mappedButton->getParentComponent() == nullptr &&
            *mappedButton->getMenuItem() == *newItem)
        {
            menuButton = mappedButton;
            break;
        }
        buttonName += "_copy";
    }
    if (menuButton == nullptr)
    {
        menuButton = createMenuButton(newItem, iconThread);
        buttonNameMap[buttonName] = menuButton;
    }
    if (btnListener != nullptr)
    {
        menuButton->addMouseListener(btnListener, false);
    }
    index = median<int>(0, index, getButtonCount());
    folderButtons.insert(index, menuButton);
    if (selectedIndex >= index)
    {
        DBG("AppMenuFolder::" << __func__  << ": index pushed from " 
                << selectedIndex << " to " << (selectedIndex + 1)
                << " after insert at " << index);
        selectedIndex++;
    }
    if (updateLayout)
    {
        layoutButtons();
    }
}

/**
 * Remove the button at a given index, shifting back any buttons
 * at greater indices to fill the gap
 */
void AppMenuFolder::removeButton(int index)
{
    if (validBtnIndex(index))
    {
        DBG("AppMenuFolder::" << __func__  << ": Removing button at index "
                << index);
        folderButtons.remove(index);
        layoutButtons();
    }
}

/**
 * Swap the indices and positions of two buttons in the folder.
 * Both indices must be valid, or nothing will happen.
 */
void AppMenuFolder::swapButtons
(int btnIndex1, int btnIndex2)
{
    if (validBtnIndex(btnIndex1) && validBtnIndex(btnIndex2) &&
        folderButtons[btnIndex1]->moveDataIndex(btnIndex2 - btnIndex1))
    {
        AppMenuButton::Ptr btn1 = folderButtons[btnIndex1];
        folderButtons.set(btnIndex1, folderButtons[btnIndex2]);
        folderButtons.set(btnIndex2, btn1);
        if (selectedIndex == btnIndex1)
        {
            selectedIndex = btnIndex2;
        }
        else if (selectedIndex == btnIndex2)
        {
            selectedIndex = btnIndex1;
        }
        layoutButtons();
    }
}

/**
 * Set the relative placement of folder buttons within the folder.
 */
void AppMenuFolder::setMargin(float margin)
{
    this->margin = margin;
}

/**
 * et the relative space between folder buttons.
 */
void AppMenuFolder::setPadding(float xPadding, float yPadding)
{
    this->xPadding = xPadding;
    this->yPadding = yPadding;
}

/**
 * Clear folderLayout,remove all child components, reload the
 * button layout, and re-add the layout buttons as child
 * components at their new positions.
 */
void AppMenuFolder::layoutButtons()
{
    while (!validBtnIndex(selectedIndex) && selectedIndex != -1)
    {
        selectedIndex--;
    }
    folderLayout.clearLayout(true);
    folderLayout.setLayout(buildFolderLayout(folderButtons), this);
#ifdef JUCE_DEBUG
    //folderLayout.printLayout();
#endif
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(margin * getWidth(), margin * getWidth());
    if (!bounds.isEmpty())
    {
        folderLayout.layoutComponents(bounds, getWidth() * xPadding,
                getHeight() * yPadding);
    }
}

/**
 * @return number of menu buttons in the folder.
 */
int AppMenuFolder::getButtonCount() const
{
    return folderButtons.size();
}

/**
 * Find the index of a menu button in this folder
 */
int AppMenuFolder::getButtonIndex
(AppMenuButton::Ptr menuButton) const
{
    return folderButtons.indexOf(menuButton);
}

/**
 * Get the display name of a menu button
 */
String AppMenuFolder::getMenuButtonName(int index) const
{
    if (index < 0 || index >= folderButtons.size())
    {
        return String();
    }
    return folderButtons.getUnchecked(index)->getMenuItem()->getAppName();
}

/**
 * @return the index of the selected menu button.
 */
int AppMenuFolder::getSelectedIndex() const
{
    return selectedIndex;
}

/**
 * @return the selected button in this folder, or nullptr if there
 * is no selected button.
 */
AppMenuButton::Ptr AppMenuFolder::getSelectedButton()
{
    if (!validBtnIndex(selectedIndex))
    {
        return nullptr;
    }
    return folderButtons[selectedIndex];
}

/**
 * @return margin space between components and the edge of the
 * folder component, as a fraction of folder width.
 */
float AppMenuFolder::getMargin() const
{
    return margin;
}

/**
 * @return horizontal space between folder child
 * components, as a fraction of folder width.
 */
float AppMenuFolder::getXPadding() const
{
    return xPadding;
}

/**
 * @return vertical space between folder child
 * components, as a fraction of folder height.
 */
float AppMenuFolder::getYPadding() const
{
    return yPadding;
}

/**
 * @return the minimum width, in pixels, needed by this folder to
 * display its contents properly. 
 */
int AppMenuFolder::getMinimumWidth()
{
    return 0;
}

/**
 * Reposition folder buttons when folder bounds change.
 */
void AppMenuFolder::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(margin * getWidth(), margin * getWidth());
    DBG("AppMenuFolder::" << __func__  << ": bounds="
            << getScreenBounds().toString() << " layoutBounds=" 
            <<bounds.toString());
    folderLayout.layoutComponents(bounds, getWidth() * xPadding,
            getHeight() * yPadding);
}

/**
 * @return the title of the menu button at this index, or String::empty
 * if there is no button at this index.
 */
String AppMenuFolder::getButtonTitle(int index)
{
    if (index < 0 || index >= folderButtons.size())
    {
        return String();
    }
    return folderButtons.getUnchecked(index)->getMenuItem()->getAppName();
}

/**
 * @return the maximum number of menu item rows to show on screen
 */
int AppMenuFolder::getMaxRows() const
{
    return maxRows;
}

/**
 * @return the maximum number of menu item columns to show on screen
 */
int AppMenuFolder::getMaxColumns() const
{
    return maxColumns;
}
