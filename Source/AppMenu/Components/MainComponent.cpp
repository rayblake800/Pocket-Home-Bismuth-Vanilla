#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Settings.h"
#include "AppMenu/Data/ConfigKeys.h"
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/Formats/Paged/Initializer.h"
#include "AppMenu/Formats/Scrolling/Initializer.h"
    
/*
 * Creates and initializes the application menu.
 */
AppMenu::MainComponent::MainComponent()
{
    addTrackedKey(ConfigKeys::menuFormatKey);
    addChildComponent(&loadingSpinner);
    loadingSpinner.setAlwaysOnTop(true);
    // Load and initialize selected menu format.
    configValueChanged(ConfigKeys::menuFormatKey);
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
    if(!getBounds().isEmpty())
    {
        menuComponent->parentResized(getLocalBounds());
    }
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
    loadingSpinner.setBounds(getLocalBounds());
}

/*
 * Updates the menu when the selected format changes.
 */
void AppMenu::MainComponent::configValueChanged
(const juce::Identifier& propertyKey)
{
    jassert(propertyKey == ConfigKeys::menuFormatKey);
    const Format newFormat = Settings::getMenuFormat();
    std::unique_ptr<Initializer> initializer;
    switch(newFormat)
    
    {
        case AppMenu::Format::Scrolling:
            initializer.reset(new AppMenu::Scrolling::Initializer());
            break;
        case AppMenu::Format::Paged:
            initializer.reset(new AppMenu::Paged::Initializer());
            break;
        case AppMenu::Format::Invalid:
            initializer.reset(nullptr);
    }

    DBG("AppMenu::MainComponent::" << __func__
            << ": Switching to Format::" 
            << Settings::formatToString(newFormat));
    initMenu(initializer.get());
}

