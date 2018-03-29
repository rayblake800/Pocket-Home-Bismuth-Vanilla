/**
 * @file PageFactory.h
 * 
 * PageFactory creates all new PageComponent objects, and initializes all
 * shared resources needed by pages.
 */
#pragma once
#include "ComponentConfigFile.h"
#include "MainConfigFile.h"
#include "WifiStateManager.h"
#include "BluetoothStatus.h"
#include "PageComponent.h"
#include "JuceHeader.h"

class PageFactory : public PageComponent::PageFactoryInterface
{
public:
    /**
     * @param fakeWifi          If true, the wifiManager shared by all pages 
     *                           will use simulated wifi data, rather than 
     *                           trying to connect to a real wifi device.
     */
    PageFactory(bool fakeWifi);

    virtual ~PageFactory() { }

    /**
     * Initializes a HomePage instance to use as the root page of the page 
     * stack.
     */
    PageComponent* createHomePage();

    /**
     * Initializes a login page instance.
     * 
     * @param loginCallback  A callback function for the page to run when the 
     *                        user successfully logs in.
     */
    PageComponent* createLoginPage(std::function<void () > loginCallback);

private:
    /**
     * Create a new page to push on top of the page stack.
     */
    PageComponent* createPage(PageComponent::PageType type) override;

    //shared page resources
    WifiStateManager wifiManager;
    BluetoothStatus bluetoothStatus;
};
