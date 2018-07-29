#include "Utils.h"
#include "JsonWifiInterface.h"
#include "LibNMInterface.h"
#include "PocketHomeApplication.h"
#include "PocketHomeWindow.h"

PocketHomeWindow::PocketHomeWindow(juce::String windowName, bool fakeWifi) :
WindowFocus::BroadcastWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons),
wifiManager([fakeWifi]
(juce::ReadWriteLock& lock)->WifiStateManager::NetworkInterface*
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
    using namespace juce;
#    if JUCE_DEBUG
    setBounds(10, 10, 480, 272);
#    else
    Rectangle<int> screenSize = getDisplaySize();

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

/*
 * closes the application normally.
 */
void PocketHomeWindow::closeButtonPressed()
{
    using namespace juce;
    JUCEApplication::getInstance()->systemRequestedQuit();
}

/*
 * Resize page content to match window size.
 */
void PocketHomeWindow::resized()
{
    using namespace juce;
    const Rectangle<int>& bounds = getLocalBounds();
    pageStack.setBounds(bounds);
    if (loginPage != nullptr)
    {
        loginPage->setBounds(bounds);
    }
}

