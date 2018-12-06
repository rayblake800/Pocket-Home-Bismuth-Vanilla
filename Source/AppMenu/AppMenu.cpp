#define APPMENU_IMPLEMENTATION
#include "AppMenu/AppMenu.h"
#include "AppMenu/Components/MainComponent.h"

/*
 * Creates an AppMenu::MainComponent, initialized with the menu format saved 
 * through AppMenu::Settings.
 */
juce::Component* AppMenu::createAppMenu()
{
    return new MainComponent();
}
