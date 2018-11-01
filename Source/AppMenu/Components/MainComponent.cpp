#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Settings.h"
#include "AppMenu/Data/ConfigKeys.h"
#include "AppMenu/Components/MainComponent.h"
    
/*
 * Creates and initializes the application menu.
 */
AppMenu::MainComponent::MainComponent(const Initializer* initializer)
{
    addTrackedKey(ConfigKeys::menuFormatKey);
    initMenu(initializer);
}

AppMenu::MainComponent::~MainComponent()
{
    destroyMenu();
}

/*
 * Initialize the menu as a new menu format, cleaning up any existing menu 
 * first.
 */
void AppMenu::MainComponent::initMenu(const Initializer* initializer)
{
    jassert(initializer != nullptr);
    if(initializer->getMenuFormat() == currentMenuFormat
            || initializer->getMenuFormat() == Format::Invalid)
    {
        DBG("AppMenu::MainComponent::" << __func__ << 
                ": Skipping menu init, new format is invalid or matches old "
                << "format.");
        return;
    }
    
    destroyMenu(); // Clean up old menu instances

    currentMenuFormat = initializer->getMenuFormat();
    menuComponent.reset(initializer->createMenuComponent());
    controller.reset(new Controller(menuComponent.get(), loadingSpinner));
    inputHandler.reset(initializer->createInputHandler
            (menuComponent.get(), controller.get()));
    addAndMakeVisible(menuComponent.get());
}

/*
 * Safely destroys all AppMenu objects held in the MainComponent.
 */
void AppMenu::MainComponent::destroyMenu()
{
    if(menuComponent != nullptr)
    {
        removeChildComponent(menuComponent.get());
    }
    inputHandler.reset();
    controller.reset();
    menuComponent.reset();
    currentMenuFormat = Format::Invalid;
}

/*
 * Notifies the MenuComponent that its parent's bounds have changed whenever the 
 * MainComponent is moved or resized.
 */
void AppMenu::MainComponent::resized()
{
    if(menuComponent != nullptr)
    {
        menuComponent->parentResized(getLocalBounds());
    }
}

/*
 * Updates the menu when the selected format changes.
 */
void AppMenu::MainComponent::configValueChanged
(const juce::Identifier& propertyKey)
{
    jassert(propertyKey == ConfigKeys::menuFormatKey);
    AppMenu::changeMenuFormat(this, Settings::getMenuFormat());
}

