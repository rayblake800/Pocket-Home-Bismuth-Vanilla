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

PageFactory::PageFactory(MainConfigFile& mainConfig,
        ComponentConfigFile& componentConfig,
        bool fakeWifi) :
mainConfig(mainConfig),
componentConfig(componentConfig)
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
    return new HomePage(this, wifiManager, mainConfig, componentConfig);
}

/**
 * Initializes a login page instance.
 */
PageComponent* PageFactory::createLoginPage
(std::function<void () > loginCallback)
{
    return new LoginPage(componentConfig, loginCallback);
}

/**
 * Create a new page to push on top of the page stack.
 */
PageComponent* PageFactory::createPage(PageComponent::PageType type)
{
    switch (type)
    {
        case PageComponent::InputSettings:
            return new InputSettingsPage(this, mainConfig, componentConfig);
        case PageComponent::Keybinding:
            return nullptr;
        case PageComponent::SetPassword:
            return new SetPasswordPage(componentConfig);
        case PageComponent::RemovePassword:
            return new RemovePasswordPage(componentConfig);
        case PageComponent::Power:
            return new PowerPage(this, mainConfig, componentConfig);
        case PageComponent::Fel:
            return new FelPage(componentConfig);
        case PageComponent::Settings:
            return new SettingsPage(this, componentConfig, wifiManager);
        case PageComponent::BluetoothSettings:
            return new BluetoothSettingsPage(componentConfig, bluetoothStatus);
        case PageComponent::WifiSettings:
            return new WifiSettingsPage(componentConfig, wifiManager);
        case PageComponent::UI:
            return nullptr;
        case PageComponent::ColourSettings:
            return new ColourPage(componentConfig);
        case PageComponent::AdvancedSettings:
            return new AdvancedSettingsPage(this, componentConfig);
        case PageComponent::DateTime:
            return new DateTimePage(mainConfig, componentConfig);
        case PageComponent::HomeSettings:
            return new HomeSettingsPage(mainConfig, componentConfig);
    }
    return nullptr;
}