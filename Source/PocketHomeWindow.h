#pragma once
/**
 * @file PocketHomeWindow.h
 * 
 * @brief  Creates the main application window.
 */

#include "WindowFocus.h"
#include "Page_Factory.h"
#include "Page_StackComponent.h"
#include "Password_LoginScreen.h"

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

    /**
     * @brief  Gets a pointer to the current open window object.
     *
     * @return  The application window pointer, or nullptr if the window is
     *          not currently open.
     */
    static PocketHomeWindow* getOpenWindow();

    /**
     * @brief  Fills the main application window with the login screen 
     *         component.
     *
     *  This replaces the usual window contents with a login screen. All other
     * application content will remain inaccessable until the user uses the
     * login screen controls to log in.
     */
    void showLoginScreen();

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

    /* Replaces application content until the user logs in: */
    Password::LoginScreen loginScreen;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
