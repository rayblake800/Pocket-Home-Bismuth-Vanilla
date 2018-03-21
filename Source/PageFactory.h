/**
 * @file PageFactory.h
 */
#pragma once
#include "ComponentConfigFile.h"
#include "MainConfigFile.h"
#include "WifiStateManager.h"
#include "BluetoothStatus.h"
#include "PageStackComponent.h"
#include "JuceHeader.h"

class PageFactory
{
public:
    PageFactory(WifiStateManager* wifiManager,
            BluetoothStatus* bluetoothStatus);

    virtual ~PageFactory() { }
    
    enum PageType
    {
        Home,
        InputSettings,
        Keybinding,
        SetPassword,
        RemovePassword,
        Power,
        Fel,
        Settings,
        BluetoothSettings,
        WifiSettings,
        UI,
        Colour,
        AdvancedSettings,
        DateTime,
        HomeSettings,
        Login
    };

    PageStackComponent::Page* createPage(PageType pageType);
private:
    ComponentConfigFile componentConfig;
    MainConfigFile mainConfig;
    ScopedPointer<WifiStateManager> wifiManager;
    ScopedPointer<BluetoothStatus> bluetoothStatus;
};
