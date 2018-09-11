#include "PageAppFolder.h"
#include "PagedAppMenu.h"
#include "ComponentConfigKeys.h"

//folder navigation key bindings
//TODO: set these in config
const juce::String PagedAppMenu::pageLeftBinding = "shift + cursor left";
const juce::String PagedAppMenu::pageRightBinding = "shift + cursor right";

PagedAppMenu::PagedAppMenu(OverlaySpinner& loadingSpinner) :
AppMenuComponent(ComponentConfigKeys::pagedAppMenuKey, loadingSpinner),
pageLeft(NavButton::left),
pageRight(NavButton::right),
closeFolderBtn(NavButton::up)
{
    using namespace juce;
    setOnlyTriggerSelected(false);
    Array<Button*> buttons = {&pageLeft, &pageRight, &closeFolderBtn};

    for (Button* button : buttons)
    {
        addChildComponent(button);
        button->setWantsKeyboardFocus(false);
        button->setAlwaysOnTop(true);
        button->addListener(this);
    }
}

/**
 * Update navigation buttons when the menu changes size.
 */
void PagedAppMenu::menuResized()
{
    pageLeft.applyConfigBounds();
    pageRight.applyConfigBounds();
    closeFolderBtn.applyConfigBounds();
}

/**
 * Uses key events to navigate through the menu.
 */
bool PagedAppMenu::folderKeyPressed(const juce::KeyPress& key,
        AppMenuFolder* activeFolder)
{
    using namespace juce;
    PageAppFolder* folder = static_cast<PageAppFolder*> (activeFolder);
    int selectedIndex = folder->getSelectedIndex();
    int currentPage = folder->getCurrentFolderPage();
    if (folder->getSelectionPage() != currentPage)
    {
        folder->deselect();
        selectedIndex = -1;
    }
    KeyPress pageLeftKey = KeyPress::createFromDescription(pageLeftBinding);
    KeyPress pageRightKey = KeyPress::createFromDescription(pageRightBinding);
    if (key == pageLeftKey || key == pageRightKey)
    {
        int newPage = currentPage + (key == pageLeftKey ? -1 : 1);
        if (folder->setCurrentFolderPage(newPage))
        {
            layoutFolders(true);
        }
        return true;
    }
    int newRow = 0;
    int newColumn = 0;
    //DBG("PagedAppMenu::" << __func__ << ": On selected index " << selectedIndex
    //        << ", pressed key " << key.getTextDescription());
    if (selectedIndex == -1)
    {
        if (key.isKeyCode(KeyPress::escapeKey))
        {
            closeNonBaseFolder();
            return true;
        }
        else if (key.isKeyCode(KeyPress::leftKey)
                 || key.isKeyCode(KeyPress::upKey))
        {
            int targetIndex = std::min((currentPage + 1) * buttonsPerPage() - 1,
                    folder->getButtonCount() - 1);
            if (folder->selectIndex(targetIndex))
            {
                folder->repaint();
            }
            return true;
        }
        else if (!(key.isKeyCode(KeyPress::rightKey)
                   || key.isKeyCode(KeyPress::returnKey)
                   || key.isKeyCode(KeyPress::downKey)))
        {
            return false;
        }
        if (folder->setSelectedPosition(currentPage, newColumn, newRow))
        {
            folder->repaint();
        }
        return true;
    }
    else
    {
        newRow = folder->getSelectionRow();
        newColumn = folder->getSelectionColumn();
        int newPage = currentPage;
        if (key.isKeyCode(KeyPress::escapeKey))
        {
            folder->deselect();
            folder->repaint();
            return true;
        }
        else if (key.isKeyCode(KeyPress::returnKey))
        {
            clickSelected();
            return true;
        }
        else if (key.isKeyCode(KeyPress::leftKey))
        {
            newColumn--;
            if (newColumn < 0)
            {
                newColumn = getMaxColumns() - 1;
                newPage--;
            }
        }
        else if (key.isKeyCode(KeyPress::rightKey))
        {
            newColumn++;
            if (newColumn >= getMaxColumns())
            {
                newColumn = 0;
                newPage++;
                while (folder->positionIndex(newPage, newColumn, newRow) >=
                       folder->getButtonCount() && newRow > 0)
                {
                    newRow--;
                }
            }
        }
        else if (key.isKeyCode(KeyPress::upKey))
        {
            if (newRow > 0)
            {
                newRow--;
            }
        }
        else if (key.isKeyCode(KeyPress::downKey))
        {
            if (newRow < (getMaxRows() - 1))
            {
                newRow++;
            }

            while (folder->positionIndex(newPage, newColumn, newRow) >=
                   folder->getButtonCount() && newColumn > 0)
            {
                newColumn--;
            }
        }
        else
        {
            return false;
        }
        if (folder->setSelectedPosition(newPage, newColumn, newRow))
        {
            if (newPage != currentPage)
            {
                layoutFolders(true);
            }
            else
            {
                folder->repaint();
            }
        }
        return true;
    }
}

/**
 * Return the bounds where the given folder should be placed in the menu.
 */
juce::Rectangle<int> PagedAppMenu::updateFolderBounds
(const AppMenuFolder* folder, int folderIndex)
{
    using namespace juce;
    const PageAppFolder* pageFolder =
            static_cast<const PageAppFolder*> (folder);
    int folderPage = pageFolder->getCurrentFolderPage();
    int numFolderPages = pageFolder->getNumFolderPages();
    int folderWidth = getWidth() - pageLeft.getWidth()*2;

    Rectangle <int> bounds = getLocalBounds();
    bounds.setWidth(getWidth() * numFolderPages);
    bounds.setY(-getHeight() * (getActiveFolderIndex() - folderIndex));
    bounds.setX(-getWidth() * folderPage);
    if (folderIndex == getActiveFolderIndex())
    {
        bool showLeft = folderPage > 0;
        bool showRight = folderPage < (numFolderPages - 1);
        bool showUp = getActiveFolderIndex() > 0;
        pageLeft.setVisible(showLeft);
        pageLeft.setEnabled(showLeft);
        pageRight.setVisible(showRight);
        pageRight.setEnabled(showRight);
        closeFolderBtn.setVisible(showUp);
        closeFolderBtn.setEnabled(showUp);
    }
    //DBG("PagedAppMenu::" << __func__ << ": folder " << folderIndex << " is at "
    //        << bounds.toString());
    return bounds;
}

/**
 * Create a folder component object from a folder menu item.
 * @param folderItem
 */
AppMenuFolder* PagedAppMenu::createFolderObject(
        AppMenuItem::Ptr folderItem,
        std::map<juce::String, AppMenuButton::Ptr>& buttonMap)
{
    PageAppFolder* folder = new PageAppFolder(folderItem, this, buttonMap);
    folder->setParentRelativeMargin(
            (float) pageLeft.getWidth() / (float) getWidth());
    return folder;
}

/**
 * Handles navigation button controls
 */
void PagedAppMenu::buttonClicked(juce::Button* button)
{
    using namespace juce;
    if (!ignoringInput())
    {
        if (button == &closeFolderBtn && getNumFolders() > 1)
        {
            closeFolder();
        }
        else if (button == &pageLeft)
        {
            keyPressed(KeyPress::createFromDescription(pageLeftBinding));
        }
        else if (button == &pageRight)
        {
            keyPressed(KeyPress::createFromDescription(pageRightBinding));
        }
    }
}
