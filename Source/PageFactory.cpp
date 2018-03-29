#include "JsonWifiInterface.h"
#include "LibNMInterface.h"
#include "HomePage.h"
#include "InputSettingsPage.h"
#include "KeybindingPage.h"
#include "SetPasswordPage.h"
#include "RemovePasswordPage.h"
#include "PowerPage.h"
#include "FelPage.h"
#include "SettingsPage.h"
#include "BluetoothSettingsPage.h"
#include "WifiSettingsPage.h"
#include "UIPage.h"
#include "ColourPage.h"
#include "AdvancedSettingsPage.h"
#include "DateTimePage.h"
#include "HomeSettingsPage.h"
#include "LoginPage.h"
#include "PageFactory.h"

PageFactory::PageFactory(bool fakeWifi)
{
    //Initialize wifi status thread
    if (fakeWifi)
    {
        wifiManager.setNetworkInterface(new JsonWifiInterface());
    }
    else
    {
#        ifdef JUCE_LINUX
        wifiManager.setNetworkInterface(new LibNMInterface());
#        else          
        wifiManager.setNetworkInterface(new JsonWifiInterface());
#        endif
    }
}

/**
 * Initializes a HomePage instance to use as the root page of the page 
 * stack.
 */
PageComponent* PageFactory::createHomePage()
{
    return setPageFactory(new HomePage());
}

/**
 * Initializes a login page instance.
 */
PageComponent* PageFactory::createLoginPage
(std::function<void () > loginCallback)
{
    return new LoginPage(loginCallback);
}

/**
 * Create a new page to push on top of the page stack.
 */
PageComponent* PageFactory::createPage(PageComponent::PageType type)
{
    switch (type)
    {
        case PageComponent::InputSettings:
            return setPageFactory(new InputSettingsPage());
        case PageComponent::Keybinding:
            return nullptr;
        case PageComponent::SetPassword:
            return new SetPasswordPage();
        case PageComponent::RemovePassword:
            return new RemovePasswordPage();
        case PageComponent::Power:
            return setPageFactory(new PowerPage());
        case PageComponent::Fel:
            return new FelPage();
        case PageComponent::Settings:
            return setPageFactory(new SettingsPage());
        case PageComponent::BluetoothSettings:
            return new BluetoothSettingsPage(bluetoothStatus);
        case PageComponent::WifiSettings:
            return new WifiSettingsPage();
        case PageComponent::UI:
            return nullptr;
        case PageComponent::ColourSettings:
            return new ColourPage();
        case PageComponent::AdvancedSettings:
            return setPageFactory(new AdvancedSettingsPage());
        case PageComponent::DateTime:
            return new DateTimePage();
        case PageComponent::HomeSettings:
            return new HomeSettingsPage();
    }
    return nullptr;
}