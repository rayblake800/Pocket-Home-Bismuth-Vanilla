/**
 * @file SettingsPage.h
 * 
 * TODO: documentation
 */
#pragma once

#include "IconSliderComponent.h"
#include "SwitchComponent.h"
#include "AppConfigFile.h"
#include "ConfigurableImageButton.h"
#include "PageComponent.h"
#include "WindowFocusedTimer.h"
#include "WifiSettingsComponent.h"
#include "WifiStateManager.h"
//#include "BluetoothSettingsComponent.h"

class SettingsPage : public PageComponent, public WindowFocusedTimer,
        private Slider::Listener {
public:
    SettingsPage(PageFactoryInterface* pageFactory,
            ComponentConfigFile& config,
            WifiStateManager& wifiManager);
    
    ~SettingsPage() { }
private:
    virtual void visibilityChanged() override;
    virtual void timerCallback() override;
    void pageButtonClicked(Button *b) override;

    void sliderValueChanged(Slider* slider){};
    void sliderDragStarted(Slider* slider);
    void sliderDragEnded(Slider* slider);

    void checkSliders();
    Slider* changingSlider;
    
    WifiSettingsComponent wifiComponent;
//    BluetoothSettingsComponent bluetoothComponent;
    IconSliderComponent screenBrightnessSlider;
    IconSliderComponent volumeSlider;
    TextButton advancedPageButton;
    unsigned int brightness;
    unsigned int volume;
};
