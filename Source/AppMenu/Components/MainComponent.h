/* Only include this file directly in the AppMenu implementation! */
#ifdef APPMENU_IMPLEMENTATION_ONLY
#pragma once
#include "JuceHeader.h"
#include "OverlaySpinner.h"
#include "DesktopEntry/Loader.h"
#include "AppMenu/Control/Initializer.h"
#include "AppMenu/Components/MenuComponent.h"
#include "AppMenu/Control/Controller.h"
#include "AppMenu/Data/JSON/ConfigFile.h"
#include "AppMenu/Implementation.h"
/**
 * @file  AppMenu/Components/MainComponent.h
 *
 * @brief  Creates and shows an AppMenu of any format.
 *
 * The MainComponent is a container component that holds the AppMenu's menu
 * component, along with the menu's loading spinner. When the menu is created 
 * by AppMenu::createAppMenu, the generic juce::Component pointer returned by
 * that function is actually an AppMenu::MainComponent.
 *
 * The MainComponent's main responsibility is to initialize the menu component
 * in a particular AppMenu format, and automatically update the menu whenever
 * a new menu format is selected. 
 */
class AppMenu::MainComponent : public juce::Component 
{
public:
    /**
     * @brief  Creates and initializes the application menu.
     */
    MainComponent();

    virtual ~MainComponent();

private:
    /**
     * @brief  Initialize the menu as a new menu format, cleaning up any
     *         existing menu first.
     *
     * @param initializer  A struct that defines how to initialize the new menu
     *                     format. If this matches the format of the existing
     *                     menu, no action will be taken.
     */
    void loadMenuFormat(const Initializer* initializer);

    /**
     * @brief  Safely destroys all AppMenu objects held in the MainComponent.
     */
    void destroyMenu();

    /**
     * @brief  Notifies the MenuComponent that its parent's bounds have changed
     *         whenever the MainComponent is moved or resized.
     */
    virtual void resized() final override;

    /**
     * @brief  Private listener class that updates the menu format whenever the
     *         saved format selection changes.
     */
    class FormatUpdater : public ConfigFile::Listener
    {
    public:
        /**
         * @brief  Initializes the updater, tracking the format key and saving
         *         a pointer to the MainComponent it updates.
         *
         * @param mainComponent  The MainComponent that holds this updater.
         */
        FormatUpdater(MainComponent* mainComponent);

        virtual ~FormatUpdater() { }

        /**
         * @brief  Applies the selected menu format to the updater's 
         *         MainComponent.
         */
        void applySelectedFormat();

        /**
         * @brief  Updates the menu when the selected format changes.
         *
         * @param propertyKey  The menu format key.
         */
        virtual void configValueChanged(const juce::Identifier& propertyKey)
            final override;

    private:
        MainComponent* const mainComponent;
    };
    FormatUpdater formatUpdater;

    /* Tracks the current AppMenu format. */
    AppMenu::Format currentMenuFormat = Format::Invalid;

    /* Loads, caches, and updates desktop entry files.  This resource should
       exist as long as the AppMenu exists. */
    DesktopEntry::Loader entryLoader;

    /* The menu's loading spinner. */
    OverlaySpinner loadingSpinner;

    /* Handles the appearance and layout of the menu. */
    std::unique_ptr<MenuComponent> menuComponent;

    /* Allows the inputHandler to control menu behavior. */
    std::unique_ptr<Controller> controller;

    /* Listens to key, mouse, and window focus events, and responds by
       triggering appropriate menu events. */
    std::unique_ptr<InputHandler> inputHandler;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

/* Only include this file directly in the AppMenu implementation! */
#endif
