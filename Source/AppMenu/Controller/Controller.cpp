#define APPMENU_IMPLEMENTATION_ONLY
#include "Controller.h"
#include "MenuItem.h"
#include "Config/MainFile.h"
#include "Config/MainKeys.h"

/**
 * @brief   Returns from the current function if the menu component is null.
 *          This will also print an error and fail an assertion if it is null 
 *          and debugging is enabled.
 *
 * @param functionName  The name of a function to print in the error message.
 */
#define CHECK_MENU_COMPONENT(functionName)          \
    if(menuComponent == nullptr)                    \
    {                                               \
        DBG("AppMenu::Controller::" << functionName \
                << ": Controller is null!");        \
        jassertfalse;                               \
        return;                                     \
    }

AppMenu::Controller::Controller() : contextMenu(*this) { }

/*
 * Cancels any pending callback functions.
 */
AppMenu::Controller::~Controller()
{
    DesktopEntryLoader entryLoader;
    entryLoader.clearCallback(loaderCallbackID.get());
}

/*
 * Creates a ControlledMenu, linking it to a controller.
 */
 AppMenu::Controller::InputListener::InputListener(Controller& controller) :
     controller(controller) 
{
    jassert(controller.menuComponent != nullptr);
    controller.menuComponent->addMouseListener(this, true);
    controller.menuComponent->addKeyListener(this);
}

/*
 * Signals to the controller that a menu item was clicked.
 */
void AppMenu::Controller::InputListener::signalItemClicked
(const MenuItem clickedItem)
{
    // Ignore clicks if a menu editor is visible or input is disabled.
    if(!getInputEnabled())
    {
        return;
    }
    // If the menu item is not in the active folder, close folders until it is.
    const MenuItem parentFolder = clickedItem.getParentFolder();
    if(parentFolder != controller.menuComponent->getActiveFolder())
    {
        while(parentFolder != controller.menuComponent->getActiveFolder()
            && controller.menuComponent->openFolderCount() > 1)
        {
            controller.menuComponent->closeActiveFolder();
        }
        jassert(parentFolder != controller.menuComponent->getActiveFolder());
    }
    // Otherwise, open folder items and launch/focus shortcut items.
    else
    {
        if(clickedItem.isFolder())
        {
            controller.menuComponent->openFolder(clickedItem);
        }
        else
        {
            controller.launchOrFocusApplication(clickedItem);
        }
    }
}


/*
 * Signals to the controller that a folder item was clicked.
 */
void AppMenu::Controller::InputListener::signalFolderClicked
(const MenuItem clickedFolder)
{
}

/*
 * Signals to the controller that the user requested a generic context menu.
 */
void AppMenu::Controller::InputListener::signalContextMenuRequested()
{
    if(getInputEnabled())
    {
        controller.contextMenu.setGenericOptions();
        controller.contextMenu.showAndHandleSelection();
    }
}

/*
 * Signals to the controller that the user requested a context menu for an open
 * folder.
 */
void AppMenu::Controller::InputListener::signalContextMenuRequested
(const MenuItem folderItem, const int closestIndex)
{
    if(getInputEnabled())
    {
        controller.contextMenu.setFolderOptions(folderItem, closestIndex);
        controller.contextMenu.showAndHandleSelection();
    }
}

/*
 * Signals to the controller that the user requested a context menu for a
 * specific menu item.
 */
void AppMenu::Controller::InputListener::signalContextMenuRequested
(const MenuItem menuItem)
{
    if(getInputEnabled())
    {
        controller.contextMenu.setMenuItemOptions(menuItem);
        controller.contextMenu.showAndHandleSelection();
    }
}

/*
 * Checks if user input should be enabled.
 */
bool AppMenu::Controller::InputListener::getInputEnabled() const
{
    return inputEnabled &&
        (controller.menuEditor == nullptr 
         || controller.menuEditor->isShowing());
}

/*
 * Sets if the ControlledMenu and all its child Components should register user 
 * input, or ignore it.
 */
void AppMenu::Controller::InputListener::setInputEnabled
(const bool allowUserInput)
{
    inputEnabled = allowUserInput;
}

/*
 * Launches or focuses an application from the menu.
 */
void AppMenu::Controller::launchOrFocusApplication(MenuItem toLaunch)
{
    CHECK_MENU_COMPONENT(__func__)
    using juce::String;
    String command = toLaunch.getCommand();
    if(toLaunch.getLaunchedInTerm())
    {
        Config::MainFile mainConfig;
        String termPrefix = mainConfig.getConfigValue<String>
            (Config::MainKeys::termLaunchCommandKey);
    }
    setLoadingSpinnerVisible(true);
    inputListener->setInputEnabled(false);
    appLauncher.startOrFocusApp(toLaunch.getCommand());
}

/*
 * Handles mouse clicks on open menu folders.
 */
void AppMenu::Controller::folderClicked
(MenuItem clickedFolder, const bool rightClicked, const int closestIndex)
{
    CHECK_MENU_COMPONENT(__func__)
    if(menuEditor != nullptr && menuEditor->isShowing())
    {
        DBG("AppMenu::Controller::" << __func__ 
                << ": Ignoring folder click, editor is open.");
        return;
    }
    if(rightClicked)
    {
        contextMenu.setFolderOptions(clickedFolder, closestIndex);
        contextMenu.showAndHandleSelection();
    }
    else 
    {
        MenuItem activeFolder = menuComponent->getActiveFolder();
        while(!(activeFolder == clickedFolder)
                && !activeFolder.getParentFolder().isNull())
        {
            menuComponent->closeActiveFolder();
            activeFolder = menuComponent->getActiveFolder();
        }
    }
}

/*
 * Handles any other clicks on the menu component.
 */
void AppMenu::Controller::menuClicked(const bool rightClicked)
{
    CHECK_MENU_COMPONENT(__func__)
    if(menuEditor != nullptr && menuEditor->isShowing())
    {
        DBG("AppMenu::Controller::" << __func__ 
                << ": Ignoring menu click, editor is open.");
        return;
    }
    if(rightClicked)
    {
        contextMenu.setGenericOptions();
        contextMenu.showAndHandleSelection();
    }
}

/*
 * Scans desktop entries for updates whenever window focus is gained.
 */
void AppMenu::Controller::windowFocusGained()
{
    // TODO: scan for changes 
}

/*
 * Hides the loading spinner and stops waiting for applications to launch when 
 * window focus is lost.
 */
void AppMenu::Controller::windowFocusLost()
{
    CHECK_MENU_COMPONENT(__func__)
    setLoadingSpinnerVisible(false); 
    menuComponent->setInputEnabled(true);
}

/*
 * Updates the ControlledMenu whenever the parent MainComponent is resized.
 */
void AppMenu::Controller::parentResized
(const juce::Rectangle<int> parentBounds) 
{
    CHECK_MENU_COMPONENT(__func__)
    menuComponent->setBounds(parentBounds);
}

/*
 * Links the ContextMenu with the controller that holds it.
 */
AppMenu::Controller::ContextMenu::ContextMenu(Controller& controller) :
menuController(controller) { } 

/*
 * Shows an editor component above the ControlledMenu.
 */
void AppMenu::Controller::ContextMenu::showPopupEditor(PopupEditor* editor)
{
   menuController.menuEditor.reset(editor); 
   if(menuController.menuComponent != nullptr)
   {
       menuController.menuComponent->addAndMakeVisible(editor);
   }
}
