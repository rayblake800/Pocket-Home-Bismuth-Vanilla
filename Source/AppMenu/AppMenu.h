#pragma once
/**
 * @file  AppMenu/AppMenu.h
 *
 * @brief  Creates the AppMenu application menu component, and returns it as a
 *         generic juce::Component pointer.
 */

#include "JuceHeader.h"

/**
 *  This defines the interface used outside the AppMenu to create application
 * menu objects. The application menu is returned as a generic juce::Component
 * pointer, and can be treated like any other Component.
 */
namespace AppMenu
{
    /**
     * @brief  Creates an AppMenu::MainComponent, initialized with the menu
     *         format saved through AppMenu::Settings.
     *
     * @return  A pointer to a newly allocated and initialized 
     *          AppMenu::MainComponent.
     */
    juce::Component* createAppMenu();
}
