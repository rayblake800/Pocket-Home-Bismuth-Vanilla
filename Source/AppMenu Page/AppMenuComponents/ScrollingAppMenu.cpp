#include "../../PocketHomeApplication.h"
#include "../AppMenuButton/MenuButton Types/ScrollingMenuButton.h"
#include "ScrollingAppMenu.h"

ScrollingAppMenu::ScrollingAppMenu(AppConfigFile& appConfig) :
AppMenuComponent(appConfig)
{
    x_origin = getBounds().getX();
    y_origin = getBounds().getY();


    loadButtons();
}

ScrollingAppMenu::~ScrollingAppMenu()
{
}

/**
 * Add a new application button to the active menu column.
 * @param appButton will be added to the bottom of the active menu column.
 */
void ScrollingAppMenu::addButtonComponent(AppMenuButton* appButton)
{
    ScrollingMenuButton* scrollButton =
            dynamic_cast<ScrollingMenuButton*> (appButton);
    if (scrollButton != nullptr)
    {
        scrollButton->applyConfigBounds();
        int buttonWidth = scrollButton->getWidth();
        int buttonHeight = scrollButton->getHeight();
        int columnIndex = scrollButton->getColumnIndex();
        int rowIndex = scrollButton->getRowIndex();
        if (rowIndex == 0 || columnIndex >= columnTops.size())
        {
            if (columnIndex == 0 || selected.size() < columnIndex
               || selected[columnIndex - 1]  == nullptr)
            {
                columnTops.push_back(y_origin);
            } else
            {
                columnTops.push_back(selected[columnIndex - 1]->getY());
            }
        }
        int x = columnIndex*buttonWidth;
        int y = columnTops[columnIndex] + buttonHeight * rowIndex;
        scrollButton->setBounds(x, y, buttonWidth, buttonHeight);
        addAndMakeVisible(appButton);
        if ((x + buttonWidth) > getWidth())
        {
            setBounds(getX(), getY(), x + buttonWidth, getHeight());
        }
        if ((y + buttonHeight) > getHeight())
        {
            setBounds(getX(), getY(), getWidth(), y + buttonHeight);
        }

    }
}

/**
 * Scroll the menu so that the selected button is centered.
 * @param animatedScroll sets if the menu should animate its scrolling,
 * or just jump immediately to the destination position.
 */
void ScrollingAppMenu::scrollToSelected(bool animatedScroll)
{
    AppMenuButton* selectedButton = getSelectedButton();
    if (selectedButton != nullptr)
    {
        int buttonWidth = selectedButton->getWidth();
        int buttonHeight = selectedButton->getHeight();
        int index = selectedButton->getRowIndex();
        int column = selectedButton->getColumnIndex();
        Rectangle<int> dest = getBounds();
        //calculate y-position
        if (selectedButton->isVisible())
        {
            int buttonYPos = selectedButton->getY();
            int screenHeight = getWindowSize().getHeight();

            int distanceFromCenter = abs(buttonYPos - getY()
                    + screenHeight / 2);
            //only scroll vertically if selected button is outside 
            //the center 3/5 
            if (distanceFromCenter > screenHeight / 5 * 3)
            {
                dest.setY(y_origin - buttonYPos + screenHeight / 2
                        - buttonHeight / 2);
            }
            if (dest.getY() > y_origin)
            {
                dest.setY(y_origin);
            }
        } else
        {
            dest.setY(y_origin - columnTops[column]);
        }
        //calculate x-position
        if (column == 0)
        {
            dest.setX(x_origin);
        } else
        {
            dest.setX(x_origin - column * buttonWidth + buttonHeight);
        }
        //scroll to the calculated position
        if (animatedScroll)
        {
            ComponentAnimator& animator = Desktop::getInstance().getAnimator();
            if (animator.isAnimating(this))
            {
                animator.cancelAnimation(this, false);
            }
            animator.animateComponent(this, dest, 1, 100, false, 1, 1);
        } else
        {
            setBounds(dest);
        }
    }
}

/**
 * Create a new menu button component.
 * @param menuItem menu data to be held by the component
 */
AppMenuButton::Ptr ScrollingAppMenu::createMenuButton
(AppMenuItem* menuItem, int columnIndex, int rowIndex)
{
    return new ScrollingMenuButton(menuItem, iconThread, columnIndex, rowIndex,
            menuItem->getAppName() + String("Button"));
}

/**
 * Resize all child components.
 */
void ScrollingAppMenu::resized()
{
    this->AppMenuComponent::resized();
    ComponentConfigFile& config = PocketHomeApplication::getInstance()
            ->getComponentConfig();
    ComponentConfigFile::ComponentSettings menuSettings =
            config.getComponentSettings(ComponentConfigFile::appMenuKey);
    Rectangle<int> menuBounds = menuSettings.getBounds();
    x_origin = menuBounds.getX();
    y_origin = menuBounds.getY();
    //resize all buttons
    Rectangle<int>buttonSize = buttonColumns[0][0]->getBounds().withZeroOrigin();
    int buttonWidth = buttonSize.getWidth();
    int buttonHeight = buttonSize.getHeight();
    int numColumns = selected.size();
    if (menuBounds.getWidth() < numColumns * buttonWidth)
    {
        menuBounds.setWidth(numColumns * buttonWidth);
    }
    for (int c = 0; c < numColumns; c++)
    {
        if (c > 0)
        {
            columnTops[c] = selected[c - 1]->getY();
        }
        int numRows = buttonColumns[c].size();
        if (menuBounds.getHeight() < numRows * buttonHeight + columnTops[c])
        {
            menuBounds.setHeight(numRows * buttonHeight + columnTops[c]);
        }
        for (int i = 0; i < numRows; i++)
        {
            AppMenuButton * button = buttonColumns[c][i];
            button->setBounds(buttonSize.withPosition(c*buttonWidth,
                    i * buttonHeight + columnTops[c]));
        }
    }
    setBounds(menuBounds);
    if (activeColumn() >= 0 && selected[activeColumn()] != nullptr
            && !Desktop::getInstance().getAnimator().isAnimating())
    {

        scrollToSelected();
    }
}
