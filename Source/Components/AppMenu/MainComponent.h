#pragma once
#include "AppMenu.h"
#include "JuceHeader.h"
#include "Scrolling/Controller.h"
#include "Page/Controller.h"
#include "OverlaySpinner.h"

/**
 * @file MainComponent.h
 *
 * @brief  The public interface for the AppMenu component.
 *
 */

class AppMenu::MainComponent : public juce::Component
{
public:
    MainComponent();
    virtual ~MainComponent();

    void initMenu(const AppMenu::Type menuType);

    class Controller
    {
    protected:
        friend class MainComponent;

        Controller();

    public:
        virtual ~Controller();

    protected:
        void setLoadingSpinnerVisible(const bool shouldShow);

        void addAndShowMenu(juce::Component* menuComponent);

    private:
        MainComponent& parent;
    };
private:
    OverlaySpinner loadingSpinner;
};
