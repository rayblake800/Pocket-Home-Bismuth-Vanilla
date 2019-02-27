#include "PocketHomeWindow.h"
#include "Utils.h"

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
        juce::DocumentWindow::allButtons)
{
    juce::Rectangle<int> screenSize = getDisplaySize();
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

    loginPage.reset(new LoginPage([this]()
    {
        setContentNonOwned(&pageStack, true);
    }));
    loginPage->setBounds(getBounds());
    if (loginPage->hasPassword())
    {
        setContentNonOwned(loginPage.get(), true);
        loginPage->textFocus();
    }
    else
    {
        setContentNonOwned(&pageStack, true);
    }
    pageStack.setRootPage(pageFactory.createHomePage());
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
    if (loginPage != nullptr)
    {
        loginPage->setBounds(bounds);
    }
}

