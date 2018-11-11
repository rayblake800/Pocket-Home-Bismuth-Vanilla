/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY

#pragma once
#include "JuceHeader.h"
#include "OverlaySpinner.h"
#include "AppLauncher.h"
#include "Locale/TextUser.h"
#include "AppMenu/AppMenu.h"
#include "AppMenu/Data/MenuItem.h"
#include "AppMenu/Data/DesktopEntryUpdater.h"
#include "AppMenu/Components/MenuComponent.h"
#include "AppMenu/Components/Editors/PopupEditor.h"

/**
 * @file  Controller.h
 *
 * @brief  Implements core menu functionality, opening and closing folders,
 *         creating menu editors, handling popup context menus, and launching 
 *         application shortcuts.
 *
 *  The controller is responsible for implementing all menu functionality not
 *  related to menu display or input handling.  The controller defines the
 *  behavior of activated menu items, launches applications, opens or closes
 *  menu folders, creates and handles popup context menus, and creates 
 *  appropriate popup editors when necessary.
 */
class AppMenu::Controller : public Locale::TextUser
{
public:
    /**
     * @brief  Creates a new menu controller.
     *
     * @param menuComponent   The main menu component controlled through the
     *                        Controller.
     *
     * @param loadingSpinner  The overlay spinner used to show to the user that
     *                        the Controller is working and input is disabled.
     */
    Controller(MenuComponent* menuComponent, OverlaySpinner& loadingSpinner);

    virtual ~Controller() { }

    /**
     * @brief  Displays a context menu with options for editing an open menu
     *         folder.
     *
     * @param folderItem    The folder menu item the context menu should edit.
     *
     * @param insertIndex   The index in the menu where new items created
     *                      through the context menu should be inserted.
     */
    void showContextMenu(const MenuItem folderItem, const int insertIndex);

    /**
     * @brief  Displays a context menu with options for editing a single menu
     *         item.
     *
     * @param menuItem  The menu item the context menu should edit.
     */
    void showContextMenu(const MenuItem menuItem);
     
    /**
     * @brief  Activates an item in the menu, launching application shortcut
     *         menu items, and opening folder menu items.
     *
     * @param clickedItem  The item in the menu that should be activated.
     */
    void activateMenuItem(const MenuItem clickedItem);
    
    /**
     * @brief  Sets if the Controller should show the loading spinner and ignore
     *         input.
     *
     * @param isLoading    Whether the Controller is currently busy and should 
     *                     ignore input.
     *
     * @param loadingText  An optional loading message to print on the loading
     *                     spinner.
     */
    void setLoadingState(const bool isLoading, 
            const juce::String loadingText = juce::String());

    /**
     * @brief  Checks if the controller is currently ignoring user input.
     *
     * @return  Whether the controller will ignore requests to show menus, show
     *          editors, and activate menu items.
     */
    bool ignoringInput() const;

private:
    /* Context menu option codes: */
    enum class OptionCode
    {
        Cancelled,
        Edit,
        Delete,
        NewShortcut,
        NewFolder,
        NewDesktopEntry,
        PinToRoot,
        MoveBack,
        MoveForward
    };

    /**
     * @brief  Performs the appropriate action for a selected context menu
     *         option.
     *
     * @param selectedOption  The option selected in the context menu.
     *
     * @param editedItem      A menu item that the action should be applied to.
     *
     * @param insertIndex     An insertion index where new items should be 
     *                        inserted into a folder targetItem.
     */
    void handleContextMenuAction(OptionCode selectedOption,
            MenuItem editedItem,
            const int insertIndex);
    /**
     * @brief  Launches or focuses an application from the menu.
     *
     * @param toLaunch  A menu item holding an application launch command.
     */
    void launchOrFocusApplication(const MenuItem toLaunch);

    /**
     * @brief  Opens a folder menu item as a new FolderComponent.
     *
     * @param folderItem   The folder item to set as the new active folder.
     */
    void openFolder(const MenuItem folderItem);

    /**
     * @brief  Creates and shows a new PopupEditor component that can create a 
     *         new application shortcut menu item.
     *
     * @param folder       The menu folder where the editor may insert a new 
     *                     item.
     *
     * @param insertIndex  The index in the folder where any new item would be
     *                     inserted.
     */
    void createNewShortcutEditor(const MenuItem folder, const int insertIndex);

     /**
     * @brief  Creates and shows a new PopupEditor component that can create a 
     *         new folder menu item.
     *
     * @param folder       The menu folder where the editor may insert a new 
     *                     item.
     *
     * @param insertIndex  The index in the folder where any new item would be
     *                     inserted.
     */
    void createNewFolderEditor(const MenuItem folder, const int insertIndex);

    /**
     * @brief  Creates and shows a new PopupEditor component that can create a 
     *         new desktop entry.
     *
     * @param categorySource  A MenuItem used to set the initial categories of
     *                        the desktop entry.
     */
    void createNewEntryEditor(const MenuItem categorySource);

    /**
     * @brief  Creates and shows a new PopupEditor component that edits an 
     *         existing item in the menu.
     *
     * @param toEdit  The menu item that will be edited.
     */
    void createExistingItemEditor(MenuItem toEdit);

    /**
     * @brief  Creates a copy of a menu item at a new index in the menu.
     *
     * @param toCopy       The menu item being copied.
     *
     * @param copyFolder   The folder where the copy will be inserted.
     *
     * @param insertIndex  The index in copyFolder where the copy will be 
     *                     inserted.
     */
    void copyMenuItem(const MenuItem toCopy, MenuItem copyFolder,
            const int insertIndex);

    /* Holds a reference to the loading spinner */
    OverlaySpinner& loadingSpinner;

    /* The main menu component */
    MenuComponent* const menuComponent;

    /* Launches or focuses applications from the menu */
    AppLauncher appLauncher;

    /* Adds and updates menu items created from DesktopEntry::EntryFiles. */
    DesktopEntryUpdater entryManager;
};

/* Only include this file directly in the AppMenu implementation! */
#endif
