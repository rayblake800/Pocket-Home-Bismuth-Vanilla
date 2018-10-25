#define APPMENU_IMPLEMENTATION_ONLY
#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "ComponentConfigKeys.h"
#include "TransitionAnimator.h"
#include "AppMenu/MenuFormats/Scrolling/FolderComponent.h"
#include "AppMenu/MenuFormats/Scrolling/MenuComponent.h"

/* Animation duration in milliseconds: */
static const constexpr int animationMS = 300;

AppMenu::Scrolling::MenuComponent::MenuComponent() :
    ConfigurableComponent(ComponentConfigKeys::scrollingAppMenuKey) { }

/*
 * Arranges folders from left to right, with the selected item of the active 
 * folder centered.
 */
void AppMenu::Scrolling::MenuComponent::updateMenuLayout() 
{
    Config::MainFile mainConfig;
    const int buttonHeight = getHeight()
        / mainConfig.getConfigValue<int>
        (Config::MainKeys::scrollingMenuRowsKey);
    const int centerX = getWidth() / 2;
    const int centerY = getHeight() / 2;

    int folderX = 0;
    for(int i = openFolderCount() - 1; i >= 0; i--)
    {
        FolderComponent* folder 
            = static_cast<FolderComponent*>(getOpenFolder(i));
        const int width = folder->getMinimumWidth();
        const int height = folder->getFolderSize() * buttonHeight;
        if(i == openFolderCount() - 1)
        {
            folderX = centerX - (width / 2);
        }
        else
        {
            folderX -= width;
        }
        const int folderY = centerY - folder->getSelectedItemYOffset();
        if(folderX != folder->getX())
        {
            const juce::Rectangle<int>
                newBounds(folderX, folderY, width, height);
            TransitionAnimator::transformBounds(folder, newBounds, animationMS);
        }
        else
        {
            folder->setBounds(folderX, folderY, width, height);
        }
    }
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
}
