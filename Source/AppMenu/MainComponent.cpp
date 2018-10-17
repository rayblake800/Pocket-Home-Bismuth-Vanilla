#include "MainComponent.h"
#include "Utils.h"

    
/*
 * Initializes the menu controller, and adds and shows its main menu component 
 * as a child under the loading spinner.
 */
AppMenu::MainComponent::MainComponent(Controller* menuController)
{
    initMenu(menuController);
}

/*
 * Initializes or recreates the menu with a new menu controller.
 */
void AppMenu::MainComponent::initMenu(Controller* newController)
{
    if(newController == nullptr)
    {
        DBG("AppMenu::MainComponent::" << __func__ 
                << ": Error: new controller is null!");
        jassertfalse;
        return;
    }
    if(menuController != nullptr)
    {
        removeAllChildren();
    }
    menuController.reset(newController);
    menuController->connectLoadingSpinner(&loadingSpinner);
    addAndMakeVisible(menuController->initMenuComponent());
    addAndMakeVisible(loadingSpinner);
}


/*
 * Gets the current menu type.
 */
AppMenu::Format AppMenu::MainComponent::getMenuFormat() const
{
    if(menuController == nullptr)
    {
        return Format::Invalid;
    }
    return menuController->getMenuFormat();
}

/*
 * Links the MainComponent's loading spinner to the Controller so it can show or
 * hide the spinner.
 */
void AppMenu::MainComponent::Controller::connectLoadingSpinner
(OverlaySpinner* spinner)
{
    loadingSpinner = spinner;
}

/*
 * Activates or deactivates the loading spinner component held by the 
 * Controller's MainComponent.
 */
void AppMenu::MainComponent::Controller::setLoadingSpinnerVisible
(const bool shouldShow)
{
    if(loadingSpinner != nullptr)
    {
        loadingSpinner->setVisible(shouldShow);
    }
}

/*
 * Notifies the Controller that its parent's bounds have changed whenever the 
 * MainComponent is moved or resized.
 */
void AppMenu::MainComponent::resized()
{
    if(menuController != nullptr)
    {
        menuController->parentResized(getLocalBounds());
    }
}
