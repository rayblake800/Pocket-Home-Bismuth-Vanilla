#include "Page_Factory.h"
#include "Page_Fel.h"
#include "Page_Type.h"
#include "Page_HomeSettings.h"
#include "Page_InputSettings.h"
#include "Page_PasswordEditor.h"
#include "Page_PasswordRemover.h"
#include "Page_Power.h"
#include "Page_QuickSettings.h"
#include "Page_SettingsList.h"
#include "Theme_Colour_ConfigPage.h"
#include "HomePage.h"

#ifdef WIFI_SUPPORTED
#include "Page_WifiConnection.h"
#endif

// Creates an instance of the application's home page.
Page::Component* Page::Factory::createHomePage()
{
    Component* homePage = new HomePage;
    homePage->setFactoryInterface(this);
    return homePage;
}


// Creates a new page to push on top of the page stack.
Page::Component* Page::Factory::createPage(const Type pageType)
{
    Component* newPage = nullptr;
    switch(pageType)
    {
        case Type::power:
            newPage = new Power;
            break;
        case Type::quickSettings:
            newPage = new QuickSettings;
            break;
        case Type::settingsList:
            newPage = new SettingsList;
            break;
        case Type::inputSettings:
            newPage = new InputSettings;
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
            break;
#ifdef CHIP_FEATURES
        case Type::fel:
            newPage = new Fel;
            break;
#endif
#ifdef WIFI_SUPPORTED
        case Type::wifiConnection:
            newPage = new WifiConnection;
            break;
#endif
    }
    if (newPage != nullptr)
    {
        newPage->setFactoryInterface(this);
    }
    return newPage;
}
