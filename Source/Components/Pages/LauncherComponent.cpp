#include <math.h>
#include <algorithm>
#include "../../ConfigFile.h"
#include "../../Main.h"
#include "../../Utils.h"
#include "SettingsPageComponent.h"
#include "PowerPageComponent.h"
#include "AppMenuPage.h"
#include "LauncherComponent.h"

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
                VectorImageButton * button = new VectorImageButton(buttonSettings, name);
                button->setWantsKeyboardFocus(false);
                if (name == "Power" || name == "Settings") {
                    button->addListener(this);
                } else {
                    button->setInterceptsMouseClicks(false, false);
                }
                addAndMakeVisible(button);
                cornerButtons.add(button);
            };
    loadButton(config->getComponentSettings(ConfigFile::POWER), "Power");
    loadButton(config->getComponentSettings(ConfigFile::SETTINGS), "Settings");

    batteryIcon = new BatteryIcon();
    addAndMakeVisible(batteryIcon);
    wifiIcon = new WifiIcon();
    addAndMakeVisible(wifiIcon);
    defaultPage = appsPage;


}

LauncherComponent::~LauncherComponent() {
}

void LauncherComponent::paint(Graphics &g) {
    auto bounds = getLocalBounds();
    g.fillAll(bgColor);
    if (hasImg) g.drawImage(bgImage, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 0, 0, bgImage.getWidth(), bgImage.getHeight(), false);

}

void LauncherComponent::resized() {
    ConfigFile * config = ConfigFile::getInstance();
    Rectangle<int>bounds = getLocalBounds();
    for (VectorImageButton * button : cornerButtons) {
        ConfigFile::ComponentType componentType;
        if (button->getName() == "Power") {
            componentType = ConfigFile::POWER;
        } else if (button->getName() == "Settings") {
            componentType = ConfigFile::SETTINGS;
        } else {
            DBG(String("LauncherComponent: found unexpected button ") + button->getName());
            break;
        }
        config->getComponentSettings(componentType).applyBounds(button);
    }
    config->getComponentSettings(ConfigFile::BATTERY).applyBounds(batteryIcon);
    config->getComponentSettings(ConfigFile::WIFI).applyBounds(wifiIcon);
    pageStack->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());

    std::function<void(ConfigFile::ComponentType, Label*) > resizeText =
            [this, config](ConfigFile::ComponentType type, Label * label) {
                Font labelFont = label->getFont();
                config->getComponentSettings(type).applyBounds(label);
                labelFont.setHeight(label->getHeight());
                label->setFont(labelFont);
            };
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


