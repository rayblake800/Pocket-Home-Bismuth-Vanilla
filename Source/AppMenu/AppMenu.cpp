#define APPMENU_IMPLEMENTATION
#include "AppMenu.h"
#include "AppMenu_MainComponent.h"

/*
 * Creates an AppMenu::MainComponent, initialized with the menu format saved 
 * through AppMenu::Settings.
 */
juce::Component* AppMenu::createAppMenu()
{
    return new MainComponent();
}
