/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "AppMenu.h"
#include "WindowFocus.h"
#include "MainComponent.h"
#include "ContextMenu.h"
#include "DesktopEntryLoader.h"
#include "AppLauncher.h"
#include "MenuItem.h"
#include "JuceHeader.h"

/**
 * @file  Controller.h
 *
 * @brief  Creates the main menu component and controls its behavior.
 *
 * The controller is responsible for creating and managing the MenuComponent,
 * handling user input events passed to it by the MenuComponent, managing the
 * ContextMenu, holding any menu editor components, handling window focus
 * change events, and launching applications from the menu.
 */
class AppMenu::Controller : public MainComponent::MenuController,
private WindowFocus::Listener
{
public:
    Controller();

    /**
     * @brief  Cancels any pending callback functions.
     */
    virtual ~Controller();


    /**
     * @brief  Defines the interface provided by AppMenu::MenuComponent to the
     *         menu controller.
     */
    class ControlledMenu : public juce::Component
    {
    public:
        ControlledMenu() { }

        virtual ~ControlledMenu() { }

        /**
         * @brief  Gets the current active folder item.
         *
         * @return  The current active folder item.  This should never be null,
         *          as the root folder should always stay open.
         */
        virtual MenuItem getActiveFolder() = 0;

        /**
         * @brief  Gets the number of open folders held by the MenuComponent.
         *
         * @return  The number of open folder components.
         */
        virtual int openFolderCount() = 0;

        /**
         * @brief  Opens a folder as a FolderComponent, making it the active
         *         folder.
         *
         * @param folderItem  A folder menu item to open.
         */
        virtual void openFolder(const MenuItem folderItem) = 0;

        /**
         * @brief  Closes the current active FolderComponent as long as more
         *         than one folder is open.
         */
        virtual void closeActiveFolder() = 0;
    };

    /**
     * @brief  Defines the interface between the controller and 
     *         AppMenu::InputHandler.
     */
    class InputListener : public juce::KeyListener, public juce::MouseListener
    {
    public:
        /**
         * @brief  Creates a new InputListener, connecting it to the controller
         *         and setting it to listen to the controller's menu component.
         *
         * @param controller     The controller that will receive input signals
         *                       from this InputListener.
         *
         * @param menuComponent  The controller's AppMenu component.
         */
        InputListener(Controller& controller);

        virtual ~InputListener() { }

    protected:
        /**
         * @brief  Signals to the controller that a menu item was clicked.
         *
         * @param clickedItem   The menu item that was clicked.
         */
        void signalItemClicked(const MenuItem clickedItem);

        /**
         * @brief  Signals to the controller that a folder item was clicked.
         *
         * @param clickedFolder  The folder item that was clicked.
         */
        void signalFolderClicked(const MenuItem clickedFolder);

        /**
         * @brief  Signals to the controller that the user requested a generic
         *         context menu.
         */
        void signalContextMenuRequested();
        
        /**
         * @brief  Signals to the controller that the user requested a context
         *         menu for an open folder.
         *
         * @param folderItem    The relevant folder menu item.
         *
         * @param closestIndex  The index in the menu where new items created
         *                      through the context menu should be inserted.
         */
        void signalContextMenuRequested(const MenuItem folderItem,
                const int closestIndex);
        
        /**
         * @brief  Signals to the controller that the user requested a context
         *         menu for a specific menu item.
         *
         * @param MenuItem  The menuItem that the context menu should affect. 
         */
        void signalContextMenuRequested(const MenuItem menuItem);

        /**
         * @brief  Checks it user input should be enabled.
         * 
         * Input is enabled unless it has been disabled using setInputEnabled()
         * or a menu editor component is visible..
         *
         * @return   Whether user input should be handled or ignored.
         */
        bool getInputEnabled() const;

    private:
        /* Only the Controller may access the InputListener's private methods.*/
        friend class Controller;

        /**
         * @brief  Sets if the InputListener should register user input, or
         *         ignore it.
         *
         * @param allowUserInput  Whether user mouse and key events should 
         *                        be handled or ignored.
         */
        void setInputEnabled(const bool allowUserInput);

        /* Tracks whether user input should be handled or ignored. */
        bool inputEnabled = true;

        /* The Controller managing this ControlledMenu. */
        Controller& controller;
    };

protected:
    /**
     * @brief  Launches or focuses an application from the menu.
     *
     * @param toLaunch  A menu item holding an application launch command.
     */
    void launchOrFocusApplication(MenuItem toLaunch);

    /**
     * @brief  Displays a context menu with generic options for editing the
     *         AppMenu.
     */
    void showContextMenu();

    /**
     * @brief  Displays a context menu with options for editing an open menu
     *         folder.
     *
     * @param folderItem    The folder menu item the context menu should edit.
     *
     * @param closestIndex  The index in the menu where new items created
     *                      through the context menu should be inserted.
     */
    void showContextMenu(MenuItem folderItem, const int closestIndex);

    /**
     * @brief  Displays a context menu with options for editing a single menu
     *         item.
     *
     * @param menuItem  The menu item the context menu should edit.
     */
    void showContextMenu(MenuItem menuItem);
     
private:
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
    
    /**
     * @brief  Gets the main menu component, creating and saving it first if
     *         necessary,
     *
     * This will only be used to add and show the menu component as a child
     * component of the MainComponent.
     *
     * @return  A Component pointer to the main menu component.
     */
    virtual juce::Component* initMenuComponent() final override;
        
    /**
     * @brief  Updates the ControlledMenu whenever the parent MainComponent 
     *         is resized.
     *
     * @param parentBounds  The new bounds of the parent MainComponent.
     */
    virtual void parentResized(const juce::Rectangle<int> parentBounds) 
        override;

    /**
     * @brief  Creates the menuComponent as a subclass of AppMenu::MenuComponent
     *         appropriate for the current AppMenu format.
     *
     * @return   The new ControlledMenu object.
     */
    virtual ControlledMenu* createMenuComponent() = 0;

    /**
     * @brief  Creates the inputListener as a subclass of AppMenu::InputHandler
     *         appropriate for the current AppMenu format.
     *
     * @return  The new InputListener object. 
     */
    virtual InputListener* createInputHandler() = 0;

    /* The main menu component: */
    std::unique_ptr<ControlledMenu> menuComponent = nullptr;

    /* The object listening for user input: */
    std::unique_ptr<InputListener> inputListener = nullptr;

    /* Holds any popup menu editor component: */
    std::unique_ptr<PopupEditor> menuEditor = nullptr;

    /* Launches applications from the menu */
    AppLauncher appLauncher;
    
    /* ID value for any pending callback function held by DesktopEntryLoader: */
    juce::Atomic<DesktopEntryLoader::CallbackID> loaderCallbackID;

    /**
     * @brief  A private ContextMenu implementation that can edit the AppMenu
     *         through the Controller.
     */
    class ContextMenu : public AppMenu::ContextMenu
    {
    public:
        /**
         * @brief  Links the ContextMenu with the controller that holds it.
         *
         * @param controller  The menu controller object.
         */
        ContextMenu(Controller& controller);

        virtual ~ContextMenu() { }

    private:
        /**
         * @brief  Shows an editor component above the ControlledMenu.
         *
         * @param editor  The new editor to show.
         */
        virtual void showPopupEditor(PopupEditor* editor) override;

        /* A reference to the Controller that holds this ContextMenu. */
        Controller& menuController;
    };
    ContextMenu contextMenu;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
