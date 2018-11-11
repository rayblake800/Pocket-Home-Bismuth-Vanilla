#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Components/MainComponent.h"
#include "AppMenu/AppMenu.h"

/*
 * Creates an AppMenu::MainComponent, initialized with the menu format saved 
 * through AppMenu::Settings.
 */
juce::Component* AppMenu::createAppMenu()
{
    return new MainComponent();
}
