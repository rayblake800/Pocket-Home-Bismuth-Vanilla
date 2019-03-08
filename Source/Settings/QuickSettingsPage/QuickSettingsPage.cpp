#include "QuickSettingsPage.h"
#include "Theme_Image_JSONKeys.h"
#include "Layout_Component_JSONKeys.h"
#include "Page_Type.h"
#include "Utils.h"

QuickSettingsPage::QuickSettingsPage() :
pageListener(*this),
wifiComponent([this]()
{
    pushPageToStack(Page::Type::wifiConnection);
}),
settingsListBtn(Theme::Image::JSONKeys::settingsListBtn),
listButtonManager(&settingsListBtn, 
        Layout::Component::JSONKeys::settingsListBtn)
{
#if JUCE_DEBUG
    setName("QuickSettingsPage");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(10, { RowItem(&wifiComponent) } ),
        Row(10, { RowItem(&brightnessSlider) } ),
        Row(10, { RowItem(&volumeSlider) } )
    });
    layout.setXMarginFraction(
            1.0 - listButtonManager.getLayout().getXFraction());
    layout.setYMarginFraction(0.1);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    setLayout(layout);
    addAndMakeVisible(settingsListBtn);
}

/*
 * Updates the advanced settings button when the page is resized.
 */
void QuickSettingsPage::pageResized()
{
    listButtonManager.applyConfigBounds();
}

/*
 * Opens the advanced settings page when its button is clicked.
 */
void QuickSettingsPage::PageListener::buttonClicked(juce::Button *button)
{
    jassert(button == &settingsPage.settingsListBtn);
    settingsPage.pushPageToStack(Page::Type::settingsList);
}
