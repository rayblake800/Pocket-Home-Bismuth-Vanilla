#define APPMENU_IMPLEMENTATION_ONLY
#include "MenuComponent.h"

/* Key codes: */
//TODO: load these from config, set on input page
static const juce::String openPopupMenuBinding = "CTRL + e";
static const juce::String reloadMenuBinding = "TAB";

/*
 * Creates the MenuComponent, linking it with its controller.
 */
 AppMenu::MenuComponent::MenuComponent(Controller& controller) :
     ControlledMenu(controller) { }

/*
 * Gets the MenuItem for the current active folder component.
 */
AppMenu::MenuItem AppMenu::MenuComponent::getActiveFolder()
{
    jassert(!openFolders.isEmpty());
    return openFolders.getLast()->getFolderItem();
}

/*
 * Creates a new folder component.
 */
 AppMenu::MenuComponent::Folder::Folder
(MenuItem folderItem, MenuComponent& menuComponent) : 
    menuComponent(menuComponent), folderItem(folderItem) { }

/*
 * Gets the MenuItem that defines this folder.
 */
AppMenu::MenuItem AppMenu::MenuComponent::Folder::getFolderItem() const
{
    return folderItem;
}

/*
 * Signals to this Folder's MenuComponent that a menu item was clicked.
 */
void AppMenu::MenuComponent::Folder::signalItemClicked
(MenuItem clickedItem, const bool rightClicked)
{
    menuComponent.signalItemClicked(clickedItem, rightClicked);
}

/*
 * Signals to this Folder's MenuComponent that the folder was clicked.
 */
void AppMenu::MenuComponent::Folder::signalFolderClicked
(const bool rightClicked, const int closestIndex)
{
    menuComponent.signalFolderClicked(folderItem, rightClicked, closestIndex);
}

/*
 * Gets the number of open folder components.
 */
int AppMenu::MenuComponent::openFolderCount() const
{
    return openFolders.size();
}

/*
 * Gets an open folder component from the list of open folders.
 */
AppMenu::MenuComponent::Folder* 
AppMenu::MenuComponent::getOpenFolder(const int folderIndex) const
{
    if(folderIndex < 0 || folderIndex >= openFolders.size())
    {
        return nullptr;
    }
    return openFolders[folderIndex];
}

/*
 * Creates, shows, and activates a new folder component.
 */
void AppMenu::MenuComponent::openFolder(MenuItem folderItem)
{
    openFolders.add(createFolderComponent(folderItem));
    addAndMakeVisible(openFolders.getLast());
    updateMenuLayout();
}

/*
 * Closes the current active folder as long as more than one folder is open. 
 */
void AppMenu::MenuComponent::closeActiveFolder()
{
    if(openFolders.size() > 1)
    {
        removeChildComponent(openFolders.getLast());
        openFolders.removeLast();
        updateMenuLayout();
    }
}

/*
 * Handles menu navigation key events not handled by the folder component.
 */
bool AppMenu::MenuComponent::keyPressed(const juce::KeyPress& key)
{
    //TODO: redefine standard menu key shortcuts
    if(key == juce::KeyPress::createFromDescription(openPopupMenuBinding))
    {
        signalMenuClicked(true);
        return true;
    }
    return false;
} 

void AppMenu::MenuComponent::mouseDown(const juce::MouseEvent& event)
{
    if(event.eventComponent == this && event.mouseWasClicked())
    {
        signalMenuClicked(event.mods.isPopupMenu()
                || event.mods.isRightButtonDown()
                || event.mods.isCtrlDown());
    }
}


/*
 * Updates the menu layout when the component is resized.
 */
void AppMenu::MenuComponent::resized()
{
    updateMenuLayout();
}
