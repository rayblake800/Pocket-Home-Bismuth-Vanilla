#define APPMENU_IMPLEMENTATION
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/Formats/Paged/Initializer.h"
#include "AppMenu/Formats/Scrolling/Initializer.h"
#include "AppMenu/Components/MainComponent.h"
    
/*
 * Creates and initializes the application menu.
 */
AppMenu::MainComponent::MainComponent() : 
formatUpdater(this)
{
    addChildComponent(&loadingSpinner);
    loadingSpinner.setAlwaysOnTop(true);
    // Load and initialize selected menu format.
    formatUpdater.applySelectedFormat();
}

AppMenu::MainComponent::~MainComponent()
{
    destroyMenu();
}

/*
 * Initialize the menu as a new menu format, cleaning up any existing menu 
 * first.
 */
void AppMenu::MainComponent::loadMenuFormat(const Initializer* initializer)
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
 * @brief  Initializes the updater, tracking the format key and saving
 *         a pointer to the MainComponent it updates.
 *
 * @param mainComponent  The MainComponent that holds this updater.
 */
AppMenu::MainComponent::FormatUpdater::FormatUpdater
(MainComponent* mainComponent) : mainComponent(mainComponent)
{
    addTrackedKey(ConfigKeys::menuFormatKey);
}

/**
 * @brief  Applies the selected menu format to the updater's 
 *         MainComponent.
 */
void AppMenu::MainComponent::FormatUpdater::applySelectedFormat()
{
    ConfigFile formatConfig;
    const Format newFormat = formatConfig.getMenuFormat();
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
            << formatConfig.formatToString(newFormat));
    mainComponent->loadMenuFormat(initializer.get());
}


/*
 * Updates the menu when the selected format changes.
 */
void AppMenu::MainComponent::FormatUpdater::configValueChanged
(const juce::Identifier& propertyKey)
{
    jassert(propertyKey == ConfigKeys::menuFormatKey);
    applySelectedFormat();
}
