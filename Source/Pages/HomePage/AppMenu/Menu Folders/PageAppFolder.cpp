#include "PageAppFolder.h"

PageAppFolder::PageAppFolder(AppMenuItem::Ptr folderItem,
        MouseListener* btnListener,
        std::map<String, AppMenuButton::Ptr>& buttonNameMap,
        IconThread& iconThread) :
AppMenuFolder(folderItem, btnListener, buttonNameMap, iconThread)
{
    reload();
}

PageAppFolder::~PageAppFolder() { }

/**
 * Create an AppMenuButton component for an AppMenuItem.
 */
AppMenuButton::Ptr PageAppFolder::createMenuButton
(AppMenuItem::Ptr menuItem)
{
    return new PageMenuButton(menuItem, iconThread,
            menuItem->getAppName() + String("Button"));
}

/**
 * Given a list of folder buttons, return an appropriate layout
 * for positioning them in the folder component.
 */
GridLayoutManager::Layout PageAppFolder::buildFolderLayout
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
int PageAppFolder::getNumFolderPages()
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
int PageAppFolder::getCurrentFolderPage()
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
bool PageAppFolder::setCurrentFolderPage(int pageNum)
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
int PageAppFolder::getSelectionPage()
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
int PageAppFolder::getSelectedIndexInFolderPage()
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
int PageAppFolder::getSelectionColumn()
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
int PageAppFolder::getSelectionRow()
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
int PageAppFolder::positionIndex(int page, int column, int row)
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
bool PageAppFolder::setSelectedPosition(int page, int column, int row)
{
    DBG(String("Setting selected page=") + String(page) + String(" column=") +
            String(column) + String(" row=") + String(row));
    return selectIndex(positionIndex(page, column, row))
            && setCurrentFolderPage(page);
}

/**
 * Resizes navigation buttons, then calls AppFolder::resized()
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
            setPadding(0.5 / (maxColumns * 20) * parentBounds.getWidth()
                    / bounds.getWidth(),
                    0.5 / (maxRows * 2) * parentBounds.getHeight()
                    / bounds.getHeight());
        }
    }
    AppMenuFolder::resized();
}

//############################  PageMenuButton  ################################

PageAppFolder::PageMenuButton::PageMenuButton
(AppMenuItem::Ptr menuItem, IconThread& iconThread, String name) :
AppMenuButton(menuItem, iconThread, name)
{

    setFillBackground(false);
    setDrawBorder(false);
    setTextJustification(Justification::centredTop);
}

PageAppFolder::PageMenuButton::~PageMenuButton() { }

void PageAppFolder::PageMenuButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    DBG(getName() + String(" bounds:") + bounds.toString());
    if (bounds.isEmpty())
    {
        return;
    }
    Rectangle<float> textBounds = bounds.toFloat();
    Rectangle<float> imageBounds = bounds.toFloat();
    const Font& titleFont = getTitleFont();
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
    
    if(textHeight != titleFont.getHeight())
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
