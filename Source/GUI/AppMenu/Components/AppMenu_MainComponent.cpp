#define APPMENU_IMPLEMENTATION
#include "AppMenu_MainComponent.h"
#include "AppMenu_ConfigKeys.h"
#include "AppMenu_Paged_Initializer.h"
#include "AppMenu_Scrolling_Initializer.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "AppMenu::MainComponent::";
#endif

// Creates and initializes the application menu.
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


// Initialize the menu as a new menu format, cleaning up any existing menu
// first.
void AppMenu::MainComponent::loadMenuFormat(const Initializer* initializer)
{
    jassert(initializer != nullptr);
    if (initializer->getMenuFormat() == currentMenuFormat
            || initializer->getMenuFormat() == Format::Invalid)
    {
        DBG(dbgPrefix << __func__ <<
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
    if (!getBounds().isEmpty())
    {
        menuComponent->parentResized(getLocalBounds());
    }
}


// Safely destroys all AppMenu objects held in the MainComponent.
void AppMenu::MainComponent::destroyMenu()
{
    if (menuComponent != nullptr)
    {
        removeChildComponent(menuComponent.get());
    }
    inputHandler.reset();
    controller.reset();
    menuComponent.reset();
    currentMenuFormat = Format::Invalid;
}


// Notifies the MenuComponent that its parent's bounds have changed whenever
// the MainComponent is moved or resized.
void AppMenu::MainComponent::resized()
{
    if (menuComponent != nullptr)
    {
        menuComponent->parentResized(getLocalBounds());
    }
    loadingSpinner.setBounds(getLocalBounds());
}


// Initializes the updater, tracking the format key and saving a pointer to the
// MainComponent it updates.
AppMenu::MainComponent::FormatUpdater::FormatUpdater
(MainComponent* mainComponent) : mainComponent(mainComponent)
{
    addTrackedKey(ConfigKeys::menuFormat);
}


// Applies the selected menu format to the updater's MainComponent.
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

    DBG(dbgPrefix << __func__ << ": Switching to Format::"
            << formatConfig.formatToString(newFormat));
    mainComponent->loadMenuFormat(initializer.get());
}


// Updates the menu when the selected format changes.
void AppMenu::MainComponent::FormatUpdater::configValueChanged
(const juce::Identifier& propertyKey)
{
    jassert(propertyKey == ConfigKeys::menuFormat);
    applySelectedFormat();
}
