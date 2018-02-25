/**
 * @file SettingsPage.h
 * 
 * TODO: documentation
 */
#pragma once

#include "../../Basic Components/IconSliderComponent.h"
#include "../../Basic Components/SwitchComponent.h"
#include "../../Configuration/AppConfigFile.h"
#include "../../Configuration/Configurables/ConfigurableImageButton.h"
#include "../../PageComponent.h"
#include "../../WindowFocusedTimer.h"
#include "WifiSettingsComponent.h"
#include "WifiSettingsPage.h"
#include "../AdvancedSettingsPage.h"

class SettingsPage : public PageComponent, public WindowFocusedTimer,
        private Slider::Listener {
public:
    SettingsPage();
    ~SettingsPage(); 
private:
    virtual void visibilityChanged() override;
    virtual void timerCallback() override;
    void pageButtonClicked(Button *b) override;

    void setSoundVolume();
    void setScreenBrightness();

    void sliderValueChanged(Slider* slider){};
    void sliderDragStarted(Slider* slider);
    void sliderDragEnded(Slider* slider);

    void checkSliders();
    Slider* changingSlider;
    
    
    WifiSettingsPage wifiSettingsPage;
    WifiSettingsComponent wifiCategoryItem;
    IconSliderComponent screenBrightnessSlider;
    IconSliderComponent volumeSlider;
    TextButton advancedPageButton;
    AdvancedSettingsPage advancedSettingsPage;
    unsigned int brightness;
    unsigned int volume;
};
