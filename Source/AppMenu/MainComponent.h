#pragma once
#include "AppMenu.h"
#include "OverlaySpinner.h"
#include "JuceHeader.h"

/**
 * @file MainComponent.h
 *
 * @brief  The public interface for the AppMenu component.
 * 
 * AppMenu::MainComponent displays the application menu, and provides an
 * interface for switching between several menu implementations.  MainComponent
 * also holds the loading spinner used to indicate that menu data is loading or
 * an application is being launched from the menu.
 *
 * MainComponent is only responsible for initializing the menu Controller and 
 * the loading OverlaySpinner, holding the main menu Component created by the
 * Controller, and notifying the Controller when its bounds change.  All other
 * menu behavior is handled by the Controller or the main menu Component.
 *
 */

class AppMenu::MainComponent : public juce::Component
{
public:
    /**
     * @brief  Creates the menu component and controls its behavior.
     */
    class Controller;
    
    /**
     * @brief  Initializes the menu controller, and adds and shows its main
     *         menu component as a child under the loading spinner.
     *
     * @param menuController  The application menu controller.  MainComponent
     *                        will delete this controller on destruction or 
     *                        whenever a new controller is assigned.
     */
    MainComponent(MainComponent::Controller* menuController);

    virtual ~MainComponent() { }

    /**
     * @brief  Initializes or recreates the menu with a new menu controller.
     *
     * @param menuType  The new menu controller to initialize.
     */
    void initMenu(MainComponent::Controller* newController);

    /**
     * @brief  Gets the current menu format.
     *
     * @return  The AppMenu format of the MainComponent's Controller object. 
     */
    AppMenu::Format getMenuFormat() const;

    /**
     * @brief  Creates the menu component and controls its behavior.
     */
    class Controller
    {
    public:
        /* Only MainComponent may access the Controller's private methods. */
        friend class MainComponent;

        Controller() { } 

        virtual ~Controller() { }

    protected:
        /**
         * @brief  Activates or deactivates the loading spinner component held
         *         by the Controller's MainComponent.
         *
         * @param shouldShow  Whether the loading spinner should be shown or
         *                    hidden.
         */
        void setLoadingSpinnerVisible(const bool shouldShow);

    private:
        /**
         * @brief  Links the MainComponent's loading spinner to the Controller
         *         so it can show or hide the spinner.
         *
         * @param spinner  The address of the MainComponent's loading spinner.
         */
        void connectLoadingSpinner(OverlaySpinner* spinner);

        /**
         * @brief  Gets the main menu component, creating and saving it first if
         *         necessary.
         *
         * This will only be used to add and show the menu component as a child
         * component of the MainComponent.
         *
         * @return  A Component pointer to the main menu component.
         */
        virtual juce::Component* initMenuComponent() = 0;

        /**
         * @brief  Triggers whenever the parent MainComponent is resized.
         *
         * This should be implemented to update the main menu component's bounds
         * to match the new parent bounds.
         *
         * @param parentBounds  The new bounds of the parent MainComponent.
         */
        virtual void parentResized(const juce::Rectangle<int> parentBounds) = 0;

        /**
         * @brief  Gets the menu controller's AppMenu format.
         *
         * @return  The controller's menu format. 
         */
        virtual AppMenu::Format getMenuType() const = 0;

        /* A pointer to the MainComponent's loading spinner. */
        OverlaySpinner* loadingSpinner = nullptr;
    };
private:
    /**
     * @brief  Notifies the Controller that its parent's bounds have changed
     *         whenever the MainComponent is moved or resized.
     */
    virtual void resized() override;

    /* The menu's loading spinner. */
    OverlaySpinner loadingSpinner;
    /* The controller that creates and controls the application menu. */
    std::unique_ptr<Controller> menuController = nullptr;
};
