#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/Formats/Scrolling/Initializer.h"
#include "AppMenu.h"

static AppMenu::Initializer* createInitializer(const AppMenu::Format menuFormat)
{
    return new AppMenu::Scrolling::Initializer();
}

/*
 * Creates an AppMenu::MainComponent with a specific initial format.
 */
juce::Component* AppMenu::createAppMenu(const Format menuFormat)
{
    std::unique_ptr<Initializer> initializer(createInitializer(menuFormat));
    if(initializer == nullptr)
    {
        return nullptr;
    }
    return new MainComponent(initializer.get());
}

/*
 * Changes the format of an existing AppMenu::MainComponent.
 */
void AppMenu::changeMenuFormat
(juce::Component* appMenu, const Format newFormat)
{
    MainComponent* mainComponent = dynamic_cast<MainComponent*>(appMenu);
    if(mainComponent == nullptr)
    {
        DBG("AppMenu::" << __func__ 
                << ": Existing menu was null or not an appMenu!");
        jassertfalse;
    }
    std::unique_ptr<Initializer> initializer(createInitializer(newFormat));
    if(initializer != nullptr)
    {
        mainComponent->initMenu(initializer.get());
    }
}
