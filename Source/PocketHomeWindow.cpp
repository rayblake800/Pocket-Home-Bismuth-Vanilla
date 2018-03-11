#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "WindowFocusedTimer.h"
#include "Utils.h"
#include "PocketHomeWindow.h"
#include "PocketHomeApplication.h"

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

void PocketHomeWindow::activeWindowStatusChanged()
{
    WifiStatus& wifiStatus =
            PocketHomeApplication::getInstance()->getWifiStatus();
    if (isActiveWindow())
    {
        WindowFocusedTimer::windowFocusGained();
        if (wifiStatus.isEnabled())
        {
            Thread* wifiThread = dynamic_cast<Thread*> (&wifiStatus);
            if (wifiThread != nullptr && !wifiThread->isThreadRunning())
            {
                DBG(__func__ << "starting wifi thread");
                wifiThread->startThread();
            }
        }
    }
    else
    {
        Thread* wifiThread = dynamic_cast<Thread*> (&wifiStatus);
        if (wifiThread != nullptr && wifiThread->isThreadRunning())
        {
            DBG(__func__ << ":stopping wifi thread");
            wifiThread->signalThreadShouldExit();
            wifiThread->notify();
        }
        WindowFocusedTimer::windowFocusLost();
    }
    pageStack.windowFocusChanged(isActiveWindow());
}

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
