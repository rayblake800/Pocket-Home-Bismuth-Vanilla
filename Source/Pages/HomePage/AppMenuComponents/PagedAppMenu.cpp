#include "PagedAppMenu.h"

/**
 * @param loadingSpinner
 */
PagedAppMenu::PagedAppMenu(OverlaySpinner& loadingSpinner) :
AppMenuComponent(ComponentConfigFile::pagedAppMenuKey, loadingSpinner),
pageLeft(ComponentConfigFile::pageLeftKey),
pageRight(ComponentConfigFile::pageRightKey)
{
    addTrackedKeys({
        ComponentConfigFile::maxRowsKey,
        ComponentConfigFile::maxColumnsKey
    });
    ComponentConfigFile config;
    maxRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    maxColumns = config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);

    pageLeft.addListener(this);
    pageRight.addListener(this);
    addAndMakeVisible(pageLeft);
    addAndMakeVisible(pageRight);
    pageLeft.setAlwaysOnTop(true);
    pageRight.setAlwaysOnTop(true);
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
    if (isLoading() || Desktop::getInstance().getAnimator().isAnimating())
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

    int selectedIndex = openFolders.getLast()->getSelectedIndex();
    int pageIndex = selectedIndex % (maxColumns * maxRows);
    int pageRow = pageIndex / maxColumns;
    int newIndex = selectedIndex;
    if (key.isKeyCode(KeyPress::escapeKey))
    {
        if (selectedIndex == -1)
        {
            closeNonBaseFolder();
        }
        else
        {
            openFolders.getLast()->deselect();
        }
        return true;
    }
    else if (key.isKeyCode(KeyPress::leftKey))
    {
        newIndex = selectedIndex - 1;
        if (newIndex < -1)
        {
            closeNonBaseFolder();
            return true;
        }
    }
    else if (key.isKeyCode(KeyPress::rightKey))
    {
        newIndex = selectedIndex + 1;
    }
    else if (key.isKeyCode(KeyPress::upKey))
    {
        if (pageRow > 0)
        {
            newIndex = selectedIndex - maxColumns;
        }
    }
    else if (key.isKeyCode(KeyPress::downKey))
    {
        if (pageRow < maxRows - 1)
        {
            newIndex = selectedIndex + maxColumns;
        }
    }

    else if (key.isKeyCode(KeyPress::returnKey))
    {
        if (selectedIndex == -1)
        {
            newIndex = 0;
        }
        else
        {
            clickSelected();
        }
    }
    else
    {
        return false;
    }
    if (newIndex == -1)
    {
        openFolders.getLast()->deselect();
    }
    else if (newIndex != selectedIndex
             && openFolders.getLast()->selectIndex(newIndex))
    {
        layoutFolders(true);
    }
    return true;
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
    int appsPerPage = maxRows * maxColumns;
    int leftEdge = 1;
    for (int i = openFolders.size() - 1; i >= 0; i--)
    {
        PageAppFolder* folder = dynamic_cast<PageAppFolder*> (openFolders[i]);
        int selectedIndex = std::max(folder->getSelectedIndex(), 0);
        int numPages = folder->size() / appsPerPage;
        if (folder->size() % appsPerPage != 0)
        {
            numPages++;
        }
        int currentPage = selectedIndex / appsPerPage;
        Rectangle<int> bounds = getLocalBounds();
        bounds.setWidth(getWidth() * numPages);
        if (leftEdge > 0)
        {
            bounds.setX(-getWidth() * currentPage);
        }
        else
        {
            bounds.setRight(leftEdge);
        }
        leftEdge = bounds.getX();
        folder->setMargin((float) pageLeft.getWidth()
                / (float) bounds.getWidth());
        if (animateTransition)
        {
            Desktop::getInstance().getAnimator().animateComponent(folder,
                    bounds, 1, 500, true, 1, 1);
        }
        else
        {
            folder->setBounds(bounds);
        }

        //update navigation buttons after changing last open folder bounds
        if (i == openFolders.size() - 1)
        {
            bool showLeft = bounds.getX() < 0;
            bool showRight = bounds.getRight() > getWidth();
            pageLeft.setVisible(showLeft);
            pageLeft.setEnabled(showLeft);
            pageRight.setVisible(showRight);
            pageRight.setEnabled(showRight);
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
 * @param 
 */
void PagedAppMenu::buttonClicked(Button* button)
{
    ComponentConfigFile config;
    int numRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    int numColumns =
            config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);
    int btnPerPage = numRows * numColumns;
    int currentPage = std::max(0,
            openFolders.getLast()->getSelectedIndex() / btnPerPage);
    int targetIndex = currentPage*btnPerPage;
    if (button == &pageLeft)
    {
        targetIndex -= btnPerPage;
    }
    else if (button == &pageRight)
    {
        targetIndex += btnPerPage;
    }
    if (openFolders.getLast()->selectIndex(targetIndex))
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
    updateGridSize(config.getConfigValue<int>(ComponentConfigFile::maxRowsKey),
            config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey));
    reload();
}

PagedAppMenu::PageAppFolder::~PageAppFolder() { }

/**
 * Sets the button grid row and column sizes, updating button layout
 * if the values change
 */
void PagedAppMenu::PageAppFolder::updateGridSize(int maxRows, int maxColumns)
{
    if (maxRows != maxRowNum || maxColumns != maxColumnNum)
    {
        maxRowNum = std::max<int>(maxRows, 1);
        maxColumnNum = std::max<int>(maxColumns, 1);
        DBG(String("PagedAppMenu: setting grid size of ") + String(maxRowNum)
                + String(" rows, ") + String(maxColumnNum) + String(" columns"));
        buttonsPerPage = maxRowNum * maxColumnNum;
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
 * 
 * @param buttons
 * @return a Layout containing all items in the button array.
 */
GridLayoutManager::Layout PagedAppMenu::PageAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
    ComponentConfigFile config;
    int numRows = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey);
    int numColumns =
            config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey);
    int btnPerPage = numRows * numColumns;
    int numPages = buttons.size() / btnPerPage;
    if (buttons.size() % btnPerPage != 0)
    {
        numPages++;
    }
    GridLayoutManager::Layout layout;
    layout.reserve(sizeof (GridLayoutManager::RowLayoutParams) * numRows);
    int layoutRowSize = numPages * numColumns;
    for (int i = 0; i < buttons.size(); i++)
    {
        int pageIndex = i % btnPerPage;
        int row = pageIndex / numColumns;
        if (layout.size() <= row)
        {
            layout.push_back({1,
                {}});
            layout[row].compRow.reserve(sizeof
                    (GridLayoutManager::ComponentLayoutParams) * layoutRowSize);
        }
        layout[row].compRow.push_back({buttons[i], 1});
    }
    for (int i = 0; i < layout.size(); i++)
    {
        while (layout[i].compRow.size() < layoutRowSize)
        {
            layout[i].compRow.push_back({nullptr, 1});
        }
    }
    return layout;
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
            setPadding(0.5 / (maxColumnNum * 2) * parentBounds.getWidth()
                    / bounds.getWidth(),
                    0.5 / (maxRowNum * 2) * parentBounds.getHeight()
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
    if (bounds.isEmpty())
    {
        return;
    }
    Rectangle<float> textBounds = bounds.toFloat();
    Rectangle<float> imageBounds = bounds.toFloat();
    Font titleFont = fontResizedToFit(getTitleFont(),
            getMenuItem()->getAppName(), textBounds.toNearestInt());
    ComponentConfigFile config;
    int minTextHeight = config.getComponentSettings
            (ComponentConfigFile::smallTextKey).getBounds().getHeight();
    int maxTextHeight = config.getComponentSettings
            (ComponentConfigFile::mediumTextKey).getBounds().getHeight();
    if (bounds.getAspectRatio() < 2)
    {
        imageBounds.setHeight(imageBounds.getWidth());
        textBounds.setTop(imageBounds.getBottom());
        if (textBounds.getHeight() < minTextHeight + 2)
        {
            textBounds.setTop(bounds.getBottom()-(minTextHeight + 2));
            imageBounds.setBottom(textBounds.getY());
        }
    }
    else
    {
        int textLeft = bounds.getX() + bounds.getHeight();
        textBounds.setLeft(textLeft);
        imageBounds.setRight(textLeft);
    }
    int textHeight = config.getFontHeight(textBounds.toNearestInt(),
            getMenuItem()->getAppName());
    jassert(textHeight <= textBounds.getHeight());
    titleFont.setHeight(std::min(textHeight, minTextHeight));
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
