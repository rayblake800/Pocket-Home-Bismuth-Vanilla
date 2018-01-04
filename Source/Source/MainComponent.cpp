#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "PokeLookAndFeel.h"
#include "LauncherComponent.h"
#include "Utils.h"
#include "MainComponent.h"

MainContentComponent::MainContentComponent() {
    lookAndFeel = new PokeLookAndFeel();
    setLookAndFeel(lookAndFeel);
    setWantsKeyboardFocus(false);

    //Function to execute when the login button is pressed on login page
    loginPage = new LoginPage([this] () {
        loggedIn();
    });
    LookAndFeel::setDefaultLookAndFeel(lookAndFeel);

    pageStack = new PageStackComponent();

    if (loginPage->hasPassword()) {
        addAndMakeVisible(loginPage);
        loginPage->textFocus();
    } else {
        addAndMakeVisible(pageStack);
    }
    pageStack->pushPage(&appMenuPage, PageStackComponent::kTransitionNone);
    setSize(480, 272);
    repaint();
}

void MainContentComponent::loggedIn() {
    removeChildComponent(loginPage);
    addAndMakeVisible(pageStack);
}

PageStackComponent * MainContentComponent::getPageStack() {
    return pageStack;
}

MainContentComponent::~MainContentComponent() {
}

void MainContentComponent::paint(Graphics &g) {
    g.fillAll(Colours::white);
}

void MainContentComponent::resized() {
    auto bounds = getLocalBounds();
    if (pageStack != nullptr) {
        pageStack->setBounds(bounds);
    }
    if (loginPage != nullptr) {
        loginPage->setBounds(bounds);
    }
}

void MainContentComponent::handleMainWindowInactive() {
    appMenuPage.stopWaitingOnLaunch();
}

