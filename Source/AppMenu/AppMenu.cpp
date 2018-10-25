#define APPMENU_IMPLEMENTATION_ONLY

#include "AppMenu.h"
#include "MainComponent.h"
#include "JuceHeader.h"

static AppMenu::Initializer* createInitializer(const AppMenu::Format menuFormat)
{
    return nullptr;
}

/*
 * Creates an AppMenu::MainComponent with a specific initial format.
 */
AppMenu::MainComponent* AppMenu::createAppMenu(const Format menuFormat)
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
(MainComponent* appMenu, const Format newFormat)
{
    if(appMenu == nullptr)
    {
        DBG("AppMenu::" << __func__ << ": Existing menu was null!");
        jassertfalse;
    }
    std::unique_ptr<Initializer> initializer(createInitializer(newFormat));
    if(initializer != nullptr)
    {
        appMenu->initMenu(initializer.get());
    }
}
