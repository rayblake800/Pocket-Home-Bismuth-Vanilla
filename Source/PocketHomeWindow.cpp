#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Utils.h"
#include "PocketHomeWindow.h"

PocketHomeWindow::PocketHomeWindow(String windowName) :
DocumentWindow(windowName, Colours::darkgrey, DocumentWindow::allButtons),
loginPage([this] ()
{

    setContentNonOwned(&pageStack, true);
})
{
#if JUCE_DEBUG
    setBounds(10, 10, 480, 272);
#else
    Rectangle<int> screenSize =
            Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    setBounds(0, 0, screenSize.getWidth(), screenSize.getHeight());
#endif
    setWantsKeyboardFocus(false);
    setUsingNativeTitleBar(true);
    setResizable(true, false);
    setLookAndFeel(&LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);

    if (loginPage.hasPassword())
    {
        setContentNonOwned(&loginPage, true);
        loginPage.textFocus();
    }
    else
    {
        setContentNonOwned(&pageStack, true);
    }
    pageStack.pushPage(&homePage, PageStackComponent::kTransitionNone);

}

PocketHomeWindow::~PocketHomeWindow() { }

void PocketHomeWindow::activeWindowStatusChanged() { }

void PocketHomeWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

void PocketHomeWindow::paint(Graphics &g)
{
    g.fillAll(Colours::white);
}

void PocketHomeWindow::resized()
{
    auto bounds = getLocalBounds();
    pageStack.setBounds(bounds);
    loginPage.setBounds(bounds);
}

void PocketHomeWindow::handleMainWindowInactive()
{
    homePage.stopWaitingOnLaunch();
}

