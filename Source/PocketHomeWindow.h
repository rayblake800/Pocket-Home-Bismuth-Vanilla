/**
 * @file PocketHomeWindow.h
 * 
 * PocketHomeWindow is the application window object and the root component in 
 * the component display tree. 
 */

#pragma once
#include "Pages/Settings Page/SettingsPage.h"
#include "Pages/PowerPage.h"
#include "Pages/HomePage/HomePage.h"
#include "Pages/LoginPage.h"
#include "PageStackComponent.h"
#include "WindowFocus.h"

class PocketHomeWindow : public WindowFocus::BroadcastWindow {
public:
    /**
     * @param windowName sets the text of the window title bar
     */
    PocketHomeWindow(String windowName);
    
    ~PocketHomeWindow();    

    
private:
    
    /**
     * closes the application normally.
     */
    void closeButtonPressed() override;

    
    //void paint(Graphics &) override;
    
    /**
     * Resize page content to match window size.
     */
    void resized() override;

    
    HomePage homePage;
    PageStackComponent pageStack;
    LoginPage loginPage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
