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
 AppMenu::Controller::ControlledMenu::ControlledMenu(Controller& controller) :
     controller(controller) { }

/*
 * Signals to the controller that a menu item was clicked.
 */
void AppMenu::Controller::ControlledMenu::signalItemClicked
(MenuItem clickedItem, const bool rightClicked)
{
    controller.menuItemClicked(clickedItem, rightClicked);
}

/*
 * Signals to the controller that an open folder was clicked.
 */
void AppMenu::Controller::ControlledMenu::signalFolderClicked
(MenuItem folderItem, const bool rightClicked, const int closestIndex)
{
    controller.folderClicked(folderItem, rightClicked, closestIndex);
}

/*
 * Signals to the controller that the menu was clicked somewhere other than at a
 * folder or menu item.
 */
void AppMenu::Controller::ControlledMenu::signalMenuClicked
(const bool rightClicked)
{
    controller.menuClicked(rightClicked);
}

/*
 * Checks if user input should be enabled.
 */
bool AppMenu::Controller::ControlledMenu::getInputEnabled() const
{
    return inputEnabled;
}

/*
 * Sets if the ControlledMenu and all its child Components should register user 
 * input.
 */
void AppMenu::Controller::ControlledMenu::setInputEnabled
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
    menuComponent->setInputEnabled(false);
    appLauncher.startOrFocusApp(toLaunch.getCommand());
}

/*
 * Handles mouse clicks on menu items.
 */
void AppMenu::Controller::menuItemClicked
(MenuItem clickedItem, const bool rightClicked)
{
    CHECK_MENU_COMPONENT(__func__)
    if(menuEditor != nullptr && menuEditor->isShowing())
    {
        DBG("AppMenu::Controller::" << __func__ 
                << ": Ignoring menu item click, editor is open.");
        return;
    }
    if(rightClicked)
    {
        contextMenu.setMenuItemOptions(clickedItem);
        contextMenu.showAndHandleSelection();
    }
    else
    {
        if(clickedItem.isFolder())
        {
            menuComponent->openFolder(clickedItem);
        }
        else
        {
            launchOrFocusApplication(clickedItem);
        }
    }
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
