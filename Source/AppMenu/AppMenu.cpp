#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Implementation.h"
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/Data/JSON/MenuFile.h"
#include "AppMenu/Data/JSON/MenuKeys.h"
#include "AppMenu/AppMenu.h"

/*
 * Creates an AppMenu::MainComponent, initialized with the menu format saved 
 * through AppMenu::Settings.
 */
juce::Component* AppMenu::createAppMenu()
{
    return new MainComponent();
}
