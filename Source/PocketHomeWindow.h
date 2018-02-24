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

class PocketHomeWindow : public DocumentWindow {
public:

    PocketHomeWindow(String windowName);
    ~PocketHomeWindow();
private:
    void activeWindowStatusChanged() override;
    void closeButtonPressed() override;
    void paint(Graphics &) override;
    void resized() override;
    void handleMainWindowInactive();

    HomePage homePage;
    PageStackComponent pageStack;
    LoginPage loginPage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PocketHomeWindow)
};
