#include "../../../Utils.h"
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
    ComponentConfigFile config;
    maxRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    maxColumns = config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);
    buttonsPerPage = maxRows * maxColumns;
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
        DBG(String("AppFolder: setting grid size of ") + String(maxRows)
                + String(" rows, ") + String(maxColumns) + String(" columns"));
        buttonsPerPage = maxRows * maxColumns;
        layoutButtons();
        resized();
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
    DBG(String("AppFolder::reload: added ") + String(folderButtons.size())
            + String(" Buttons from ") + String(menuItems.size())
            + String(" menu items"));
    layoutButtons();
}

/**
 * @return number of menu buttons in the folder.
 */
int AppMenuFolder::size()
{
    return folderButtons.size();
}

/**
 * Find the index of a menu button in this folder
 */
int AppMenuFolder::getButtonIndex
(AppMenuButton::Ptr menuButton)
{
    return folderButtons.indexOf(menuButton);
}

/**
 * @return The selected menu button in this folder.
 * This will only return nullptr if there are no buttons in 
 * this folder.
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
 * Set this folder's selected menu button
 */
bool AppMenuFolder::selectIndex(int index)
{
    if (validBtnIndex(index))
    {
        deselect();
        DBG(String("Selecting button ") + String(index));
        selectedIndex = index;
        folderButtons[selectedIndex]->setSelected(true);
        return true;
    }
    DBG(String("Failed to select button ") + String(index));
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
 * @return the index of the selected menu button.
 */
int AppMenuFolder::getSelectedIndex()
{
    return selectedIndex;
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
        if (mappedButton->getParentComponent() == nullptr)
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
    index = median<int>(0, index, size());
    folderButtons.insert(index, menuButton);
    if (selectedIndex >= index)
    {
        DBG(String("index pushed from ") + String(selectedIndex)
                + String(" to ") + String(selectedIndex + 1)
                + String(" after insert at ") + String(index));
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
        DBG(String("Removing button at index ") + String(index));
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
 * @return margin space between components and the edge of the
 * folder component, as a fraction of folder width.
 */
float AppMenuFolder::getMargin()
{
    return margin;
}

/**
 * @return horizontal space between folder child
 * components, as a fraction of folder width.
 */
float AppMenuFolder::getXPadding()
{
    return xPadding;
}

/**
 * @return vertical space between folder child
 * components, as a fraction of folder height.
 */
float AppMenuFolder::getYPadding()
{
    return yPadding;
}

/**
 * Reposition folder buttons when folder bounds change.
 */
void AppMenuFolder::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(margin * getWidth(), margin * getWidth());
    DBG(String("AppFolder resized, layout bounds=")
            + getScreenBounds().toString());
    folderLayout.layoutComponents(bounds, getWidth() * xPadding,
            getHeight() * yPadding);
}

/**
 * Clear folderLayout,remove all child components, reload the
 * button layout, and re-add the layout buttons as child
 * components.
 */
void AppMenuFolder::layoutButtons()
{
    while (!validBtnIndex(selectedIndex) && selectedIndex != -1)
    {
        selectedIndex--;
    }
    folderLayout.clearLayout(true);
    folderLayout.setLayout(buildFolderLayout(folderButtons), this);
    folderLayout.printLayout();
    if (!getBounds().isEmpty())
    {
        resized();
    }
}
