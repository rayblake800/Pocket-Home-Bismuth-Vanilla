#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "TempTimer.h"
#include "ComponentConfigKeys.h"
#include "TransitionAnimator.h"
#include "AppMenu/Settings.h"
#include "AppMenu/Data/ConfigKeys.h"
#include "AppMenu/Formats/Scrolling/FolderComponent.h"
#include "AppMenu/Formats/Scrolling/MenuComponent.h"

/* Animation duration in milliseconds: */
static const constexpr int animationMS = 300;

AppMenu::Scrolling::MenuComponent::MenuComponent() :
    ConfigurableComponent(ComponentConfigKeys::scrollingAppMenuKey),
    maxRows(Settings::getScrollingMenuRows())
{ 
    addTrackedKey(ConfigKeys::scrollingMenuRowsKey);
}

/*
 * Finds the bounds where a menu folder should be placed.
 */
juce::Rectangle<int> AppMenu::Scrolling::MenuComponent::getFolderBounds
(const int folderIndex, const bool closingFolder)
{
    using juce::Rectangle;
    const int buttonHeight = getHeight() / maxRows;
    const int centerX = getWidth() / 2;
    const int centerY = getHeight() / 2;
    const int openFolders = openFolderCount();
    const int lastIndex = openFolders - 1;

    // Update cached layout values before updating the first folder.
    if(folderIndex == 0)
    {
        int widthSum = 0;
        for(int i = 0; i < openFolders; i++)
        {
            int width = static_cast<FolderComponent*>(getOpenFolder(i))
                ->getMinimumWidth();
            folderWidths.set(i, width);
            if(!closingFolder || i < lastIndex)
            {
                widthSum += width;
            }
        }
        folderRightEdge = centerX + folderWidths[lastIndex] / 2;
        if(folderRightEdge < widthSum)
        {
            folderRightEdge = std::min(widthSum, getWidth());
        }
        if(closingFolder)
        {
            folderRightEdge += folderWidths[lastIndex] / 2;
            folderRightEdge += folderWidths[lastIndex - 1] / 2;
        }
    }

    int folderX = folderRightEdge;
    for(int i = lastIndex; i >= folderIndex; i--)
    {
        folderX -= folderWidths[i];
    }
    FolderComponent* folder = static_cast<FolderComponent*>
        (getOpenFolder(folderIndex));
    const int width = (folderIndex == lastIndex && closingFolder) ?
        0 : folder->getMinimumWidth();
    const int height = folder->getFolderSize() * buttonHeight;
    const int folderY = median<int>(
            buttonHeight,
            centerY - folder->getSelectedItemYOffset(),
            getHeight() - buttonHeight - height);
    if(folder->getBounds().isEmpty())
    {
        if(folderIndex > 0)
        {
            // New folder, set initial bounds for fancier animation
            int initialX = getOpenFolder(folderIndex-1)->getRight();
            folder->setBounds(initialX, folderY, 0, height);
        }
        else
        {
            // Root folder being initialized, don't animate
            folder->setBounds(folderX, folderY, width, height);
        }
    }
    return Rectangle<int>(folderX, folderY, width, height); 
}

/*
 * Gets the duration in milliseconds to animate folder transitions.
 */
int AppMenu::Scrolling::MenuComponent::getAnimationDuration() const
{
    return animationMS;
}

/*
 * Creates a new Scrolling::FolderComponent for an opened folder menu item.
 */
AppMenu::FolderComponent* 
AppMenu::Scrolling::MenuComponent::createFolderComponent
(MenuItem folderItem) const 
{
    return new Scrolling::FolderComponent(folderItem);
}

/*
 * Updates the menu's bounds whenever its parent component is resized.
 */
void AppMenu::Scrolling::MenuComponent::parentResized
(const juce::Rectangle<int> parentBounds)
{
    applyConfigBounds();
    updateMenuLayout();
}

/*
 * Updates the menu layout when the scrolling menu row count changes.
 */
void AppMenu::Scrolling::MenuComponent::configValueChanged
(const juce::Identifier& propertyKey)
{
    jassert(propertyKey == ConfigKeys::scrollingMenuRowsKey);
    maxRows = Settings::getScrollingMenuRows();
    updateMenuLayout(true);
}
