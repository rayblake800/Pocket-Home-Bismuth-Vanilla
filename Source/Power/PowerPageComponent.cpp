
#include <numeric>
#include "../Utils.h"
#include "../PocketHomeApplication.h"
#include "../LoginPage.h"
#include "../PokeLookAndFeel.h"
#include "PowerPageFelComponent.h"
#include "PowerPageComponent.h"

unsigned char PowerPageComponent::rev_number = 9;
unsigned char PowerPageComponent::bug_number = 0;

PowerPageComponent::PowerPageComponent() :
bgColor(Colours::black),
backButton("nextIcon.svg"),
powerOffButton("Shutdown"),
rebootButton("Reboot"),
sleepButton("Sleep"),
felButton("Flash Software"),
lockscreen([this]()
{

    hideLockscreen();
})
{
    std::vector<GridLayoutManager::ComponentLayoutParams> pageLayout = {
        {nullptr, 0, 1},
        {nullptr, 1, 1},
        {&powerOffButton, 1, 6},
        {nullptr, 1, 1},

        {nullptr, 2, 1},
        {&sleepButton, 2, 6},
        {nullptr, 2, 1},

        {nullptr, 3, 1},
        {&rebootButton, 3, 6},
        {nullptr, 3, 1},

        {nullptr, 4, 1},
        {&felButton, 4, 6},
        {nullptr, 4, 1},
        {nullptr, 5, 1}
    };
    layoutManager.addComponents(pageLayout, this);

    std::vector<Button*> buttons = {
        &backButton,
        &powerOffButton,
        &sleepButton,
        &rebootButton,
        &felButton
    };
    for (Button* button : buttons)
    {
        button->addListener(this);
    }

    backButton.setAlwaysOnTop(true);
    addAndMakeVisible(backButton);
    addChildComponent(overlaySpinner);

}

PowerPageComponent::~PowerPageComponent()
{
}

/**
 * Turns off the display until key or mouse input is detected.
 * The lock screen will be visible when the display turns on again.
 */
void PowerPageComponent::startSleepMode()
{
#if JUCE_LINUX
    StringArray cmd{ "xset", "q", "|", "grep", "is O"};
    if (commandProcess.start(cmd))
    {
        const String result(commandProcess.readAllProcessOutput());
        if (result == "Monitor is Off")
        {
            commandProcess.start("xset dpms force on");
        } else
        {
            addAndMakeVisible(lockscreen);
            lockscreen.setAlwaysOnTop(true);
            //Turn off the screen
            commandProcess.start("xset dpms force off");
        }
    }
#endif
}

/**
 * If the lock screen is visible, this will remove it from the screen.
 */
void PowerPageComponent::hideLockscreen()
{
    if (lockscreen.isShowing())
    {
        removeChildComponent(&lockscreen);
        PocketHomeApplication::getInstance()
                ->getMainStack().popPage(PageStackComponent::kTransitionNone);
    }
}

/**
 * Show the power spinner to indicate to the user that the system is
 * restarting or shutting down.
 */
void PowerPageComponent::showPowerSpinner()
{
    backButton.setVisible(false);
    powerOffButton.setVisible(false);
    sleepButton.setVisible(false);
    rebootButton.setVisible(false);
    felButton.setVisible(false);
    overlaySpinner.setVisible(true);
}

/**
 * Fills in the background with bgColor.
 */
void PowerPageComponent::paint(Graphics &g)
{
    g.fillAll(bgColor);
}

/**
 * Resize all child components to fit the page.
 */
void PowerPageComponent::resized()
{

    Rectangle<int> bounds = getLocalBounds();
    overlaySpinner.setBounds(bounds);
    lockscreen.setBounds(bounds);
    layoutManager.layoutComponents(bounds, 0, bounds.getHeight() / 20);

    Rectangle<int> backButtonBounds = bounds;
    backButtonBounds.setLeft(sleepButton.getRight());
    backButtonBounds.setTop(sleepButton.getBounds().getCentreY());
    backButtonBounds.setBottom(rebootButton.getBounds().getCentreY());
    backButton.setBounds(backButtonBounds);
}

/**
 * Draw buttons differently on mouse-over or button click.
 */
void PowerPageComponent::buttonStateChanged(Button *btn)
{
    if (btn->isMouseButtonDown() && btn->isMouseOver())
    {
        btn->setAlpha(0.5f);
    } else
    {
        btn->setAlpha(1.0f);
    }
}

/**
 * Handles all button clicks.
 */
void PowerPageComponent::buttonClicked(Button *button)
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    PageStackComponent& mainStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == &backButton)
    {
        mainStack.popPage
                (PageStackComponent::kTransitionTranslateHorizontalLeft);
    } else if (button == &powerOffButton)
    {
        showPowerSpinner();
        commandProcess.start(config.getConfigString
                (MainConfigFile::shutdownCommandKey));
    } else if (button == &rebootButton)
    {
        showPowerSpinner();
        commandProcess.start(config.getConfigString
                (MainConfigFile::restartCommandKey));
    } else if (button == &sleepButton)
    {
        startSleepMode();
    } else if (button == &felButton)
    {
        mainStack.pushPage(&felPage,
                PageStackComponent::kTransitionTranslateHorizontalLeft);
    }
}
