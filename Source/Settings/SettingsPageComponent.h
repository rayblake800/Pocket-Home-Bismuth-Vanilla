#pragma once

#include "../Basic Components/IconSliderComponent.h"
#include "../Basic Components/SwitchComponent.h"
#include "../Configuration/AppConfigFile.h"
#include "../Configuration/Configurables/ConfigurableImageButton.h"
#include "SettingsPageWifiComponent.h"
#include "SettingsPageBluetoothComponent.h"
#include "WifiCategoryItemComponent.h"
#include "SettingsCategoryButton.h"
#include "SettingsPageLogin.h"
#include "AdvancedSettingsPage.h"

class SettingsPageComponent : public Component, private Button::Listener,
        private Slider::Listener {
public:

    SettingsPageComponent(AppConfigFile& appConfig);
    ~SettingsPageComponent();

    void paint(Graphics &g) override;
    void resized() override;

    void buttonClicked(Button *b) override;

    void setSoundVolume();
    void setScreenBrightness();

    void sliderValueChanged(Slider* slider);
    void sliderDragStarted(Slider* slider);
    void sliderDragEnded(Slider* slider);

    void checkSliders(); 
    
    /* Advanced Settings */
    ScopedPointer<TextButton> advanced;
    ScopedPointer<AdvancedSettingsPage> advancedPage;
    void deleteIcon(String, String);
    /*******************/

private:
    
    ScopedPointer<IconSliderComponent> screenBrightnessSlider, volumeSlider;
    ConfigurableImageButton backButton;
    ScopedPointer<Component> mainPage;
    ScopedPointer<WifiCategoryItemComponent> wifiCategoryItem;

   

    ScopedPointer<SettingsPageWifiComponent> wifiPage;
    StretchableLayoutManager verticalLayout;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SettingsPageComponent)
    Colour bgColor;
    Image bgImage;

    class BrightnessTimer : public Timer {
    public:
        BrightnessTimer(SettingsPageComponent* settingsPage);
    private:
        virtual void timerCallback() override;
        SettingsPageComponent* settingsPage;
    };
    BrightnessTimer brightnessSliderTimer;

    class VolumeTimer : public Timer {
    public:
        VolumeTimer(SettingsPageComponent* settingsPage);
    private:
        virtual void timerCallback() override;
        SettingsPageComponent* settingsPage;
    };
    VolumeTimer volumeSliderTimer;
    unsigned int brightness;
    unsigned int volume;
};
