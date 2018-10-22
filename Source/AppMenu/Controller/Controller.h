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
     * @brief  Creates and manages all menu folder components.
     */
    class ControlledMenu : public juce::Component
    {
    public:
        /**
         * @brief  Creates a ControlledMenu, linking it to a controller.
         *
         * @param controller  The controller that should receive user input 
         *                    events from the menu component.
         */
        ControlledMenu(Controller& controller);

        virtual ~ControlledMenu() { }

    protected:
        /**
         * @brief  Signals to the controller that a menu item was clicked.
         *
         * @param clickedItem   The menu item that was clicked.
         *
         * @param rightClicked  Whether the menu item was right-clicked.
         */
        void signalItemClicked(MenuItem clickedItem, const bool rightClicked);

        /**
         * @brief  Signals to the controller that an open folder was clicked.
         *
         * @param folderItem     The folder item that was clicked.
         *
         * @param rightClicked   Whether the folder was right-clicked.
         *
         * @param closestIndex   The closest menu index to the spot that was
         *                       clicked, or -1 to specify no particular index.
         */
        void signalFolderClicked(MenuItem folderItem, const bool rightClicked,
                const int closestIndex = -1);

        /**
         * @brief  Signals to the controller that the menu was clicked somewhere
         *         other than at a folder or menu item.
         *
         * @param rightClicked  Whether the menu was right-clicked.
         */
        void signalMenuClicked(const bool rightClicked);

        /**
         * @brief  Gets the current active folder item.
         *
         * @return  The current active folder item.  This should never be null,
         *          as the root folder should always stay open.
         */
        virtual MenuItem getActiveFolder() = 0;

        /**
         * @brief  Checks if user input should be enabled.
         *
         * @return  Whether the ControlledMenu should register key and mouse
         *          events.
         */
        bool getInputEnabled() const;

    private:
        // Only the Controller may access the ControlledMenu's private methods. 
        friend class Controller;

        /**
         * @brief  Sets if the ControlledMenu and all its child Components should
         *         register user input.
         *
         * @param allowUserInput  Whether user mouse and key events should be
         *                        handled or ignored.
         */
        void setInputEnabled(const bool allowUserInput);

        /**
         * @brief  Opens a folder as a FolderComponent, making it the active
         *         folder.
         *
         * @param folderItem  A folder menu item to open.
         */
        virtual void openFolder(MenuItem folderItem) = 0;

        /**
         * @brief  Closes the current active FolderComponent as long as more
         *         than one folder is open.
         */
        virtual void closeActiveFolder() = 0;

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
     * @brief  Handles mouse clicks on menu items.
     *
     * @param clickedItem   The menu item that was clicked.
     *
     * @param rightClicked  Whether the menu item was right-clicked.
     */
    virtual void menuItemClicked(MenuItem clickedItem, const bool rightClicked);

    /**
     * @brief  Handles mouse clicks on open menu folders.
     *
     * @param clickedFolder  The folder menu item that was clicked.
     *
     * @param rightClicked   Whether the folder was right-clicked.
     *
     * @param closestIndex   The closest folder index to the spot that was
     *                       clicked, or -1 to specify no particular index.
     */
    virtual void folderClicked(MenuItem clickedFolder, const bool rightClicked,
            const int closestIndex);

    /**
     * @brief  Handles any other clicks on the menu component.
     *
     * @param rightClicked  Whether the menu was right-clicked.
     */
    virtual void menuClicked(const bool rightClicked);
     
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
     *         necessary.
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
     * @brief  Creates the ControlledMenu as whatever MenuComponent subclass
     *         is appropriate for the current AppMenu format.
     *
     * @return   The new ControlledMenu object.
     */
    virtual ControlledMenu* createMenuComponent() = 0;

    /* The main menu component: */
    std::unique_ptr<ControlledMenu> menuComponent = nullptr;

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
