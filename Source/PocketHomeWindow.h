#pragma once
/**
 * @file PocketHomeWindow.h
 * 
 * @brief  Creates the main application window.
 */

#include "LoginPage.h"
#include "Page_Factory.h"
#include "Page_StackComponent.h"
#include "WindowFocus.h"

/**
 * @brief The sole application window object and the root component in the 
 *        component display tree. 
 */
class PocketHomeWindow : public WindowFocus::BroadcastWindow
{
public:
    /**
     * @brief  Creates and shows the main application window.
     *
     * @param windowName  Sets the text of the window title bar.
     */
    PocketHomeWindow(juce::String windowName);

    virtual ~PocketHomeWindow() { }

private:
    /**
     * @brief  Closes the application normally when the window closes.
     */
    void closeButtonPressed() override;

    /**
     * @brief  Resizes page content to match the window size.
     */
    virtual void resized() override;

    /* Creates all pages on the page stack: */
    Page::Factory pageFactory;
    
    /* Holds all page components besides the login page: */
    Page::StackComponent pageStack;

    /* LoginPage will be initially shown instead of the pageStack if a password
     * has been set: */
    std::unique_ptr<LoginPage> loginPage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
