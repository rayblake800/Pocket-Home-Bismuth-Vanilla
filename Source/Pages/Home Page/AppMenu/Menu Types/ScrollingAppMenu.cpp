
#include "ScrollingAppFolder.h"
#include "ScrollingAppMenu.h"

ScrollingAppMenu::ScrollingAppMenu(MainConfigFile& mainConfig,
        ComponentConfigFile& componentConfig,
        AppConfigFile& appConfig,
        OverlaySpinner& loadingSpinner) :
AppMenuComponent(mainConfig, componentConfig, appConfig,
ComponentConfigFile::scrollingAppMenuKey, loadingSpinner) { }

/**
 * Use key presses for menu navigation, setting specific controls based on 
 * AppMenu type. Other classes may call this to pass on or simulate
 * key events.
 */
bool ScrollingAppMenu::folderKeyPressed(const KeyPress& key,
        AppMenuFolder* activeFolder)
{
    int selectedIndex = activeFolder->getSelectedIndex();
    if (selectedIndex == -1 && getActiveFolderIndex() > 0)
    {
        activeFolder->selectIndex(0);
    }
    int newIndex = selectedIndex;
    DBG("ScrollingAppMenu::" << __func__ << ":On selected index "
            << selectedIndex << ", pressed key " << key.getTextDescription());
    if (selectedIndex == -1)
    {
        if (key.isKeyCode(KeyPress::escapeKey)
            || key.isKeyCode(KeyPress::leftKey))
        {
            closeNonBaseFolder();
            return true;
        }
        else if (key.isKeyCode(KeyPress::rightKey)
                 || key.isKeyCode(KeyPress::downKey))
        {
            newIndex = 0;
        }
        else if (!(key.isKeyCode(KeyPress::rightKey)
                   || key.isKeyCode(KeyPress::returnKey)
                   || key.isKeyCode(KeyPress::downKey)))
        {
            return false;
        }
        activeFolder->selectIndex(newIndex);
        return true;
    }
    else
    {
        if (key.isKeyCode(KeyPress::escapeKey))
        {
            activeFolder->deselect();
            activeFolder->repaint();
            return true;
        }
        else if (key.isKeyCode(KeyPress::returnKey) ||
                 key.isKeyCode(KeyPress::rightKey))
        {
            clickSelected();
            return true;
        }
        else if (key.isKeyCode(KeyPress::leftKey))
        {
            closeNonBaseFolder();
            return true;
        }
        else if (key.isKeyCode(KeyPress::upKey))
        {
            newIndex--;
        }
        else if (key.isKeyCode(KeyPress::downKey))
        {
            newIndex++;
        }
        else
        {
            return false;
        }
        if (activeFolder->selectIndex(newIndex))
        {
            layoutFolders();
        }
        return true;
    }
}

/**
 * Return the bounds where the given folder should be placed in the menu.
 * @param folder
 * @param folderIndex
 * @return 
 */
Rectangle<int> ScrollingAppMenu::updateFolderBounds(const AppMenuFolder* folder,
        int folderIndex)
{
    int activeFolder = getActiveFolderIndex();
    static Array<int> folderWidths;
    while (folderWidths.size() >= getNumFolders())
    {
        folderWidths.removeLast();
    }
    int buttonHeight = getHeight() / getMaxRows();
    while (folderWidths.size() < getNumFolders())
    {
        int folderIndex = folderWidths.size();
        folderWidths.add(getMinimumFolderWidth(folderIndex));
    }

    Point<int> selection;
    Point<int> folderPosition;
    for (int i = 0; i <= std::max<int>(activeFolder, folderIndex - 1); i++)
    {
        int yOff = std::max<int>(0, getFolderSelectedIndex(i) * buttonHeight);
        if (i <= activeFolder)
        {
            selection.addXY(folderWidths[i], yOff);
        }
        if (i < folderIndex)
        {
            folderPosition.addXY(folderWidths[i], yOff);
        }
    }
    selection.addXY(-folderWidths[activeFolder] / 2, buttonHeight / 2);
    folderPosition += (getLocalBounds().getCentre() - selection);
    Point<int> folderSize(folderWidths[folderIndex],
            folder->getButtonCount() * buttonHeight);
    return Rectangle<int>(folderPosition, folderPosition + folderSize);
}

/**
 * Create a folder component object from a folder menu item.
 * @param folderItem
 */
AppMenuFolder* ScrollingAppMenu::createFolderObject
(AppMenuItem::Ptr folderItem,
        std::map<String, AppMenuButton::Ptr>& buttonMap,
        IconThread& iconThread)
{
    ScrollingAppFolder* folder = new ScrollingAppFolder
            (folderItem, this, buttonMap, iconThread);
    folder->selectIndex(0);
    return folder;
}
