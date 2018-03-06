#include "../Menu Folders/PageAppFolder.h"
#include "PagedAppMenu.h"

/**
 * @param loadingSpinner
 */
PagedAppMenu::PagedAppMenu(OverlaySpinner& loadingSpinner) :
AppMenuComponent(ComponentConfigFile::pagedAppMenuKey, loadingSpinner),
pageLeft(ComponentConfigFile::pageLeftKey),
pageRight(ComponentConfigFile::pageRightKey),
closeFolderBtn(ComponentConfigFile::pageUpKey)
{
    addAndMakeVisible(pageLeft);
    pageLeft.setWantsKeyboardFocus(false);
    pageLeft.setAlwaysOnTop(true);
    pageLeft.addListener(this);

    addAndMakeVisible(pageRight);
    pageRight.setWantsKeyboardFocus(false);
    pageRight.setAlwaysOnTop(true);
    pageRight.addListener(this);

    addAndMakeVisible(closeFolderBtn);
    closeFolderBtn.setWantsKeyboardFocus(false);
    closeFolderBtn.setAlwaysOnTop(true);
    closeFolderBtn.addListener(this);
}

PagedAppMenu::~PagedAppMenu() { }

/**
 * Use key presses for menu navigation, setting specific controls based on 
 * AppMenu type. Other classes may call this to pass on or simulate
 * key events.
 * @param key
 * @return true if the key press was used.
 */
bool PagedAppMenu::keyPressed(const KeyPress& key)
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

    PageAppFolder* folder = static_cast<PageAppFolder*>
            (openFolders[getActiveFolderIndex()]);
    int selectedIndex = folder->getSelectedIndex();
    int currentPage = folder->getCurrentFolderPage();
    if (folder->getSelectionPage() != currentPage)
    {
        folder->deselect();
        selectedIndex = -1;
    }
    int newRow = 0;
    int newColumn = 0;
    DBG(String("On selected index ") + String(selectedIndex) + String(", pressed key ")
            + key.getTextDescription());
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
            int targetIndex = std::min((currentPage + 1) * buttonsPerPage - 1,
                    folder->size() - 1);
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
                newColumn = maxColumns - 1;
                newPage--;
            }
        }
        else if (key.isKeyCode(KeyPress::rightKey))
        {
            newColumn++;
            if (newColumn >= maxColumns)
            {
                newColumn = 0;
                newPage++;
                while (folder->positionIndex(newPage, newColumn, newRow) >=
                       folder->size() && newRow > 0)
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
            if (newRow < (maxRows - 1))
            {
                newRow++;
            }

            while (folder->positionIndex(newPage, newColumn, newRow) >=
                   folder->size() && newColumn > 0)
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
 * Updates the folder component layout, optionally animating the transition.
 * @param animateTransition if true, animate component changes rather than
 * immediately updating folder bounds.
 */
void PagedAppMenu::layoutFolders(bool animateTransition)
{
    static int lastFolder = -1;
    static int lastPage = -1;
    if (openFolders.isEmpty() || 
            (lastFolder == getActiveFolderIndex() &&
            lastPage == static_cast<PageAppFolder*>
            (openFolders[getActiveFolderIndex()])
            ->getCurrentFolderPage()))
    {
        return;
    }
    lastFolder = getActiveFolderIndex();
    lastPage =  static_cast<PageAppFolder*>
            (openFolders[getActiveFolderIndex()])
            ->getCurrentFolderPage();
    pageLeft.applyConfigBounds();
    pageRight.applyConfigBounds();
    closeFolderBtn.applyConfigBounds();
    for (int i = openFolders.size() - 1; i >= 0; i--)
    {
        PageAppFolder* folder = static_cast<PageAppFolder*> (openFolders[i]);
        int folderPage = folder->getCurrentFolderPage();
        DBG(String("positioning folder with folderPage ") + String(folderPage));
        int numFolderPages = folder->getNumFolderPages();

        Rectangle<int> bounds = getLocalBounds();
        bounds.setWidth(getWidth() * numFolderPages);
        bounds.setX(-getWidth() * folderPage);
        bounds.setY(getHeight() * (getActiveFolderIndex()-i));
        if (folder->getBounds().isEmpty())
        {
            folder->setBounds(bounds.withY(getBottom()));
        }
        float margin = (float) pageLeft.getWidth()
                / (float) bounds.getWidth();
        if (margin != folder->getMargin())
        {
            folder->setMargin(margin);
            folder->layoutButtons();
        }
        DBG(String("Moving folder to bounds ") + bounds.toString());
        if (animateTransition)
        {
            Desktop::getInstance().getAnimator().animateComponent(folder,
                    bounds, 1, 250, true, 1, 1);
        }
        else
        {
            folder->setBounds(bounds);
        }

        //update navigation buttons after changing last open folder bounds
        if (i == getActiveFolderIndex())
        {
            bool showLeft = folderPage > 0;
            bool showRight = folderPage < (numFolderPages - 1);
            bool showUp = openFolders.size() > 1;
            pageLeft.setVisible(showLeft);
            pageLeft.setEnabled(showLeft);
            pageRight.setVisible(showRight);
            pageRight.setEnabled(showRight);
            closeFolderBtn.setVisible(showUp);
            closeFolderBtn.setEnabled(showUp);
        }
    }
}

/**
 * Create a folder component object from a folder menu item.
 * @param folderItem
 */
AppMenuFolder* PagedAppMenu::createFolderObject
(AppMenuItem::Ptr folderItem)
{
    return new PageAppFolder(folderItem, this, buttonNameMap, iconThread);
}

/**
 * Handles navigation button controls
 */
void PagedAppMenu::buttonClicked(Button* button)
{
    if (openFolders.isEmpty() ||
        !(button == &pageLeft 
          || button == &pageRight
          || button == &closeFolderBtn))
    {
        return;
    }
    if(button == &closeFolderBtn && openFolders.size() > 1)
    {
        closeFolder();
        layoutFolders(true);
        return;
    }
    PageAppFolder* folder = static_cast<PageAppFolder*>
            (openFolders[getActiveFolderIndex()]);

    int targetPage = folder->getCurrentFolderPage();
    if (button == &pageLeft)
    {
        targetPage--;
    }
    else if (button == &pageRight)
    {
        targetPage++;
    }
    if (folder->setCurrentFolderPage(targetPage))
    {
        layoutFolders(true);
    }
}
