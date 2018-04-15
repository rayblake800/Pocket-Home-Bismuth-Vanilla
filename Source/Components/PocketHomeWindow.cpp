#include "Display.h"
#include "Utils.h"
#include "JsonWifiInterface.h"
#include "LibNMInterface.h"
#include "PocketHomeApplication.h"
#include "PocketHomeWindow.h"

PocketHomeWindow::PocketHomeWindow(String windowName, bool fakeWifi) :
WindowFocus::BroadcastWindow(windowName, Colours::darkgrey,
        DocumentWindow::allButtons),
wifiManager([fakeWifi]
(CriticalSection& lock)->WifiStateManager::NetworkInterface*
{
    if(fakeWifi)
    {
        return new JsonWifiInterface(lock);
    }
    else
    {
        return new LibNMInterface(lock);
    }
})
{
    ASSERT_SINGULAR;

#    if JUCE_DEBUG
    setBounds(10, 10, 480, 272);
#    else
    Rectangle<int> screenSize = Display::getDisplaySize();

    setBounds(0, 0, screenSize.getWidth(), screenSize.getHeight());
#    endif
    setWantsKeyboardFocus(false);
    setUsingNativeTitleBar(true);
    setResizable(true, false);
    setLookAndFeel(&LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);

    loginPage = static_cast<LoginPage*>
            (pageFactory.createLoginPage([this]()
            {

                setContentNonOwned(&pageStack, true);
            }));
    loginPage->setBounds(getBounds());
    if (loginPage->hasPassword())
    {
        setContentNonOwned(loginPage, true);
        loginPage->textFocus();
    }
    else
    {
        setContentNonOwned(&pageStack, true);
    }
    pageStack.setRootPage(pageFactory.createHomePage());
}

/**
 * closes the application normally.
 */
void PocketHomeWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

/**
 * Resize page content to match window size.
 */
void PocketHomeWindow::resized()
{
    const Rectangle<int>& bounds = getLocalBounds();
    pageStack.setBounds(bounds);
    if (loginPage != nullptr)
    {
        loginPage->setBounds(bounds);
    }
}

