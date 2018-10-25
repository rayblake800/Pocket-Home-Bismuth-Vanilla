/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "JuceHeader.h"
#include "WindowFocus.h"
#include "AppMenu.h"
#include "MenuComponents/MenuButton.h"
#include "MenuComponents/FolderComponent.h"
#include "MenuComponents/MenuComponent.h"
#include "Control/Controller.h"


class AppMenu::InputHandler : public juce::MouseListener,
    public juce::KeyListener, private WindowFocus::Listener
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
     * @brief  Handles clicks to menu item buttons.
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
     * @brief  Handles clicks to menu folders.
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
     * @brief  Handles clicks elsewhere on the menu component.
     *
     * By default, this opens the context menu when the component is
     * right clicked.
     *
     * @param rightClicked  Whether the component was right clicked.
     */
    virtual void menuClicked(const bool rightClicked); 

    /**
     * @brief   Gets the menu component tracked by this InputHandler.
     *
     * @return  The AppMenu's menu component.
     */
    MenuComponent* getMenuComponent();

    /**
     * @brief   Gets the menu controller used by this InputHandler.
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
     * @param sourceComponent  The source of the key event.  This should always
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

    /* The menu component that is the source of all key and mouse events. */
    MenuComponent* const menuComponent;

    /* Used by the InputHandler to control the menu's behavior. */
    Controller* const controller;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
