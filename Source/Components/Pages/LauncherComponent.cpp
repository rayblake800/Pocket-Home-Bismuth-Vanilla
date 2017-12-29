#include <math.h>
#include <algorithm>
#include "../../ConfigFile.h"
#include "../../Main.h"
#include "../../Utils.h"
#include "SettingsPageComponent.h"
#include "PowerPageComponent.h"
#include "AppMenuPage.h"
#include "LauncherComponent.h"

enum BatteryIconImage{
    BATTERY_0,
    BATTERY_1,
    BATTERY_2,
    BATTERY_3,
    CHARGING_0,
    CHARGING_1,
    CHARGING_2,
    CHARGING_3
};

enum WifiIconImage{
    WIFI_OFF,
    WIFI_STRENGTH_0,
    WIFI_STRENGTH_1,
    WIFI_STRENGTH_2,
    WIFI_STRENGTH_3
};

void BatteryIconTimer::timerCallback() {
    // get current battery status from the battery monitor thread
    auto batteryStatus = launcherComponent->batteryMonitor.getCurrentStatus();

    // we can't change anything if we don't have a LauncherComponent
    if (launcherComponent) {
        for (VectorImageButton * button : *buttons) {
            BatteryIconImage batteryImg = BATTERY_3;
            if (button->getName() == "Battery") {
                int status = round(((float) batteryStatus.percentage) / 100.0f * 3.0f);

                //int pct = (int) batteryStatus.percentage;
                int pct = 100;
                String pct_s = std::to_string(pct) + " %";
                launcherComponent->batteryLabel->setText(pct_s, dontSendNotification);

                if (batteryStatus.percentage <= 5) {
                    status = 3;
                } else {
                    // limit status range to [0:3]
                    if (status < 0) status = 0;
                    if (status > 2) status = 2;
                }
                if (!batteryStatus.isCharging) {
                    batteryImg = (BatteryIconImage)status;
                } else {
                    batteryImg = (BatteryIconImage)(status+(int)CHARGING_0);

                }

                button->setImage((int) batteryImg);
            }
        }
    }
    //DBG( "Charging: "  << batteryStatus.isCharging );
    //DBG( "Voltage: " << batteryStatus.percentage );

}

void WifiIconTimer::timerCallback() {
    if (!launcherComponent) {
        return;
    }

    for (VectorImageButton * button : *buttons) {
        if (button->getName() == "WiFi") {
            WifiIconImage wifiIcon;
            const auto& conAp = getWifiStatus().connectedAccessPoint();

            // wifi on and connected
            if (getWifiStatus().isConnected() && conAp) {
                //Get IP and show it  
                launcherComponent->updateIp();
                // 0 to 100
                float sigStrength = std::max(0, std::min(99, conAp->signalStrength));
                wifiIcon = (WifiIconImage)(2+(int)(sigStrength*3/100));
            }// wifi on but no connection
            else if (getWifiStatus().isEnabled()) {
                wifiIcon = WIFI_STRENGTH_0;
                launcherComponent->setIpVisible(false);
            }// wifi off
            else {
                wifiIcon = WIFI_OFF;
                launcherComponent->setIpVisible(false);
            }

            button->setImage((int)wifiIcon);
        }
    }
}

void LauncherComponent::setColorBackground(const String& str) {
    String value = "FF" + str;
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << value;
    ss >> x;
    bgColor = Colour(x);
    hasImg = false;
}

void LauncherComponent::setImageBackground(const String& str) {
    String value(str);
    if (value == "") value = "mainBackground.png";
    File f;
    if (value[0] == '~' || value[0] == '/') f = File(value);
    else f = assetFile(value);
    bgImage = createImageFromFile(f);
    hasImg = true;
}

void LauncherComponent::setClockVisible(bool visible) {
    if (visible) {
        if (clock->isThreadRunning()) return;
        addAndMakeVisible(clock->getLabel(), 10);
        clock->startThread();
    } else {
        if (!clock->isThreadRunning()) return;
        Label& l = clock->getLabel();
        removeChildComponent(&l);
        clock->stopThread(1500);
    }
}

void LauncherComponent::setClockAMPM(bool ampm) {
    clock->setAmMode(ampm);
}

void LauncherComponent::hideLaunchSpinner() {
    AppMenuPage * appPage = (AppMenuPage *) pagesByName["Apps"];
    if (appPage != nullptr) {
        appPage->hideLaunchSpinner();
    }
}

LauncherComponent::LauncherComponent() :
clock(nullptr), labelip("ip", "") {
    pageStack = new PageStackComponent();
    pageStack->setWantsKeyboardFocus(false);
    addAndMakeVisible(pageStack);

    ConfigFile * config = ConfigFile::getInstance();
    /* Ip settings */
    labelip.setVisible(false);

    std::function<void(Label*, ConfigFile::ComponentType) > positionLabel =
            [this, config](Label * label, ConfigFile::ComponentType type) {
                config->getComponentSettings(type).applyBounds(label);
                label->setFont(Font(label->getHeight()));
                label->setWantsKeyboardFocus(false);
                addAndMakeVisible(label);
            };
    /* Setting the clock */
    clock = new ClockMonitor;
    positionLabel(&(clock->getLabel()), ConfigFile::CLOCK);
    clock->getLabel().setJustificationType(Justification::centredRight);
    String formatclock = config->getConfigString(ConfigFile::TIME_FORMAT);
    setClockVisible(config->getConfigBool(ConfigFile::SHOW_CLOCK));
    setClockAMPM(formatclock == "ampm");

    /* Battery percentage label */
    batteryLabel = new Label("percentage", "-%");
    positionLabel(batteryLabel, ConfigFile::BATTERY_PERCENT);
    batteryLabel->setJustificationType(Justification::centredLeft);

    String value = config->getConfigString(ConfigFile::BACKGROUND);

    bgColor = Colour(0x4D4D4D);
    if (value.length() == 6 && value.containsOnly("0123456789ABCDEF"))
        setColorBackground(value);
    else
        setImageBackground(value);

    focusButtonPopup = new ImageComponent("Focus Button Popup");
    focusButtonPopup->setInterceptsMouseClicks(false, false);
    focusButtonPopup->setWantsKeyboardFocus(false);
    addChildComponent(focusButtonPopup);

    // Settings page
    auto settingsPage = new SettingsPageComponent(this);
    settingsPage->addChildComponent(labelip);
    settingsPage->setName("Settings");
    pages.add(settingsPage);
    pagesByName.set("Settings", settingsPage);

    // Power page
    auto powerPage = new PowerPageComponent();
    powerPage->setName("Power");
    pages.add(powerPage);
    pagesByName.set("Power", powerPage);


    auto appsPage = new AppMenuPage(this);
    appsPage->setName("Apps");
    pages.add(appsPage);
    pagesByName.set("Apps", appsPage);

    // Read config for button locations and images
    std::function<void(ConfigFile::ComponentSettings, String) > loadButton =
            [this](ConfigFile::ComponentSettings buttonSettings, String name) {
                VectorImageButton * button = new VectorImageButton(buttonSettings,name);
                button->setWantsKeyboardFocus(false);
                if (name == "Power" || name == "Settings") {
                    button->addListener(this);
                } else {
                    button->setInterceptsMouseClicks(false, false);
                }
                addAndMakeVisible(button);
                cornerButtons.add(button);
            };

    loadButton(config->getComponentSettings(ConfigFile::BATTERY), "Battery");
    loadButton(config->getComponentSettings(ConfigFile::WIFI), "Wifi");
    loadButton(config->getComponentSettings(ConfigFile::POWER), "Power");
    loadButton(config->getComponentSettings(ConfigFile::SETTINGS), "Settings");

    defaultPage = appsPage;

    batteryMonitor.updateStatus();
    batteryMonitor.startThread();

    batteryIconTimer.launcherComponent = this;
    batteryIconTimer.buttons = &cornerButtons;
    batteryIconTimer.startTimer(1000);
    batteryIconTimer.timerCallback();

    wifiIconTimer.launcherComponent = this;
    wifiIconTimer.buttons = &cornerButtons;
    wifiIconTimer.startTimer(2000);
    wifiIconTimer.timerCallback();

}

LauncherComponent::~LauncherComponent() {
    batteryIconTimer.stopTimer();
    batteryMonitor.stopThread(2000);
}

void LauncherComponent::paint(Graphics &g) {
    auto bounds = getLocalBounds();
    g.fillAll(bgColor);
    if (hasImg) g.drawImage(bgImage, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 0, 0, bgImage.getWidth(), bgImage.getHeight(), false);
    //g.drawImage(trashButton, bounds.getX()+395, bounds.getY()+16, 40, 20, 0, 0, 50, 50, false);
}

void LauncherComponent::resized() {
    ConfigFile * config = ConfigFile::getInstance();
    Rectangle<int>bounds = getLocalBounds();
    for (VectorImageButton * button : cornerButtons) {
        ConfigFile::ComponentType componentType;
        if (button->getName() == "Battery") {
            componentType = ConfigFile::BATTERY;
        } else if (button->getName() == "Wifi") {
            componentType = ConfigFile::WIFI;
        } else if (button->getName() == "Power") {
            componentType = ConfigFile::POWER;
        } else if (button->getName() == "Settings") {
            componentType = ConfigFile::SETTINGS;
        } else {
            DBG(String("LauncherComponent: found unexpected button ") + button->getName());
            break;
        }
        config->getComponentSettings(componentType).applyBounds(button);
    }

    pageStack->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    //pageStack->setWantsKeyboardFocus(true);

    std::function<void(ConfigFile::ComponentType, Label*) > resizeText =
            [this, config](ConfigFile::ComponentType type, Label * label) {
                Font labelFont = label->getFont();
                config->getComponentSettings(type).applyBounds(label);
                labelFont.setHeight(label->getHeight());
                label->setFont(labelFont);
            };
    resizeText(ConfigFile::BATTERY_PERCENT, batteryLabel);
    resizeText(ConfigFile::CLOCK, &(clock->getLabel()));
    labelip.setBounds(bounds.getX() + 190, bounds.getY(), 100, 30);
    // init
    if (!resize) {
        resize = true;
        pageStack->swapPage(defaultPage, PageStackComponent::kTransitionNone);
    }
}

void LauncherComponent::updateIp() {
    //Creating a socket
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    //This will help us getting the IPv4 associated with wlan0 interface
    struct ifreq ifr;
    memset(&ifr, 0, sizeof (ifr));
    ifr.ifr_addr.sa_family = AF_INET;
    //Copying the string "wlan0" in the structure
    sprintf(ifr.ifr_name, "wlan0");
    //Getting the informations of the socket, so IP address
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);

    char* addr = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);
    String ip(addr);
    //Showing the new ip if different than 0.0.0.0
    if (addr == "0.0.0.0") {
        labelip.setVisible(false);
        return;
    }
    labelip.setText("IP: " + ip, dontSendNotification);
    labelip.setVisible(true);
}

void LauncherComponent::setIpVisible(bool v) {
    labelip.setVisible(v);
}

void LauncherComponent::buttonClicked(Button *button) {
    auto currentPage = pageStack->getCurrentPage();

    if ((!currentPage || currentPage->getName() != button->getName()) &&
            pagesByName.contains(button->getName())) {
        auto page = pagesByName[button->getName()];
        if (button->getName() == "Settings") {
            getMainStack().pushPage(page, PageStackComponent::kTransitionTranslateHorizontal);
        } else if (button->getName() == "Power") {
            getMainStack().pushPage(page, PageStackComponent::kTransitionTranslateHorizontalLeft);
        } else {
            pageStack->swapPage(page, PageStackComponent::kTransitionTranslateHorizontal);
        }
    }
}


