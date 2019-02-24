#pragma once
#include "LoginPage.h"
#include "Page_Factory.h"
#include "Page_StackComponent.h"
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
     * @param windowName  Sets the text of the window title bar.
     */
    PocketHomeWindow(juce::String windowName);

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
    Page::Factory pageFactory;
    
    //Holds all page components, besides the login page
    Page::StackComponent pageStack;

    //LoginPage will be initially shown instead of the pageStack if a password
    //has been set.
    juce::ScopedPointer<LoginPage> loginPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
