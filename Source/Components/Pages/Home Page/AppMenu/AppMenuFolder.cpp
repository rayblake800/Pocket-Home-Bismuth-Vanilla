#include "Utils.h"
#include "AppMenuFolder.h"

/*
 * Creates a new folder component, loading menu buttons from a folder menu item.
 */
AppMenuFolder::AppMenuFolder(
        const AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<juce::String, AppMenuButton::Ptr>& buttonNameMap) :
sourceFolderItem(folderItem),
btnListener(btnListener),
buttonNameMap(buttonNameMap)
{
    using namespace juce;
#if JUCE_DEBUG
    setName(String("AppMenuFolder") + folderItem->getAppName());
#endif
}

/*
 * Sets the button grid row and column sizes, updating button layout
 * if the values change
 */
void AppMenuFolder::updateGridSize(const int maxRows, const int maxColumns)
{
    if (maxRows != this->maxRows || maxColumns != this->maxColumns)
    {
        this->maxRows = std::max<int>(maxRows, 1);
        this->maxColumns = std::max<int>(maxColumns, 1);
        //DBG("AppMenuFolder::" << __func__ << ": grid size set to " << maxRows
        //        << " rows, " << maxColumns << " columns");
        layoutButtons();
    }
}

/*
 * Reloads all folder menu buttons from their source menu item. 
 */
void AppMenuFolder::reload()
{
    using namespace juce;
    removeAllChildren();
    Array<AppMenuItem::Ptr> menuItems = sourceFolderItem->getFolderItems();
    for (AppMenuItem::Ptr menuItem : menuItems)
    {
        insertButton(menuItem, folderButtons.size(), false);
    }
    //DBG("AppMenuFolder::" << __func__ << ": added " << folderButtons.size() 
    //       << " buttons from " << menuItems.size() << " menu items");
    layoutButtons();
}

/*
 * Sets this folder's selected menu button.
 */
bool AppMenuFolder::selectIndex(const int index)
{
    if (validBtnIndex(index))
    {
        deselect();
        //DBG("AppMenuFolder::" << __func__  << ": selecting button " << index);
        selectedIndex = index;
        folderButtons[selectedIndex]->setSelected(true);
        return true;
    }
    return false;
}

/*
 * Deselects the selected button, if one exists.
 */
void AppMenuFolder::deselect()
{
    if (validBtnIndex(selectedIndex))
    {
        folderButtons[selectedIndex]->setSelected(false);
        selectedIndex = -1;
    }
}

/*
 * Creates or reloads a button for a menu item, inserting it into
 * the folder at a specific index. 
 */
void AppMenuFolder::insertButton
(const AppMenuItem::Ptr newItem, const int index, const bool updateLayout)
{
    using namespace juce;
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
        menuButton = createMenuButton(newItem);
        buttonNameMap[buttonName] = menuButton;
    }
    if (btnListener != nullptr)
    {
        menuButton->addMouseListener(btnListener, false);
    }
    int insertIndex = median<int>(0, index, getButtonCount());
    folderButtons.insert(insertIndex, menuButton);
    if (selectedIndex >= insertIndex)
    {
        //DBG("AppMenuFolder::" << __func__  << ": index pushed from " 
        //        << selectedIndex << " to " << (selectedIndex + 1)
        //        << " after insert at " << index);
        selectedIndex++;
    }
    if (updateLayout)
    {
        layoutButtons();
    }
}

/*
 * Removes the button at a given index, shifting back any buttons
 * at greater indices to fill the gap.
 */
void AppMenuFolder::removeButton(const int index)
{
    if (validBtnIndex(index))
    {
        //DBG("AppMenuFolder::" << __func__  << ": Removing button at index "
        //        << index);
        folderButtons.remove(index);
        layoutButtons();
    }
}

/*
 * Swaps the indices and positions of two buttons in the folder.
 */
void AppMenuFolder::swapButtons(const int btnIndex1, const int btnIndex2)
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

/*
 * Sets the relative size of the folder's margins.
 */
void AppMenuFolder::setMargin(const float margin)
{
    this->margin = margin;
}

/*
 * Sets the relative space between folder buttons.
 */
void AppMenuFolder::setPadding(const float xPadding, const float yPadding)
{
    this->xPadding = xPadding;
    this->yPadding = yPadding;
}

/*
 * Updates the layout of all menu buttons within the folder.
 */
void AppMenuFolder::layoutButtons()
{
    using namespace juce;
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
        folderLayout.layoutComponents(bounds);
    }
}

/*
 * Gets the number of menu buttons in the folder.
 */
int AppMenuFolder::getButtonCount() const
{
    return folderButtons.size();
}

/*
 * Finds the index of a menu button in this folder.
 */
int AppMenuFolder::getButtonIndex
(const AppMenuButton::Ptr menuButton) const
{
    return folderButtons.indexOf(menuButton);
}

/*
 * Gets the display name of a menu button.
 */
juce::String AppMenuFolder::getMenuButtonName(const int index) const
{
    using namespace juce;
    if (index < 0 || index >= folderButtons.size())
    {
        return String();
    }
    return folderButtons.getUnchecked(index)->getMenuItem()->getAppName();
}

/*
 * Gets the folder's selected menu button.
 */
int AppMenuFolder::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Gets the folder's  relative margin size.
 */
AppMenuButton::Ptr AppMenuFolder::getSelectedButton()
{
    if (!validBtnIndex(selectedIndex))
    {
        return nullptr;
    }
    return folderButtons[selectedIndex];
}

/*
 * Gets the folder's relative horizontal padding size.
 */
float AppMenuFolder::getMargin() const
{
    return margin;
}

/*
 * Gets the folder's relative horizontal padding size.
 */
float AppMenuFolder::getXPadding() const
{
    return xPadding;
}

/*
 * Gets the folder's relative vertical padding size.
 */
float AppMenuFolder::getYPadding() const
{
    return yPadding;
}

/*
 * Gets the folder's minimum width.
 */
int AppMenuFolder::getMinimumWidth()
{
    return 0;
}

/*
 * Repositions folder buttons when folder bounds change.
 */
void AppMenuFolder::resized()
{
    using namespace juce;
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(margin * getWidth(), margin * getWidth());
    //DBG("AppMenuFolder::" << __func__  << ": bounds="
    //        << getScreenBounds().toString() << " layoutBounds=" 
    //        <<bounds.toString());
    folderLayout.layoutComponents(bounds);
}

/*
 * Gets the display title of a folder menu item.
 */
juce::String AppMenuFolder::getButtonTitle(const int index)
{
    using namespace juce;
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
