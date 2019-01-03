#define APPMENU_IMPLEMENTATION
#include "AppMenu_Scrolling_FolderComponent.h"
#include "AppMenu_Scrolling_MenuButton.h"
#include "AppMenu_ConfigKeys.h"
#include "AppMenu_ConfigFile.h"
#include "Utils.h"

/*
 * Creates a new scrolling folder component.
 */
 AppMenu::Scrolling::FolderComponent::FolderComponent(MenuItem folderItem) :
     AppMenu::FolderComponent(folderItem) { }

/*
 * Finds the closest index to a spot on the folder that was clicked.
 */
int AppMenu::Scrolling::FolderComponent::closestIndex
(const int xPos, const int yPos) const 
{
    return getFolderMenuItem().getMovableChildCount();
}

/*
 * Creates a button component for one of the folder's child menu items.
 */
AppMenu::MenuButton* 
AppMenu::Scrolling::FolderComponent::createMenuButton
(const MenuItem folderItem) const 
{
    return new Scrolling::MenuButton(folderItem);
}

/*
 * Updates the sizes and positions of all buttons in the menu.
 */
void AppMenu::Scrolling::FolderComponent::updateButtonLayout()
{
    const juce::Rectangle<int> folderBounds = getLocalBounds(); 
    const int buttonHeight = folderBounds.getHeight() 
        / std::max(getFolderSize(), 1);
    const int buttonWidth = getMinimumWidth();
    int yPos = 0;
    for(int i = 0; i < getFolderSize(); i++)
    {
        getButtonComponent(i)->setBounds(0, yPos, buttonWidth, buttonHeight);
        yPos += buttonHeight;
    }
}

/*
 * Gets the distance between the top of the folder component and its selected 
 * menu item.
 */
int AppMenu::Scrolling::FolderComponent::getSelectedItemYOffset() const
{
    if(getSelectedIndex() <= 0 || getFolderSize() == 0)
    {
        return 0;
    }
    return getHeight() / getFolderSize() * getSelectedIndex();
}

/*
 * Gets the minimum width needed to fit this folder's menu items.
 */
int AppMenu::Scrolling::FolderComponent::getMinimumWidth() const
{
    ConfigFile appConfig;
    const int iconWidth = getWindowBounds().getHeight() /
        appConfig.getScrollingMenuRows();
    int maxTextWidth = 0;
    for(int i = 0; i < getFolderSize(); i++)
    {
        maxTextWidth = std::max(maxTextWidth, 
                getButtonComponent(i)->getTitleWidth());
    }
    return iconWidth + maxTextWidth;
}
