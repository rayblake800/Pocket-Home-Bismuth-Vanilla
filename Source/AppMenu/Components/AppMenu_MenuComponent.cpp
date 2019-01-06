#define APPMENU_IMPLEMENTATION
#include "AppMenu_MenuComponent.h"
#include "Layout_Transition_Animator.h"
#include "Utils.h"

/*
 * Gets the MenuItem for the current active folder component.
 */
AppMenu::MenuItem AppMenu::MenuComponent::getActiveFolder() const
{
    jassert(!openFolders.isEmpty());
    return openFolders.getLast()->getFolderMenuItem();
}

/*
 * Creates, shows, and activates a new folder component.
 */
void AppMenu::MenuComponent::openFolder(MenuItem folderItem)
{
    FolderComponent* newFolder = createFolderComponent(folderItem);
    newFolder->initMenuItems();
    openFolders.add(newFolder);
    addAndMakeVisible(newFolder);
    newFolder->setBounds(initialFolderBounds(openFolders.size() - 1));
    newFolder->updateButtonLayout();
    updateMenuLayout();
}

/*
 * Gets the number of open folder components.
 */
int AppMenu::MenuComponent::openFolderCount() const
{
    return openFolders.size();
}

/*
 * Closes the current active folder as long as more than one folder is open. 
 */
void AppMenu::MenuComponent::closeActiveFolder()
{
    if(openFolders.size() > 1)
    {
        updateMenuLayout(true, true);
        removeChildComponent(openFolders.getLast());
        openFolders.removeLast();
    }
}

/*
 * Updates the positions and sizes of all open folder components.
 */
void AppMenu::MenuComponent::updateMenuLayout(const bool animate)
{
   updateMenuLayout(animate, false); 
}

/*
 * Gets an open folder component from the list of open folders.
 */
AppMenu::FolderComponent* 
AppMenu::MenuComponent::getOpenFolder(const int folderIndex) const
{
    if(folderIndex < 0 || folderIndex >= openFolders.size())
    {
        return nullptr;
    }
    return openFolders[folderIndex];
}

/*
 * Saves a new popup editor component, and adds it to the main menu component.
 */
void AppMenu::MenuComponent::saveAndShowEditor(PopupEditor* newEditor)
{
    // Any pre-existing editor should have been removed first.
    jassert(menuEditor == nullptr || !menuEditor->isShowing());
    menuEditor.reset(newEditor);
    addAndMakeVisible(newEditor);
    menuEditor->applyConfigBounds();
    //Center the menu editor within the menu
    menuEditor->centreWithSize(menuEditor->getWidth(), 
            menuEditor->getHeight());
}

/*
 * Checks if the menu is currently showing a menu editor component.
 */
bool AppMenu::MenuComponent::showingEditor() const
{
    return menuEditor != nullptr && menuEditor->isShowing();
}

/*
 * If a menu editor component exists, this will remove it from its parent 
 * component.
 */
void AppMenu::MenuComponent::removeEditor()
{
    if(menuEditor != nullptr)
    {
        removeChildComponent(menuEditor.get());
    }
}

/*
 * Updates the positions and sizes of all open folder components.
 */
void AppMenu::MenuComponent::updateMenuLayout
(const bool animate, const bool closingFolder)
{
    using juce::Rectangle;
    layoutUpdateStarting(closingFolder);
    for(int i = 0; i < openFolders.size(); i++)
    {
        Rectangle<int> newBounds = getFolderBounds(i, closingFolder);
        Component* folder = openFolders[i];
        if(newBounds == folder->getBounds())
        {
            // Bounds are unchanged, so no action is required.
            continue;
        }
        if(animate)
        {
            Layout::Transition::Animator::transformBounds(
                    folder,
                    newBounds,
                    getAnimationDuration(), 
                    closingFolder && (i == openFolders.size() - 1));
        }
        else
        {
            folder->setBounds(newBounds);
        }
    }
}

/*
 * Updates the menu layout when the component is resized.
 */
void AppMenu::MenuComponent::resized()
{
    // Don't animate the change the first time the MenuComponent's bounds are
    // set.
    updateMenuLayout(!openFolders.getLast()->getBounds().isEmpty());
    for(int i = 0; i < openFolders.size(); i++)
    {
        openFolders[i]->updateButtonLayout();
    }
    if(showingEditor())
    {
        menuEditor->applyConfigBounds();
        //Center the menu editor within the menu
        menuEditor->centreWithSize(menuEditor->getWidth(), 
                menuEditor->getHeight());
    }
}
