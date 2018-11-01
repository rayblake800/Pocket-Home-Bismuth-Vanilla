#define APPMENU_IMPLEMENTATION_ONLY
#include "ComponentConfigKeys.h"
#include "AppMenu/Data/ConfigKeys.h"
#include "AppMenu/Formats/Paged/FolderComponent.h"
#include "AppMenu/Formats/Paged/MenuComponent.h"

/* Animation duration in milliseconds: */
static const constexpr int animationMS = 300;

AppMenu::Paged::MenuComponent::MenuComponent() :
ConfigurableComponent(ComponentConfigKeys::pagedAppMenuKey) 
{ 
    addTrackedKey(ConfigKeys::pagedMenuColumnsKey);
    addTrackedKey(ConfigKeys::pagedMenuRowsKey);
}
 
/*
 * Finds the bounds where a menu folder should be placed.
 */
juce::Rectangle<int> AppMenu::Paged::MenuComponent::getFolderBounds
(const int folderIndex, const bool closingFolder)
{
    int openFolders = openFolderCount();
    if(closingFolder)
    {
        openFolders--;
    }
    FolderComponent* folder = static_cast<AppMenu::Paged::FolderComponent*>
        (getOpenFolder(folderIndex));
    const int folderX = folder->getCurrentFolderPage() * -getWidth(); 
    const int folderY = (folderIndex - openFolders + 1) * getHeight(); 
    const int width   = getWidth() * folder->getNumFolderPages();
    if(folder->getBounds().isEmpty() && folderIndex > 0)
    {
        // New folder opened, set initial bounds for nicer animation
        folder->setBounds(folderX, folderY - getHeight(), width, getHeight());
    }
    return juce::Rectangle<int>(folderX, folderY, width, getHeight());
}

/*
 * Gets the duration in milliseconds to animate folder transitions.
 */
int AppMenu::Paged::MenuComponent::getAnimationDuration() const
{
    return animationMS;
}

/*
 * Creates a new Paged::FolderComponent for an opened folder menu item.
 */
AppMenu::FolderComponent* 
AppMenu::Paged::MenuComponent::createFolderComponent(MenuItem folderItem) const 
{
    return new AppMenu::Paged::FolderComponent(folderItem);
}

/*
 * Updates the menu's bounds whenever its parent component is resized.
 */
void AppMenu::Paged::MenuComponent::parentResized
(const juce::Rectangle<int> parentBounds)
{
    applyConfigBounds();
}

/*
 * Updates the menu layout when the paged menu's grid dimensions change.
 */
void AppMenu::Paged::MenuComponent::configValueChanged
(const juce::Identifier& propertyKey)
{
    updateMenuLayout(false);
}
