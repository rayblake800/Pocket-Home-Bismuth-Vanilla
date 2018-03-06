
#include "../Menu Folders/ScrollingAppFolder.h"
#include "ScrollingAppMenu.h"

ScrollingAppMenu::ScrollingAppMenu(OverlaySpinner& loadingSpinner) :
AppMenuComponent(ComponentConfigFile::scrollingAppMenuKey, loadingSpinner) { }

ScrollingAppMenu::~ScrollingAppMenu() { }

/**
 * Use key presses for menu navigation, setting specific controls based on 
 * AppMenu type. Other classes may call this to pass on or simulate
 * key events.
 */
bool ScrollingAppMenu::keyPressed(const KeyPress& key)
{
    if (isLoading() || openFolders.isEmpty()
        || Desktop::getInstance().getAnimator().isAnimating())
    {
        return true;
    }
    std::function<void() > closeNonBaseFolder = [this]()
    {
        if (getActiveFolderIndex() > 0)
        {
            closeFolder();
            layoutFolders(true);
        }
        else
        {
            loadBaseFolder();
        }
    };

    AppMenuFolder* folder = openFolders[getActiveFolderIndex()];
    int selectedIndex = folder->getSelectedIndex();
    if (selectedIndex == -1 && getActiveFolderIndex() > 0)
    {
        folder->selectIndex(0);
    }
    int newIndex = selectedIndex;
    DBG(String("On selected index ") + String(selectedIndex) + String(", pressed key ")
            + key.getTextDescription());
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
        folder->selectIndex(newIndex);
        return true;
    }
    else
    {
        if (key.isKeyCode(KeyPress::escapeKey))
        {
            folder->deselect();
            folder->repaint();
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
        if (folder->selectIndex(newIndex))
        {
            layoutFolders(true);
        }
        return true;
    }
}

/**
 * Updates the folder component layout, optionally animating the transition.
 */
void ScrollingAppMenu::layoutFolders(bool animateTransition)
{
    if (openFolders.isEmpty() || getBounds().isEmpty())
    {
        return;
    }
    int xPos = 0;
    int yPos = 0;
    int buttonHeight = getHeight() / maxRows;
    DBG(String("Button height is ") + String(buttonHeight));
    Array<Rectangle<int>> folderBounds;
    for (AppMenuFolder* folder : openFolders)
    {
        ScrollingAppFolder* scrollingFolder =
                static_cast<ScrollingAppFolder*>(folder);
        folderBounds.add(Rectangle<int>
                (xPos, yPos, 
                buttonHeight + scrollingFolder->getTextWidth(),
                 buttonHeight * scrollingFolder->size()));
        xPos += folderBounds.getLast().getWidth();
        int selected = folder->getSelectedIndex();
        if (selected > 0)
        {
            yPos += selected * buttonHeight;
        }
    }
    AppMenuFolder* activeFolder = openFolders[getActiveFolderIndex()];
    int selected = std::max<int>(0, activeFolder->getSelectedIndex());
    Rectangle<int> bounds = getLocalBounds();
    Rectangle<int> selectionBounds = folderBounds[getActiveFolderIndex()];
    DBG(String("Centering on folder ") + String(getActiveFolderIndex())
            + String(" index ") + String(selected));
    int selXCenter = selectionBounds.getCentreX();
    int selYCenter = selectionBounds.getY() + selected * buttonHeight
            + buttonHeight / 2;
    int xOffset = bounds.getCentreX() - selXCenter;
    int yOffset = bounds.getCentreY() - selYCenter;

    for (int i = 0; i < openFolders.size(); i++)
    {
        AppMenuFolder* folder = openFolders[i];
        Rectangle<int> movedBounds = folderBounds[i].translated
                (xOffset, yOffset);
        DBG(String("Moving folder to ") + movedBounds.toString());
        if (animateTransition)
        {
            Desktop::getInstance().getAnimator().animateComponent(folder,
                    movedBounds, 1, 250, true, 1, 1);
        }
        else
        {
            folder->setBounds(movedBounds);
        }
    }
}

/**
 * Create a folder component object from a folder menu item.
 * @param folderItem
 */
AppMenuFolder* ScrollingAppMenu::createFolderObject
(AppMenuItem::Ptr folderItem)
{
    return new ScrollingAppFolder(folderItem, this, buttonNameMap,
            iconThread);
}