#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "LauncherComponent.h"
#include "Utils.h"
#include "MainComponent.h"

MainContentComponent::MainContentComponent() :
loginPage([this] ()
{

    loggedIn();
})
{
    setWantsKeyboardFocus(false);

    if (loginPage.hasPassword())
    {
        addAndMakeVisible(loginPage);
        loginPage.textFocus();
    } else
    {
        addAndMakeVisible(pageStack);
    }
    pageStack.pushPage(&appMenuPage, PageStackComponent::kTransitionNone);
#if JUCE_DEBUG
    setSize(480, 272);
#else
    Rectangle<int> screenSize =
            Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    setSize(screenSize.getWidth(),screenSize.getHeight());
#endif
    repaint();
}

void MainContentComponent::loggedIn()
{
    removeChildComponent(&loginPage);
    addAndMakeVisible(pageStack);
}

PageStackComponent& MainContentComponent::getPageStack()
{
    return pageStack;
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint(Graphics &g)
{
    g.fillAll(Colours::white);
}

void MainContentComponent::resized()
{
    auto bounds = getLocalBounds();
    pageStack.setBounds(bounds);
    loginPage.setBounds(bounds);
}

void MainContentComponent::handleMainWindowInactive()
{
    appMenuPage.stopWaitingOnLaunch();
}

