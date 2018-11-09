#pragma once
#include "LoginPage.h"
#include "PageFactory.h"
#include "PageStackComponent.h"
#include "WifiStateManager.h"
#include "WindowFocus.h"

/**
 * @file PocketHomeWindow.h
 * 
 * @brief The sole application window object and the root component in the 
 *        component display tree. 
 */

class PocketHomeWindow : public WindowFocus::BroadcastWindow
{
public:
    /**
     * @param windowName         Sets the text of the window title bar.
     * 
     * @param fakeWifi           If true, wifi-using components in this window
     *                            will get all their wifi data from a simulated 
     *                            wifi device.
     */
    PocketHomeWindow(
            juce::String windowName, 
            bool fakeWifi);

    virtual ~PocketHomeWindow() { }

private:
    /**
     * closes the application normally.
     */
    void closeButtonPressed() override;

    /**
     * Resize page content to match window size.
     */
    void resized() override;

    //Factory object for creating all UI pages
    PageFactory pageFactory;
    
    WifiStateManager wifiManager;

    //Holds all page components, besides the login page
    PageStackComponent pageStack;

    //LoginPage will be initially shown instead of the pageStack if a password
    //has been set.
    juce::ScopedPointer<LoginPage> loginPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
