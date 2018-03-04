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
	buttonsPerPage = maxRows*maxColumns;
	
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
	int currentPage = folder->getCurrentPage();
	int lastPageRow = maxRows;
	int lastPageColumn = maxColumns;
	if(currentPage == folder->getNumFolderPages()-1)
	{
		int folderIndex = size() % buttonsPerPage;
		lastPageRow = folderIndex / maxColumns;
		lastPageColumn = folderIndex % maxColumns
	}
	int newRow = 0;
	int newColumn = 0;
    if(selectedIndex == -1)
	{
		switch(key.getKeyCode()){
			case KeyPress::escapeKey:
				closeNonBaseFolder();
				return true;
			case KeyPress::leftKey:
			case KeyPress::upKey:
				newRow = lastPageRow;
				newColumn = lastPageColumn;
				break;
			case KeyPress::rightKey:
			case KeyPress::returnKey:
			case KeyPress::downKey:
				break;
			default:
				return false;
		}
		if(folder->setSelectedPosition(currentPage,newColumn,newRow))
		{
			folder->repaint;
		}
		return true;
	}
	else
	{
		int pageRow = folder->getSelectionRow();
		int pageColumn = folder->getSelectionColumn();
		int newPage = currentPage;
		switch(key.getKeyCode()){
			case KeyPress::escapeKey:
				folder->deselect();
				folder->repaint();
				return true;
			case KeyPress::returnKey:
				clickSelected();
				return true;
			case KeyPress::leftKey:
				pageColumn--;
				if(pageColumn < 0)
				{
					pageColumn = maxColumns;
					newPage--;
				}
			break;
			case KeyPress::rightKey:
				pageColumn++;
				if(pageColumn >= lastPageColumn)
				{
					pageColumn = 0;
					newPage++;
				}
			break;
			case KeyPress::upKey:
				if(pageRow > 0)
				{
					pageRow--;
				}
			break;
			case KeyPress::downKey:
				if(pageRow < lastPageRow-1)
				{
					pageRow++;
				}
			break;
			default:
				return false;
		}
		if(folder->setSelectedPosition(newPage,newColumn,newRow))
		{
			if(newPage != currentPage)
			{
				layoutFolders(true);
			}
			else
			{
				folder->repaint;
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
    int appsPerPage = maxRows * maxColumns;
    int leftEdge = 1;
    for (int i = openFolders.size() - 1; i >= 0; i--)
    {
        PageAppFolder* folder = static_cast<PageAppFolder*> (openFolders[i]);
		int folderPage = folder->getCurrentPage();
		int numFolderPages = folder->getNumFolderPages();
		
        Rectangle<int> bounds = getLocalBounds();
        bounds.setWidth(getWidth() * numFolderPages);
        if (leftEdge > 0)
        {
            bounds.setX(-getWidth() * folderPage);
        }
        else
        {
            bounds.setRight(leftEdge);
        }
        leftEdge = bounds.getX();
        folder->setMargin((float) pageLeft.getWidth()
                / (float) bounds.getWidth());
		if(folder->getBounds().isEmpty())
		{
			folder->setBounds(bounds.withX(getRight());
		}
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
            bool showLeft = folderPage > 0;
            bool showRight = folderPage < (numFolderPages-1);
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
 */
void PagedAppMenu::buttonClicked(Button* button)
{
	if(openFolders.isEmpty() || 
	!(button == &pageLeft || button == &pageRight))
	{
		return;
	}
	
    int targetPage = openFolders.getLast()->getCurrentFolderPage();
    if (button == &pageLeft)
    {
        targetPage--;
    }
    else if (button == &pageRight)
    {
        targetPage++;
    }
    if (openFolders.getLast()->setCurrentFolderPage(targetPage))
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
    maxRowNum = config.getConfigValue<int>(ComponentConfigFile::maxRowsKey,
    maxColumnNum = config.getConfigValue<int>(ComponentConfigFile::maxColumnsKey));
    buttonsPerPage = maxRowNum * maxColumnNum;
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
 */
GridLayoutManager::Layout PagedAppMenu::PageAppFolder::buildFolderLayout
(Array<AppMenuButton::Ptr>& buttons)
{
	if(buttons.isEmpty())
	{
		return {};
	}
    GridLayoutManager::Layout layout;
	int numPages = getNumFolderPages();
	int numSpacers = numPages - 1;
	int numAppColumns = maxColumnNum*numPages;
	int numPaddingGaps = numAppColumns - 1;
	
	int spacerSize = getMargin() * getWidth();
	int paddingSize = getXPadding() * getWidth();
	//the +2 to numSpacers is to account for the two margins
	int buttonSize = (getWidth() - (numSpacers+2) * spacerSize 
			- numPaddingGaps * paddingSize) / numAppColumns;
			
	int numLayoutColumns = numAppColumns+numSpacers;
	int numLayoutRows = maxRowNum;
	
	std::function<void(Component*,int)> addButton = 
	[&layout,buttonSize,spacerSize,this]
	(Component* component,int row)
	{	
        layout[row].compRow.push_back({component, buttonSize});
		if((layout[row].compRow.size()+1) % (maxColumnNum+1) == 0)
		{			
			layout[row].compRow.push_back({nullptr, spacerSize});
		}
	};
	
	//reserve space
    layout.reserve(sizeof (GridLayoutManager::RowLayoutParams)
			* numLayoutRows);
	for (int i = 0; i < numLayoutRows; i++)
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
        int pageIndex = i % btnPerPage;
        int row = pageIndex / numColumns;
		addButton(buttons[i],row);
    }
	
	//Fill in remaining empty spaces
    for (int row = 0; row < numLayoutRows; row++)
    {
        while (layout[row].compRow.size() < layoutRowSize)
        {
			addButton(nullptr,row);
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
	int numPages = size()/buttonsPerPage;
	if(size()% buttonsPerPage > 0)
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
	if(currentPage < 0 || currentPage >= getNumFolderPages())
	{
		currentPage = std::max(getSelectionPage(),0);
	}
	return currentPage;	
}


/**
 * Set which folder page should currently be visible.
 */
bool PagedAppMenu::PageAppFolder::setCurrentFolderPage(int pageNum)
{
	if(pageNum < 0 || pageNum >= getNumFolderPages()){
		return false;
	}
	currentPage = pageNum;
	return true;
}

/**
 * @return the folder page index containing the selected folder 
 * button, or -1 if there is no selection.
 */
int PagedAppMenu::PageAppFolder::getSelectionPage()
{
	int selected = getSelectedIndex();
	if(selected != -1)
	{	
		selected /= appsPerPage;
	}
	return selected;
}


/**
 * @return the index of the selected button within its folder page,
 * or -1 if there is no selection.
 */
int getSelectedIndexInFolderPage()
{
	int selected = getSelectedIndex();
	if(selected != -1)
	{
		selected %= appsPerPage;
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
	if(selected != -1)
	{
		selected %= maxColumnNum;
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
	if(selected != -1)
	{
		selected /= maxColumnNum;
	}
	return selected;
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
bool setSelectedPosition(int page,int column, int row){
	if(page < 0 || column < 0 || row < 0 ||
		page >= getNumFolderPages() 
		|| column >= maxColumnNum
		|| row >= maxRowNum)
	{
		return false;
	}
	int newIndex = page * maxColumnNum * maxRowNum
		+ row * maxColumnNum + column;
	return setSelectedPosition(newIndex) && setCurrentFolderPage(page);
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
