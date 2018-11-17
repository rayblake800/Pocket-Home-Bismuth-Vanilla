#define APPMENU_IMPLEMENTATION_ONLY
#include <map>
#include "AppMenu/Data/JSON/MenuFile.h"
#include "DesktopEntry/Loader.h"
#include "AppMenu/Control/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu component's input
 * events.
 */
AppMenu::InputHandler::InputHandler
(MenuComponent* const menuComponent, Controller* const controller) :
     menuComponent(menuComponent), controller(controller)
{
    jassert(menuComponent != nullptr);
    jassert(controller != nullptr);
    menuComponent->setWantsKeyboardFocus(true);
    menuComponent->addKeyListener(this);
    menuComponent->addMouseListener(this, true);
    MenuFile appConfig;
    menuComponent->openFolder(appConfig.getRootFolderItem());
}

/*
 * Unsubscribes the InputHandler from menu input events before destruction.
 */ 
AppMenu::InputHandler::~InputHandler()
{
    menuComponent->removeKeyListener(this);
    menuComponent->removeMouseListener(this);
}

/*
 * Handles clicks to menu item buttons in the active folder.
 */
void AppMenu::InputHandler::menuItemClicked
(const MenuButton* clickedButton, const bool rightClicked)
{
    MenuItem buttonItem = clickedButton->getMenuItem();
    FolderComponent* parentFolder = static_cast<FolderComponent*>
        (clickedButton->getParentComponent());
    parentFolder->setSelectedIndex(buttonItem.getIndex());
    if(rightClicked)
    {
        controller->showContextMenu(buttonItem);
    }
    else
    {
        controller->activateMenuItem(buttonItem);
    }
}

/*
 * Handles clicks to menu folders, or menu item buttons in inactive folders.
 */
void AppMenu::InputHandler::folderClicked(const FolderComponent* clickedFolder, 
        const int closestIndex, const bool rightClicked)
{
    MenuItem folderItem = clickedFolder->getFolderMenuItem();
    while(folderItem != menuComponent->getActiveFolder()
            && menuComponent->openFolderCount() > 1)
    {
        menuComponent->closeActiveFolder();
    }
    if(rightClicked)
    {
        controller->showContextMenu(folderItem, closestIndex);
    }
}

/*
 * Gets the menu component tracked by this InputHandler.
 */
AppMenu::MenuComponent* AppMenu::InputHandler::getMenuComponent()
{
    return menuComponent;
}

/*
 * Gets the menu controller used by this InputHandler.
 */
AppMenu::Controller* AppMenu::InputHandler::getController()
{
    return controller;
}

/*
 * Convert generic mouse events into calls to InputHandler clicked methods.
 */
void AppMenu::InputHandler::mouseDown(const juce::MouseEvent& event) 
{
    // TODO: Don't just assume ctrl+click is equivalent to right click, define
    //       it in the input settings.
    const bool rightClicked = event.mods.isRightButtonDown()
        || event.mods.isPopupMenu()
        || event.mods.isCtrlDown();
    AppMenu::FolderComponent* clickedFolder = nullptr;
    if((MenuComponent*) event.eventComponent == menuComponent)
    {
        const int activeFolderIndex = menuComponent->openFolderCount() - 1;
        if(activeFolderIndex >= 0)
        {
            clickedFolder = menuComponent->getOpenFolder(activeFolderIndex); 
        }
        else
        {
            // At least one folder should be open!
            jassertfalse;
            return;
        }
    }
    if(clickedFolder == nullptr)
    {
        MenuButton* clickedButton = dynamic_cast<MenuButton*>
            (event.eventComponent);
        if(clickedButton != nullptr)
        {
            clickedFolder = dynamic_cast<FolderComponent*>
                (clickedButton->getParentComponent());
            if(clickedFolder != nullptr 
                    && clickedFolder->getFolderMenuItem() 
                    == menuComponent->getActiveFolder())
            {
                menuItemClicked(clickedButton, rightClicked);
                return;
            }
        }
        if(clickedFolder == nullptr)
        {
            clickedFolder = dynamic_cast<FolderComponent*>
                (event.eventComponent);
        }
    }
    if(clickedFolder != nullptr)
    {
        juce::Point<int> clickPos = event.getPosition();
        int closestIndex = clickedFolder->closestIndex(clickPos.x, clickPos.y);
        folderClicked(clickedFolder, closestIndex, rightClicked);
    }
}

/*
 * Convert generic key events into calls to the InputHandler's protected 
 * keyPress method.
 */
bool AppMenu::InputHandler::keyPressed(const juce::KeyPress& keyPress,
        juce::Component* sourceComponent) 
{
    // TODO: don't hardcode in key bindings, read them from input settings.
    using juce::KeyPress;
    static const std::map<juce::String, KeyType> keyMap =
    {
        {"cursor up",     KeyType::Up},
        {"cursor down",   KeyType::Down},
        {"cursor left",   KeyType::Left},
        {"cursor right",  KeyType::Right},
        {"return",        KeyType::Select},
        {"escape",        KeyType::Cancel},
        {"tab",           KeyType::Tab},
        {"ctrl + E",      KeyType::Edit}
    };
    const juce::String keyString = keyPress.getTextDescription();
    if(keyMap.count(keyString))
    {
        return keyPressed(keyMap.at(keyString));
    }
    return false;
}

/*
 * Scans desktop entries for updates whenever window focus is gained.
 */
void AppMenu::InputHandler::windowFocusGained() 
{
    DesktopEntry::Loader entryLoader;
    entryLoader.scanForChanges();
}

/*
 * Hides the loading spinner and stops waiting for applications to launch when
 * window focus is lost.
 */
void AppMenu::InputHandler::windowFocusLost() 
{
    controller->setLoadingState(false);
}
