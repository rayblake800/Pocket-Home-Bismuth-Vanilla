#include "Display.h"
#include "Utils.h"
#include "PocketHomeApplication.h"
#include "PocketHomeWindow.h"

PocketHomeWindow::PocketHomeWindow(
        String windowName,
        bool fakeWifi,
        MainConfigFile& mainConfig,
        ComponentConfigFile& componentConfig) :
WindowFocus::BroadcastWindow(windowName, Colours::darkgrey,
DocumentWindow::allButtons),
mainConfig(mainConfig),
componentConfig(componentConfig),
lookAndFeel(mainConfig, componentConfig),
pageFactory(mainConfig, componentConfig, fakeWifi)
{
    ASSERT_SINGULAR;
    LookAndFeel::setDefaultLookAndFeel(&lookAndFeel);

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

