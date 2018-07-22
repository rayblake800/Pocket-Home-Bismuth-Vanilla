#include "Utils.h"
#include "PageAppFolder.h"

PageAppFolder::PageAppFolder(
        AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap) :
AppMenuFolder(folderItem, btnListener, buttonNameMap)
{
    reload();
}

/**
 * Create an AppMenuButton component for an AppMenuItem.
 */
AppMenuButton::Ptr PageAppFolder::createMenuButton(AppMenuItem::Ptr menuItem)
{
    return new PageMenuButton(
            menuItem,
            menuItem->getAppName() + String("Button"));
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 */
LayoutManager::Layout PageAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
    if (buttons.isEmpty() || getBounds().isEmpty())
    {
        return {};
    }
    LayoutManager::Layout layout;
    int numPages = getNumFolderPages();
    int numSpacers = numPages - 1;
    int numAppColumns = getMaxColumns() * numPages;
    int numPaddingGaps = numAppColumns - 1;

    int paddingSize = getXPadding() * getWidth();
    int spacerSize = getMargin() * getWidth() * 2 + paddingSize;
    //the +2 to numSpacers is to account for the two margins
    int buttonSize = (getWidth() - (numSpacers + 2) * spacerSize
                      - numPaddingGaps * paddingSize) / numAppColumns;
    paddingSize /= 2;

    int numLayoutColumns = numAppColumns + numSpacers;
    
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    Array<Row> rows;
    for(int i = 0; i < getMaxRows(); i++)
    {
        rows.add(Row(10, {}));
    }

    std::function<void(Component*, int) > addButton =
            [&rows, buttonSize, spacerSize, paddingSize, numLayoutColumns, this]
            (Component* component, int row)
            {
                rows.getReference(row).addRowItem
                        (RowItem(component, buttonSize));
                int rowSize = rows[row].itemCount();
                if ((rowSize + 1) % (getMaxColumns() + 1) == 0
                    && rowSize < numLayoutColumns)
                {
                    rows.getReference(row).addRowItem(RowItem(spacerSize));
                }
            };

    //place buttons
    for (int i = 0; i < buttons.size(); i++)
    {
        int pageIndex = i % getButtonsPerPage();
        int row = pageIndex / getMaxColumns();
        addButton(buttons[i], row);
    }

    //Fill in remaining empty spaces
    for (int row = 0; row < getMaxRows(); row++)
    {
        while (rows[row].itemCount() < numLayoutColumns)
        {
            addButton(nullptr, row);
        }
        layout.addRow(rows[row]);
    }
    layout.setXPaddingWeight(paddingSize);
    layout.setYPaddingWeight(1);
    return layout;
}

/**
 * @return the number of pages this folder needs to display all menu
 *          buttons.
 */
int PageAppFolder::getNumFolderPages() const
{
    int numPages = getButtonCount() / getButtonsPerPage();
    if (getButtonCount() % getButtonsPerPage() > 0)
    {
        numPages++;
    }
    return numPages;
}

/**
 * @return the index of the page that's currently visible, or -1 if 
 *          there is no current page. 
 */
int PageAppFolder::getCurrentFolderPage() const
{
    return median<int>(0, currentPage, getNumFolderPages() - 1);
}

/**
 * Set which folder page should currently be visible.
 */
bool PageAppFolder::setCurrentFolderPage(int pageNum)
{
    if (pageNum < 0 || pageNum >= getNumFolderPages())
    {
        return false;
    }
    currentPage = pageNum;
    DBG("PageAppFolder::" << __func__ << ": Moved to page " << currentPage);
    return true;
}

/**
 * @return the folder page index containing the selected folder 
 *          button, or -1 if there is no selection.
 */
int PageAppFolder::getSelectionPage() const
{
    int selected = getSelectedIndex();
    if (selected != -1)
    {
        selected /= getButtonsPerPage();
    }
    return selected;
}

/**
 * @return the index of the selected button within its folder page,
 *          or -1 if there is no selection.
 */
int PageAppFolder::getSelectedIndexInFolderPage() const
{
    int selected = getSelectedIndex();
    if (selected != -1)
    {
        selected %= getButtonsPerPage();
    }
    return selected;
}

/**
 * @return the column index of the selected button within its
 *          folder page, or -1 if there is no selection.
 */
int PageAppFolder::getSelectionColumn() const
{
    int selected = getSelectedIndexInFolderPage();
    if (selected != -1)
    {
        selected %= getMaxColumns();
    }
    return selected;
}

/**
 * @return the row index of the selected button within its
 *          folder page, or -1 if there is no selection.
 */
int PageAppFolder::getSelectionRow() const
{
    int selected = getSelectedIndexInFolderPage();
    if (selected != -1)
    {
        selected /= getMaxColumns();
    }
    return selected;
}

/**
 * Finds what index value a button would have at a particular
 * position within the folder.
 */
int PageAppFolder::positionIndex(int page, int column, int row) const
{
    if (page < 0 || column < 0 || row < 0 ||
        page >= getNumFolderPages()
        || column >= getMaxColumns()
        || row >= getMaxRows())
    {
        return -1;
    }
    return page * getMaxColumns() * getMaxRows()
            + row * getMaxColumns() + column;
}

/**
 * Set the button selection based on its position in the
 * folder.  The selection will not change if there isn't
 * a button located at the given position.  If necessary,
 * the current folder page will change to the one containing
 * the new selection.
 */
bool PageAppFolder::setSelectedPosition(int page, int column, int row)
{
    //DBG("PageAppFolder::" << __func__ << ": Setting selected page=" << page
    //        << " column=" << column << " row=" << row);
    return selectIndex(positionIndex(page, column, row))
            && setCurrentFolderPage(page);
}

/**
 * Sets the margin width relative to the parent component width, rather than
 * relative to folder width.  The actual margin will be calculated using
 * this value whenever the folder's size changes.
 */
void PageAppFolder::setParentRelativeMargin(float margin)
{
    parentRelativeMargin = margin;
}

/**
 * Resizes padding, then calls AppFolder::resized()
 */
void PageAppFolder::resized()
{
    Component* parent = getParentComponent();
    if (parent != nullptr)
    {
        Rectangle<int> bounds = getLocalBounds();
        Rectangle<int> parentBounds = parent->getLocalBounds();
        if (bounds.getWidth() > 0)
        {
            setPadding(0.5 / (getMaxColumns() * 20) * parentBounds.getWidth()
                    / bounds.getWidth(),
                    0.5 / (getMaxRows() * 2) * parentBounds.getHeight()
                    / bounds.getHeight());
            if (parentRelativeMargin > 0)
            {
                setMargin(parentRelativeMargin * parentBounds.getWidth()
                        / bounds.getWidth());
            }
        }
    }
    layoutButtons();
    AppMenuFolder::resized();
}

//############################  PageMenuButton  ################################

PageAppFolder::PageMenuButton::PageMenuButton
(AppMenuItem::Ptr menuItem, String name) :
AppMenuButton(menuItem, name)
{
    setFillBackground(false);
    setDrawBorder(false);
    setTextJustification(Justification::centredTop);
}

void PageAppFolder::PageMenuButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    if (bounds.isEmpty())
    {
        return;
    }
    Rectangle<float> textBounds = bounds.toFloat();
    Rectangle<float> imageBounds = bounds.toFloat();
    const Font& titleFont = getTitleFont();
    ComponentConfigFile config;
    int textHeight = config.getFontHeight(ComponentConfigFile::smallText);

    imageBounds.setHeight(imageBounds.getWidth());
    textBounds.setTop(imageBounds.getBottom());
    if (textBounds.getHeight() < textHeight + 4)
    {
        textBounds.setTop(bounds.getBottom()-(textHeight + 4));
        imageBounds.setBottom(textBounds.getY());
    }

    if (textHeight != titleFont.getHeight())
    {
        setTitleFont(titleFont.withHeight(textHeight));
    }

    setTextBounds(textBounds);
    setImageBounds(imageBounds);
}

/**
 * Set the background and border to only draw for selected buttons.
 */
void PageAppFolder::PageMenuButton::selectionStateChanged()
{
    setFillBackground(isSelected());
    setDrawBorder(isSelected());
}
