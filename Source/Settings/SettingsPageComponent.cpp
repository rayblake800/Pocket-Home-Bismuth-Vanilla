#include "../PocketHomeApplication.h"
#include "../Utils.h"
#include "../PokeLookAndFeel.h"
#include "SettingsPageComponent.h"

SettingsPageComponent::SettingsPageComponent() :
brightnessSliderTimer(this),
volumeSliderTimer(this)
{
    bgColor = Colour(0xffd23c6d);
    bgImage = createImageFromFile(assetFile("settingsBackground.png"));
    mainPage = new Component();
    addAndMakeVisible(mainPage);
    mainPage->toBack();
    ChildProcess child{};

    /* Adding the personalize button */
    advancedPage = new AdvancedSettingsPage();
    advanced = new TextButton("Advanced Settings");
    advanced->addListener(this);
    addAndMakeVisible(advanced);

    brightness = 8;
#if JUCE_LINUX
    // Get initial brightness value
    if (child.start("cat /sys/class/backlight/backlight/brightness"))
    {
        String result{child.readAllProcessOutput()};
        brightness = result.getIntValue();
    };
#endif


    volume = 90;

#if JUCE_LINUX
    // Get initial volume value
    StringArray cmd{ "amixer", "sget", "Power Amplifier"};
    if (child.start(cmd))
    {
        const String result(child.readAllProcessOutput());
        int resultIndex = result.indexOf("[") + 1;
        child.waitForProcessToFinish(5 * 1000);
        char buff[4];
        for (int i = 0; i < 4; i++)
        {
            char c = result[resultIndex + i];
            if (c >= '0' && c <= '9')
            {
                buff[i] = c;
            } else
            {
                buff[i] = (char) 0;
            }
        }
        String newVol = String(buff);
        volume = newVol.getIntValue();
    }
#endif
    ScopedPointer<Drawable> brightLow=Drawable::createFromImageFile
            (assetFile("brightnessIconLo.png"));
    ScopedPointer<Drawable> brightHigh=Drawable::createFromImageFile
            (assetFile("brightnessIconHi.png"));
    screenBrightnessSlider = new IconSliderComponent(brightLow,brightHigh);
    screenBrightnessSlider->addListener(this);
    screenBrightnessSlider->setValue(1 + (brightness - 0.09)*10);

    ScopedPointer<Drawable> volLow=Drawable::createFromImageFile
            (assetFile("volumeIconLo.png"));
    ScopedPointer<Drawable> volHigh=Drawable::createFromImageFile
            (assetFile("volumeIconHi.png"));
    volumeSlider = new IconSliderComponent(volLow,volHigh);
    volumeSlider->addListener(this);
    volumeSlider->setValue(volume);

    // create back button
    backButton = createImageButton(
            "Back", createImageFromFile(assetFile("backIcon.png")));
    backButton->addListener(this);
    backButton->setAlwaysOnTop(true);
    addAndMakeVisible(backButton);

    wifiCategoryItem = new WifiCategoryItemComponent();
    wifiCategoryItem->button->addListener(this);
    addAndMakeVisible(wifiCategoryItem);
    PocketHomeApplication::getInstance()
            ->getWifiStatus().addListener(wifiCategoryItem);

    addAndMakeVisible(screenBrightnessSlider);
    addAndMakeVisible(volumeSlider);

    wifiPage = new SettingsPageWifiComponent();
}

SettingsPageComponent::~SettingsPageComponent()
{
}

void SettingsPageComponent::deleteIcon(String name, String shell)
{

    advancedPage->deleteIcon(name, shell);
}

void SettingsPageComponent::paint(Graphics &g)
{

    auto bounds = getLocalBounds();
    g.fillAll(bgColor);
    g.drawImage(bgImage, bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(), 0, 0, bgImage.getWidth(), bgImage.getHeight(), false);
}

void SettingsPageComponent::resized()
{
    auto bounds = getLocalBounds();
    int numRows = 4;
    double rowProp = 0.6 / numRows;
    {
        for (int i = 0, j = 0; i < numRows; ++i)
        {

            if (i > 0) verticalLayout.setItemLayout(j++, 0, -1, -1);
            verticalLayout.setItemLayout(j++, -rowProp, -rowProp, -rowProp);
        }

        Component * settingsItems[] = {
            wifiCategoryItem, nullptr,
            screenBrightnessSlider, nullptr,
            volumeSlider, nullptr,
            advanced
        };
        int numItems = sizeof (settingsItems) / sizeof (Component*);

        auto b = bounds;
        b.setLeft(60);
        b.setTop(30);
        b.setHeight(b.getHeight() - 30);
        b.setWidth(b.getWidth() - 60);
        DBG(String("Laying out ") + String(numItems) + String(" items within") + bounds.toString());
        verticalLayout.layOutComponents(settingsItems, numItems, b.getX(), b.getY(), b.getWidth(),
                b.getHeight(), true, true);
    }

    mainPage->setBounds(bounds);

    backButton->setBounds(bounds.getX(), bounds.getY(), 60, bounds.getHeight());
}

void SettingsPageComponent::buttonClicked(Button *button)
{
    PageStackComponent& mainStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == backButton)
    {
        mainStack.popPage(PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == wifiCategoryItem->button)
    {
        wifiPage->updateAccessPoints();
        mainStack.pushPage(wifiPage, PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == advanced)
    {

        mainStack.pushPage(advancedPage, PageStackComponent::kTransitionTranslateHorizontal);
    }
}

void SettingsPageComponent::setSoundVolume()
{
    volume = volumeSlider->getValue();
#if JUCE_LINUX
    StringArray cmd{"amixer", "sset", "Power Amplifier", (String(volume) + "%").toRawUTF8()};
    ChildProcess child;
    if (child.start(cmd))
    {

        String result{child.readAllProcessOutput()};
    }

#endif
}

void SettingsPageComponent::setScreenBrightness()
{
    brightness = 1 + (screenBrightnessSlider->getValue()*0.09);
#if JUCE_LINUX
    ChildProcess child;
    StringArray cmd{"sh", "-c", (String("echo ") + String(brightness) + String(" > /sys/class/backlight/backlight/brightness")).toRawUTF8()};
    if (child.start(cmd))
    {

        String result{child.readAllProcessOutput()};
        DBG(result);
    }
#endif
}

void SettingsPageComponent::sliderValueChanged(Slider* slider)
{
    //
}

void SettingsPageComponent::sliderDragStarted(Slider* slider)
{
    if (screenBrightnessSlider->ownsSlider(slider) && !brightnessSliderTimer.isTimerRunning())
    {
        brightnessSliderTimer.startTimer(200);
    } else if (volumeSlider->ownsSlider(slider) && !volumeSliderTimer.isTimerRunning())
    {

        volumeSliderTimer.startTimer(200);
    }
}

void SettingsPageComponent::sliderDragEnded(Slider* slider)
{
    if (screenBrightnessSlider->ownsSlider(slider)
            && brightnessSliderTimer.isTimerRunning())
    {
        brightnessSliderTimer.stopTimer();
        setScreenBrightness();
    } else if (volumeSlider->ownsSlider(slider)
            && volumeSliderTimer.isTimerRunning())
    {

        volumeSliderTimer.stopTimer();
        setSoundVolume();
    }
}

SettingsPageComponent::BrightnessTimer::BrightnessTimer
(SettingsPageComponent* settingsPage) : settingsPage(settingsPage)
{
}

void SettingsPageComponent::BrightnessTimer::timerCallback()
{
    if (settingsPage != nullptr)
    {

        settingsPage->setScreenBrightness();
    }
}

SettingsPageComponent::VolumeTimer::VolumeTimer
(SettingsPageComponent* settingsPage) : settingsPage(settingsPage)
{
}

void SettingsPageComponent::VolumeTimer::timerCallback()
{
    if (settingsPage != nullptr)
    {
        settingsPage->setSoundVolume();
    }
}
