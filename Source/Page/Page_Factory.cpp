#include "Page_Factory.h"
#include "Page_Type.h"
#include "Page_HomeSettings.h"
#include "Page_PasswordEditor.h"
#include "Page_PasswordRemover.h"
#include "Page_WifiConnection.h"
#include "HomePage.h"
#include "PowerPage.h"
#include "FelPage.h"
#include "QuickSettingsPage.h"
#include "SettingsListPage.h"
#include "InputSettingsPage.h"
#include "Theme_Colour_ConfigPage.h"

/*
 * Creates an instance of the application's home page.
 */
Page::Component* Page::Factory::createHomePage()
{
    Component* homePage = new HomePage;
    homePage->setFactoryInterface(this);
    return homePage;
}

/*
 * Creates a new page to push on top of the page stack.
 */
Page::Component* Page::Factory::createPage(const Type pageType)
{
    Component* newPage = nullptr;
    switch(pageType)
    {
        case Type::power:
            newPage = new PowerPage;
            break;
        case Type::fel:
            newPage = new FelPage;
            break;
        case Type::quickSettings:
            newPage = new QuickSettingsPage;
            break;
        case Type::wifiConnection:
            newPage = new WifiConnection;
            break;
        case Type::settingsList:
            newPage = new SettingsListPage;
            break;
        case Type::inputSettings:
            newPage = new InputSettingsPage;
            break;
        case Type::setPassword:
            newPage = new PasswordEditor;
            break;
        case Type::removePassword:
            newPage = new PasswordRemover;
            break;
        case Type::colourSettings:
            newPage = new Theme::Colour::ConfigPage;
            break;
        case Type::homeSettings:
            newPage = new HomeSettings;
    }
    if(newPage != nullptr)
    {
        newPage->setFactoryInterface(this);
    }
    return newPage;
}
