#define APPMENU_IMPLEMENTATION_ONLY
#include "Utils.h"
#include "AppMenu/Components/MenuComponent.h"

AppMenu::MenuComponent::MenuComponent()
{
    setWantsKeyboardFocus(true);
}

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
        removeChildComponent(openFolders.getLast());
        openFolders.removeLast();
        updateMenuLayout();
    }
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
 * Updates the menu layout when the component is resized.
 */
void AppMenu::MenuComponent::resized()
{
    updateMenuLayout();
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
