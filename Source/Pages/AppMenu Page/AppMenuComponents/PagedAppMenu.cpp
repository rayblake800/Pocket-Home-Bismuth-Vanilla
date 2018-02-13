#include "../../../Utils.h"
#include "PagedAppMenu.h"

PagedAppMenu::PagedAppMenu(AppConfigFile& appConfig) :
AppMenuComponent(ComponentConfigFile::pagedAppMenuKey, appConfig)
{
    setBounds(getWindowSize());
    loadButtons();
}

PagedAppMenu::~PagedAppMenu()
{
}

/**
 * Add a new application button to the active menu column.
 * @param appButton will be added to the bottom of the active menu column.
 */
void PagedAppMenu::addButtonComponent(AppMenuButton* appButton)
{
    PagedMenuButton* pageButton =
            dynamic_cast<PagedMenuButton*> (appButton);
    if (pageButton != nullptr)
    {

        addAndMakeVisible(pageButton);
        resized();
    }
}

/**
 * Scroll the menu so that the selected button is centered.
 * @param animatedScroll sets if the menu should animate its scrolling,
 * or just jump immediately to the destination position.
 */
void PagedAppMenu::scrollToSelected(bool animatedScroll)
{
    AppMenuButton* selectedButton =
            dynamic_cast<PagedMenuButton*> (getSelectedButton());
    if (selectedButton != nullptr)
    {
    }
}

/**
 * Receives all keyPress events and uses them for page navigation.
 * @param key
 * @return true iff the key press was used by the AppMenu
 */
bool PagedAppMenu::keyPressed(const KeyPress& key)
{
    int keyCode = key.getKeyCode();
    if (keyCode == KeyPress::tabKey)
    {
        DBG("pressed tab");
        loadButtons();
    }
    if (keyCode == KeyPress::upKey || keyCode == KeyPress::downKey)
    {
        changeSelection((keyCode == KeyPress::upKey) ? -1 : 1);
        return true;
    } else if (keyCode == KeyPress::leftKey || keyCode == KeyPress::escapeKey)
    {
        if (activeColumn() > 0)
        {
            closeFolder();
        }
        return true;
    } else if (keyCode == KeyPress::returnKey ||
            keyCode == KeyPress::spaceKey ||
            keyCode == KeyPress::rightKey)
    {
        DBG("AppMenuPage:click selected AppMenuButton");
        clickSelected();
        return true;
    } else if (key == KeyPress::createFromDescription("CTRL+e"))
    {
        openPopupMenu(true);
        return true;
    }
    return false;
}

/**
 * Create a new menu button component.
 * @param menuItem menu data to be held by the component
 */
AppMenuButton::Ptr PagedAppMenu::createMenuButton
(AppMenuItem* menuItem, int columnIndex, int rowIndex)
{
    return new PagedMenuButton(menuItem, iconThread, columnIndex, rowIndex,
            menuItem->getAppName() + String("Button"));
}

/**
 * Resize all child components.
 */
void PagedAppMenu::resized()
{
    DBG(String("New menu bounds:") + getBounds().toString());
    this->AppMenuComponent::resized();
    Rectangle<int> bounds = getLocalBounds();
    if (buttonColumns.size() == 0 || buttonColumns.size() > 2)
    {
        DBG(String("PagedAppMenu::resized(): invalid column count of ")
                + String(buttonColumns.size()));
        return;
    }
    //"Folder" column=0
    int btnHeight = bounds.getHeight() /
            (buttonColumns[0].size() + 1);
    int columnWidth = 0;
    for (AppMenuButton::Ptr button : buttonColumns[0])
    {
        int textHeight = btnHeight / 2;
        if (textHeight > 30)
        {
            textHeight = 30;
        }
        if (textHeight < 8)
        {
            textHeight = btnHeight - 2;
        }
        String btnText = button->getMenuItem()->getAppName();
        int buttonWidth = Font(textHeight).getStringWidth(btnText)
                + btnHeight;
        if (buttonWidth > columnWidth)
        {
            columnWidth = buttonWidth;
        }
    }
    for (int i = 0; i < buttonColumns[0].size(); i++)
    {
        AppMenuButton::Ptr button = buttonColumns[0][i];
        button->setBounds(0, i*btnHeight, columnWidth, btnHeight);
    }
    if (buttonColumns.size() > 1)
    {
        int xOrigin = 0;
        //As things are currently, this should always be true, but checking
        //is cheap enough that it's worth doing just in case I change
        //things later
        if (buttonColumns[0].size() > 0)
        {
            xOrigin = buttonColumns[0][0]->getRight();
        }
        int gridWidth = bounds.getWidth() - xOrigin;

        int numGridBtns = buttonColumns[1].size();
        int numGridColumns = (int) std::sqrt(numGridBtns *
                gridWidth/ bounds.getHeight());
        if(numGridColumns<1){
            numGridColumns=1;
        }
        int numGridRows=numGridBtns/numGridColumns;
        if(numGridRows*numGridColumns<numGridBtns){
            numGridRows++;
        }
        int btnSize = bounds.getHeight() / numGridRows;
        for (int i = 0; i < buttonColumns[1].size(); i++)
        {
            AppMenuButton::Ptr button = buttonColumns[1][i];
            int pageColumn = i % numGridColumns;
            int pageRow = i / numGridColumns;
            button->setBounds(xOrigin + btnSize*pageColumn, btnSize*pageRow,
                    btnSize, btnSize);
        }
    }
}

PagedAppMenu::PagedMenuButton::PagedMenuButton
(AppMenuItem* menuItem, IconThread& iconThread, int columnIndex,
        int rowIndex, String name) :
AppMenuButton(menuItem, iconThread, columnIndex, rowIndex, name)
{
}

PagedAppMenu::PagedMenuButton::~PagedMenuButton()
{
}

/**
 * Re-calculates draw values whenever the button is resized
 */
void PagedAppMenu::PagedMenuButton::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    textBox = bounds.toFloat();
    imageBox = bounds.toFloat();
    if (bounds.getAspectRatio() == 1)
    {
        int textBoxHeight = bounds.getHeight()/8;
        if(textBoxHeight<8){
            textBoxHeight=8;
        }
        int textTop = bounds.getBottom() - textBoxHeight;
        textBox.setTop(textTop);
        imageBox.setBottom(textTop);
        titleFont = fontResizedToFit
                (titleFont, getMenuItem()->getAppName(), textBox.toNearestInt());
        if(titleFont.getHeight()<8){
            titleFont.setHeight(8);
        }
    } else
    {
        int textLeft = bounds.getX() + bounds.getHeight();
        textBox.setLeft(textLeft);
        imageBox.setRight(textLeft);
        int textHeight = bounds.getHeight() / 2;
        if (textHeight > 30)
        {
            textHeight = 30;
        }
        if (textHeight < 8)
        {
            textHeight = bounds.getHeight() - 2;
        }
        titleFont.setHeight(textHeight);
    }
}


