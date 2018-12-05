#define APPMENU_IMPLEMENTATION
#include "ComponentConfigKeys.h"
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/Formats/Paged/FolderComponent.h"
#include "AppMenu/Formats/Paged/MenuComponent.h"

/* Animation duration in milliseconds: */
static const constexpr int animationMS = 300;

/*
 * Creates the menu component and initializes the navigation buttons.
 */
AppMenu::Paged::MenuComponent::MenuComponent() :
    ConfigurableComponent(ComponentConfigKeys::pagedAppMenuKey), 
    upButton(NavButton::WindowEdge::up),
    leftButton(NavButton::WindowEdge::left),
    rightButton(NavButton::WindowEdge::right)
{ 
    ConfigFile formatConfig;
    addTrackedKey(ConfigKeys::pagedMenuColumnsKey);
    addTrackedKey(ConfigKeys::pagedMenuRowsKey);
    addChildComponent(upButton);
    upButton.setAlwaysOnTop(true);
    addChildComponent(leftButton);
    leftButton.setAlwaysOnTop(true);
    addChildComponent(rightButton);
    rightButton.setAlwaysOnTop(true);
}
 
/*
 * Adds a listener object to the menu's navigation buttons. 
 */
void AppMenu::Paged::MenuComponent::addNavButtonListener
(juce::Button::Listener* listener)
{
    upButton.addListener(listener);
    leftButton.addListener(listener);
    rightButton.addListener(listener);
}

/*
 * Finds the initial bounds to apply to a newly created folder component.
 */
juce::Rectangle<int> AppMenu::Paged::MenuComponent::initialFolderBounds
(const int newFolderIndex) const
{    
    using juce::Rectangle;
    Rectangle<int> startingBounds = getFolderBounds(newFolderIndex, false);
    if(newFolderIndex > 0)
    {
        startingBounds.setY(startingBounds.getY() + startingBounds.getHeight());
    }
    return startingBounds;
}

/*
 * Prepares to update the folder layout, showing or hiding the navigation 
 * buttons based on the needs of the new layout.
 */
void AppMenu::Paged::MenuComponent::layoutUpdateStarting
(const bool closingFolder)
{
    FolderComponent* activeFolder 
        = static_cast<AppMenu::Paged::FolderComponent*>
        (getOpenFolder(openFolderCount() - (closingFolder ? 2 : 1)));
    const bool navLeftNeeded = (activeFolder->getCurrentFolderPage() > 0);
    const bool navRightNeeded = (activeFolder->getCurrentFolderPage()
            < activeFolder->getNumFolderPages() - 1);
    const bool navUpNeeded = (openFolderCount() > (closingFolder ? 2 : 1));
    
    leftButton.setVisible(navLeftNeeded);
    leftButton.setEnabled(navLeftNeeded);
    rightButton.setVisible(navRightNeeded);
    rightButton.setVisible(navRightNeeded);
    upButton.setVisible(navUpNeeded);
    upButton.setEnabled(navUpNeeded);
}

/*
 * Finds the bounds where a menu folder should be placed.
 */
juce::Rectangle<int> AppMenu::Paged::MenuComponent::getFolderBounds
(const int folderIndex, const bool closingFolder) const
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
    upButton.applyConfigBounds();
    leftButton.applyConfigBounds();
    rightButton.applyConfigBounds();
}

/*
 * Updates the menu layout when the paged menu's grid dimensions change.
 */
void AppMenu::Paged::MenuComponent::configValueChanged
(const juce::Identifier& propertyKey)
{
    updateMenuLayout(false);
}
