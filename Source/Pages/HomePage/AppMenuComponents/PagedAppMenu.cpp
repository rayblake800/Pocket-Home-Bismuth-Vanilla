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
    addTrackedKeys({
        ComponentConfigFile::maxRowsKey,
        ComponentConfigFile::maxColumnsKey
    });
    ComponentConfigFile config;
    maxRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    maxColumns = config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);
    buttonsPerPage = maxRows*maxColumns;

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
        if (openFolders.size() > 1)
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
            (openFolders.getLast());
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
    if (openFolders.isEmpty())
    {
        return;
    }
    pageLeft.applyConfigBounds();
    pageRight.applyConfigBounds();
    closeFolderBtn.applyConfigBounds();
    int topEdge = 1;
    for (int i = openFolders.size() - 1; i >= 0; i--)
    {
        PageAppFolder* folder = static_cast<PageAppFolder*> (openFolders[i]);
        int folderPage = folder->getCurrentFolderPage();
        DBG(String("positioning folder with folderPage ") + String(folderPage));
        int numFolderPages = folder->getNumFolderPages();

        Rectangle<int> bounds = getLocalBounds();
        bounds.setWidth(getWidth() * numFolderPages);
        bounds.setX(-getWidth() * folderPage);
        if (topEdge < 1)
        {
            bounds.setY(topEdge - getHeight());
        }
        topEdge = bounds.getY();

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
        if (i == openFolders.size() - 1)
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
AppMenuComponent::AppFolder* PagedAppMenu::createFolderObject
(AppMenuItem::Ptr folderItem)
{
    return new PageAppFolder(folderItem, this, buttonNameMap, iconThread);
}

/**
 * Updates the layout if row/column size changes, otherwise handle
 * changes like any other ConfigurableComponent.
 * @param config the configFile containing the updated data value
 * @param key the key of property that has changed
 */
void PagedAppMenu::loadConfigProperties(ConfigFile* config, String key)
{
    ComponentConfigFile* compConf = dynamic_cast<ComponentConfigFile*> (config);
    if (compConf != nullptr)
    {
        if (key == ComponentConfigFile::maxColumnsKey)
        {
            maxColumns = compConf->getConfigValue<int>(key);
        }
        else if (key == ComponentConfigFile::maxRowsKey)
        {
            maxRows = compConf->getConfigValue<int>(key);
        }
        else
        {
            ConfigurableComponent::loadConfigProperties(config, key);
            return;
        }
        for (AppFolder* folder : openFolders)
        {
            PageAppFolder* pageFolder = static_cast<PageAppFolder*> (folder);
            pageFolder->updateGridSize(maxRows, maxColumns);
        }
        layoutFolders(false);
    }
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
            (openFolders.getLast());

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

//############################ PageAppFolder ###################################

PagedAppMenu::PageAppFolder::PageAppFolder(AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap,
        IconThread& iconThread) :
AppFolder(folderItem, btnListener, buttonNameMap, iconThread)
{
    ComponentConfigFile config;
    maxRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    maxColumns = config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);
    buttonsPerPage = maxRows * maxColumns;
    reload();
}

PagedAppMenu::PageAppFolder::~PageAppFolder() { }

/**
 * Sets the button grid row and column sizes, updating button layout
 * if the values change
 */
void PagedAppMenu::PageAppFolder::updateGridSize(int maxRows, int maxColumns)
{
    if (maxRows != this->maxRows || maxColumns != this->maxColumns)
    {
        this->maxRows = std::max<int>(maxRows, 1);
        this->maxColumns = std::max<int>(maxColumns, 1);
        DBG(String("PagedAppMenu: setting grid size of ") + String(maxRows)
                + String(" rows, ") + String(maxColumns) + String(" columns"));
        buttonsPerPage = maxRows * maxColumns;
        layoutButtons();
        resized();
    }
}

/**
 * Create an AppMenuButton component for an AppMenuItem.
 */
AppMenuButton::Ptr PagedAppMenu::PageAppFolder::createMenuButton
(AppMenuItem::Ptr menuItem)
{
    return new PageMenuButton(menuItem, iconThread,
            menuItem->getAppName() + String("Button"));
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 */
GridLayoutManager::Layout PagedAppMenu::PageAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
    if (buttons.isEmpty() || getBounds().isEmpty())
    {
        return {};
    }
    GridLayoutManager::Layout layout;
    int numPages = getNumFolderPages();
    int numSpacers = numPages - 1;
    int numAppColumns = maxColumns*numPages;
    int numPaddingGaps = numAppColumns - 1;

    int spacerSize = getMargin() * getWidth() * 2;
    int paddingSize = getXPadding() * getWidth();
    //the +2 to numSpacers is to account for the two margins
    int buttonSize = (getWidth() - (numSpacers + 2) * spacerSize
                      - numPaddingGaps * paddingSize) / numAppColumns;

    int numLayoutColumns = numAppColumns + numSpacers;

    std::function<void(Component*, int) > addButton =
            [&layout, buttonSize, spacerSize, numLayoutColumns, this]
            (Component* component, int row)
            {
                layout[row].compRow.push_back({component, buttonSize});
                if ((layout[row].compRow.size() + 1) % (maxColumns + 1) == 0
                    && layout[row].compRow.size() < numLayoutColumns)
                {
                    layout[row].compRow.push_back({nullptr, spacerSize});
                }
            };

    //reserve space
    layout.reserve(sizeof (GridLayoutManager::RowLayoutParams)
            * maxRows);
    for (int i = 0; i < maxRows; i++)
    {
        layout.push_back({1,
            {}});
        layout[i].compRow.reserve(sizeof
                (GridLayoutManager::ComponentLayoutParams)
                * numLayoutColumns);
    }

    //place buttons
    for (int i = 0; i < buttons.size(); i++)
    {
        int pageIndex = i % buttonsPerPage;
        int row = pageIndex / maxColumns;
        addButton(buttons[i], row);
    }

    //Fill in remaining empty spaces
    for (int row = 0; row < maxRows; row++)
    {
        while (layout[row].compRow.size() < numLayoutColumns)
        {
            addButton(nullptr, row);
        }
    }
    return layout;
}

/**
 * @return the number of pages this folder needs to display all menu
 * buttons.
 */
int PagedAppMenu::PageAppFolder::getNumFolderPages()
{
    int numPages = size() / buttonsPerPage;
    if (size() % buttonsPerPage > 0)
    {
        numPages++;
    }
    return numPages;
}

/**
 * @return the index of the page that's currently visible, or -1 if 
 * there is no current page. 
 */
int PagedAppMenu::PageAppFolder::getCurrentFolderPage()
{
    if (currentPage < 0 || currentPage >= getNumFolderPages())
    {
        currentPage = std::max(getSelectionPage(), 0);
    }
    return std::max(currentPage, 0);
}

/**
 * Set which folder page should currently be visible.
 */
bool PagedAppMenu::PageAppFolder::setCurrentFolderPage(int pageNum)
{
    if (pageNum < 0 || pageNum >= getNumFolderPages())
    {
        return false;
    }
    currentPage = pageNum;
    DBG(String("Moved to folder page ") + String(currentPage));
    return true;
}

/**
 * @return the folder page index containing the selected folder 
 * button, or -1 if there is no selection.
 */
int PagedAppMenu::PageAppFolder::getSelectionPage()
{
    int selected = getSelectedIndex();
    if (selected != -1)
    {
        selected /= buttonsPerPage;
    }
    return selected;
}

/**
 * @return the index of the selected button within its folder page,
 * or -1 if there is no selection.
 */
int PagedAppMenu::PageAppFolder::getSelectedIndexInFolderPage()
{
    int selected = getSelectedIndex();
    if (selected != -1)
    {
        selected %= buttonsPerPage;
    }
    return selected;
}

/**
 * @return the column index of the selected button within its
 * folder page, or -1 if there is no selection.
 */
int PagedAppMenu::PageAppFolder::getSelectionColumn()
{
    int selected = getSelectedIndexInFolderPage();
    if (selected != -1)
    {
        selected %= maxColumns;
    }
    return selected;
}

/**
 * @return the row index of the selected button within its
 * folder page, or -1 if there is no selection.
 */
int PagedAppMenu::PageAppFolder::getSelectionRow()
{
    int selected = getSelectedIndexInFolderPage();
    if (selected != -1)
    {
        selected /= maxColumns;
    }
    return selected;
}

/**
 * Finds what index value a button would have at a particular
 * position within the folder.
 */
int PagedAppMenu::PageAppFolder::positionIndex(int page, int column, int row)
{
    if (page < 0 || column < 0 || row < 0 ||
        page >= getNumFolderPages()
        || column >= maxColumns
        || row >= maxRows)
    {
        return -1;
    }
    return page * maxColumns * maxRows
            + row * maxColumns + column;
}

/**
 * Set the button selection based on its position in the
 * folder.  The selection will not change if there isn't
 * a button located at the given position.  If necessary,
 * the current folder page will change to the one containing
 * the new selection.
 *
 * @param page the new selected button's folder page index
 * @param column the new selected button's column number within
 * its folder page.
 * @param row the new selected button's row number withing
 * its folder page.
 * @return true if the selection changed, false otherwise.
 */
bool PagedAppMenu::PageAppFolder::setSelectedPosition(int page, int column, int row)
{
    DBG(String("Setting selected page=") + String(page) + String(" column=") +
            String(column) + String(" row=") + String(row));
    return selectIndex(positionIndex(page, column, row))
            && setCurrentFolderPage(page);
}

/**
 * Resizes navigation buttons, then calls AppFolder::resized()
 */
void PagedAppMenu::PageAppFolder::resized()
{
    Component* parent = getParentComponent();
    if (parent != nullptr)
    {
        Rectangle<int> bounds = getLocalBounds();
        Rectangle<int> parentBounds = parent->getLocalBounds();
        if (bounds.getWidth() > 0)
        {
            setPadding(0.5 / (maxColumns * 20) * parentBounds.getWidth()
                    / bounds.getWidth(),
                    0.5 / (maxRows * 2) * parentBounds.getHeight()
                    / bounds.getHeight());
        }
    }
    AppFolder::resized();
}

//############################  PageMenuButton  ################################

PagedAppMenu::PageMenuButton::PageMenuButton
(AppMenuItem::Ptr menuItem, IconThread& iconThread, String name) :
AppMenuButton(menuItem, iconThread, name)
{

    setFillBackground(false);
    setDrawBorder(false);
    setTextJustification(Justification::centredTop);
}

PagedAppMenu::PageMenuButton::~PageMenuButton() { }

void PagedAppMenu::PageMenuButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    DBG(getName() + String(" bounds:") + bounds.toString());
    if (bounds.isEmpty())
    {
        return;
    }
    Rectangle<float> textBounds = bounds.toFloat();
    Rectangle<float> imageBounds = bounds.toFloat();
    Font titleFont = fontResizedToFit(getTitleFont(),
            getMenuItem()->getAppName(), textBounds.toNearestInt());
    ComponentConfigFile config;
    int textHeight = config.getComponentSettings
            (ComponentConfigFile::smallTextKey).getBounds().getHeight();

    imageBounds.setHeight(imageBounds.getWidth());
    textBounds.setTop(imageBounds.getBottom());
    if (textBounds.getHeight() < textHeight + 4)
    {
        textBounds.setTop(bounds.getBottom()-(textHeight + 4));
        imageBounds.setBottom(textBounds.getY());
    }

    titleFont.setHeight(textHeight);
    setTextBounds(textBounds);
    setImageBounds(imageBounds);
    setTitleFont(titleFont);
}

/**
 * Set the background and border to only draw for selected buttons.
 */
void PagedAppMenu::PageMenuButton::selectionStateChanged()
{
    setFillBackground(isSelected());
    setDrawBorder(isSelected());
}
