#include "Page_QuickSettings.h"
#include "Theme_Image_JSONKeys.h"
#include "Layout_Component_JSONKeys.h"
#include "Config_MainFile.h"
#include "Page_Type.h"

/* Page layout constants: */

static const constexpr int ipRowWeight = 3;
static const constexpr int wifiRowWeight = 10;
static const constexpr int sliderRowWeight = 10;
static const constexpr int rowPaddingWeight = 1;
static const constexpr float yMarginFraction = 0.08;

Page::QuickSettings::QuickSettings() :
pageListener(*this),
#ifdef WIFI_SUPPORTED
wifiComponent([this]()
{
    pushPageToStack(Page::Type::wifiConnection);
}),
#endif
settingsListBtn(Theme::Image::JSONKeys::settingsListBtn),
listButtonManager(&settingsListBtn, 
        Layout::Component::JSONKeys::settingsListBtn)
{
#if JUCE_DEBUG
    setName("Page::QuickSettings");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
#ifdef WIFI_SUPPORTED
        Row(wifiRowWeight, 
        { 
            RowItem(&wifiComponent) 
        }),
#endif
        Row(sliderRowWeight, 
        { 
            RowItem(&brightnessSlider) 
        }),
        Row(sliderRowWeight, 
        { 
            RowItem(&volumeSlider) 
        })
    });
    layout.setXMarginFraction(1.0 
            - listButtonManager.getLayout().getXFraction());
    layout.setYMarginFraction(yMarginFraction);
    layout.setYPaddingWeight(rowPaddingWeight);
    Config::MainFile mainConfig;
    if(mainConfig.getIPLabelOnSettingsPage())
    {
        ipLabel.reset(new Info::IPLabel());
        layout.insertRow(0, Row(ipRowWeight, { RowItem(ipLabel.get()) }));
        ipLabel->setJustificationType(juce::Justification::centred);
    }
    setLayout(layout);
    addAndMakeVisible(settingsListBtn);
    settingsListBtn.addListener(&pageListener);
}

/*
 * Updates the settings list button when the page is resized.
 */
void Page::QuickSettings::pageResized()
{
    listButtonManager.applyConfigBounds();
}

/*
 * Opens the settings list page when its button is clicked.
 */
void Page::QuickSettings::PageListener::buttonClicked(juce::Button *button)
{
    jassert(button == &settingsPage.settingsListBtn);
    settingsPage.pushPageToStack(Page::Type::settingsList);
}
