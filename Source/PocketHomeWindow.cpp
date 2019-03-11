#include "PocketHomeWindow.h"
#include "Password.h"
#include "Display.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "PocketHomeWindow::";
/* Make the window the size of the pocketCHIP display on debug builds: */
static const constexpr int dbgWidth = 480;
static const constexpr int dbgHeight = 272;
#endif

/*
 * Creates and shows the main application window.
 */
PocketHomeWindow::PocketHomeWindow(juce::String windowName) :
WindowFocus::BroadcastWindow(windowName, juce::Colours::darkgrey,
        juce::DocumentWindow::allButtons),
loginScreen([this](){ setContentNonOwned(&pageStack, true); })
{
    juce::Rectangle<int> screenSize = Display::getSize();
#if JUCE_DEBUG
    setBounds(screenSize.getWidth() / 2 - dbgWidth / 2,
            screenSize.getHeight() / 2 - dbgHeight / 2,
            dbgWidth, dbgHeight);
#else
    setBounds(0, 0, screenSize.getWidth(), screenSize.getHeight());
#endif
    setWantsKeyboardFocus(false);
    setUsingNativeTitleBar(true);
    setResizable(true, false);
    setLookAndFeel(&juce::LookAndFeel::getDefaultLookAndFeel());
    setVisible(true);
    setWantsKeyboardFocus(false);

    loginScreen.setBounds(getLocalBounds());
    if(Password::isPasswordSet())
    {
        showLoginScreen();
    }
    else
    {
        setContentNonOwned(&pageStack, true);
    }
    pageStack.setRootPage((Page::Interface::Component*) 
            pageFactory.createHomePage());
}

/*
 * Gets a pointer to the current open window object.
 */
PocketHomeWindow* PocketHomeWindow::getOpenWindow()
{
    juce::Component* rootComponent 
            = juce::Desktop::getInstance().getComponent(0);
    return dynamic_cast<PocketHomeWindow*>(rootComponent);
}

/*
 * Fills the main application window with the login screen component.
 */
void PocketHomeWindow::showLoginScreen()
{
    setContentNonOwned(&loginScreen, true);
    loginScreen.getKeyboardFocus();
}

/*
 * Closes the application normally when the window closes.
 */
void PocketHomeWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

/*
 * Resizes page content to match the window size.
 */
void PocketHomeWindow::resized()
{
    const juce::Rectangle<int> bounds = getLocalBounds();
    pageStack.setBounds(bounds);
    loginScreen.setBounds(bounds);
}

