#ifndef APPMENU_IMPLEMENTATION
    #error File included directly outside of AppMenu implementation.
#endif
#pragma once
/**
 * @file  AppMenu_InputHandler.h
 *
 * @brief  Captures key, mouse, and window focus input events, and defines how
 *         they are applied to the menu.
 */

#include "AppMenu_Controller.h"
#include "AppMenu_MenuComponent.h"
#include "Windows_FocusListener.h"
#include "JuceHeader.h"

namespace AppMenu
{
    class InputHandler;
    class FolderComponent;
}

/**
 * @brief  Captures key and mouse events for the MenuComponent and its
 *         FolderComponent and MenuButton child components.
 *
 *  These events are sorted based on their type and target, and used to call
 * various virtual methods that InputHandler subclasses may override.
 *
 *  A default implementation for handling mouse clicks is provided. This
 * activates selected menu items, selects un-selected menu items, sets clicked
 * folders as active, and calls for context menus to be created when menu items
 * or folders are right-clicked. InputHandler implementations can override these
 * methods to change how the menu behaves when folders or menu items are
 * clicked.
 *
 *  The InputHandler listens for key events created while the menu is visible,
 * identifies if the key events are relevant to the menu, categorizes them as a
 * specific KeyType, and passes them to the virtual keyPressed method. All key
 * event handling from this point must be implemented by an InputHandler
 * subclass defined by the active AppMenu::Format.
 *
 *  The InputHandler also registers when the application window gains and loses
 * focus. This is used to hide the loading spinner when the window loses focus,
 * on the assumption that whatever action it was waiting for was probably the
 * action that caused the window to lose focus.
 */
class AppMenu::InputHandler : public juce::MouseListener,
    public juce::KeyListener, public Windows::FocusListener
{
public:
    /**
     * @brief  Initializes the InputHandler, setting it to handle the menu
     *         component's input events.
     *
     * @param menuComponent  The menu component tracked by the InputHandler.
     *
     * @param controller     The controller used to control menu behavior.
     */
    InputHandler(MenuComponent* const menuComponent,
            Controller* const controller);

    /**
     * @brief  Unsubscribes the InputHandler from menu input events before
     *         destruction.
     */
    virtual ~InputHandler();

protected:
    /**
     * @brief  Describes all types of key events handled by the InputHandler.
     */
    enum class KeyType
    {
        Up,
        Down,
        Left,
        Right,
        Select,
        Cancel,
        Edit,
        Tab
    };

    /**
     * @brief  Handles a key event received from the menu.
     *
     * @param keyType  The type of key event that was detected.
     *
     * @return         True to indicate that the key event was handled, false
     *                 to pass it on to other active juce::KeyListener objects.
     */
    virtual bool keyPressed(const KeyType keyType) = 0;

    /**
     * @brief  Handles clicks to menu item buttons in the active folder.
     *
     * By default, this sets the button as the selected button in its folder,
     * activates the button if it is left clicked, and shows the context menu
     * if the button is right clicked.
     *
     * @param clickedButton  The button component that was clicked.
     *
     * @param rightClicked   Whether the button was right clicked.
     */
    virtual void menuItemClicked(const MenuButton* clickedButton,
            const bool rightClicked);

    /**
     * @brief  Handles clicks to menu folders, or menu buttons in inactive
     *         folders.
     *
     * By default, this closes any folders open above the clicked folder, and
     * shows the context menu if the folder was right clicked.
     *
     * @param clickedFolder  The folder component that was clicked.
     *
     * @param closestIndex   The closest menu item index to the clicked spot in
     *                       the folder.
     *
     * @param rightClicked   Whether the folder was right clicked.
     */
    virtual void folderClicked(const FolderComponent* clickedFolder,
            const int closestIndex, const bool rightClicked);

    /**
     * @brief  Gets the menu component tracked by this InputHandler.
     *
     * @return  The AppMenu's menu component.
     */
    MenuComponent* getMenuComponent();

    /**
     * @brief  Gets the menu controller used by this InputHandler.
     *
     * @return   The Controller used to control the AppMenu.
     */
    Controller* getController();

private:
    /**
     * @brief  Convert generic mouse events into calls to InputHandler clicked
     *         methods.
     *
     * @param event  A mouse click event over some child component of the
     *               MenuComponent.
     */
    virtual void mouseDown(const juce::MouseEvent& event) final override;

    /**
     * @brief  Convert generic key events into calls to the InputHandler's
     *         protected keyPress method.
     *
     * @param keyPress         The key or keys that were pressed.
     *
     * @param sourceComponent  The source of the key event. This should always
     *                         be the menu component.
     *
     * @return                 True to signal that the key event was handled,
     *                         false to allow other juce::KeyListener objects
     *                         to handle the event.
     */
    virtual bool keyPressed(const juce::KeyPress& keyPress,
            juce::Component* sourceComponent) final override;

    /**
     * @brief  Scans desktop entries for updates whenever window focus is
     *         gained.
     */
    virtual void windowFocusGained() final override;

    /**
     * @brief  Hides the loading spinner and stops waiting for applications to
     *         launch when window focus is lost.
     */
    virtual void windowFocusLost() final override;

    // The menu component that is the source of all key and mouse events.
    MenuComponent* const menuComponent;

    // Used by the InputHandler to control the menu's behavior.
    Controller* const controller;
};
