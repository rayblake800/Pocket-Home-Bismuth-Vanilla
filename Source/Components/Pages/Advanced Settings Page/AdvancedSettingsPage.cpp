#include "AdvancedSettingsPage.h"
#include "Password.h"

AdvancedSettingsPage::AdvancedSettingsPage() :
Localized("AdvancedSettingsPage"),
PageComponent("AdvancedSettingsPage"),
titleLabel("settings", localeText(advanced_settings)),
setPasswordButton(localeText(set_password)),
removePasswordButton(localeText(remove_password)),
personalizeButton(localeText(personalize_homepage)),
dateTimeButton(localeText(date_and_time)),
inputOptionsButton(localeText(input_settings)),
prevArrow(ComponentConfigFile::pageUpKey),
nextArrow(ComponentConfigFile::pageDownKey)
{

#    if JUCE_DEBUG
    setName("AdvancedSettingsPage");
#    endif
    std::vector<Button*> allButtons = getButtonList(true);
    for (Button* button : allButtons)
    {
        button->addListener(this);
    }
    addAndMakeVisible(prevArrow);
    addAndMakeVisible(nextArrow);
    titleLabel.setJustificationType(Justification::centred);
    reloadLayout();
}

/**
 * Get pointers to all buttons on the page
 */
std::vector<Button*> AdvancedSettingsPage::getButtonList(bool includeAll)
{
    std::vector<Button*> buttonList;
    buttonList.push_back(&personalizeButton);
    buttonList.push_back(&setPasswordButton);
    if (Password::isPasswordSet() || includeAll)
    {
        buttonList.push_back(&removePasswordButton);
    }
    buttonList.push_back(&dateTimeButton);
    buttonList.push_back(&inputOptionsButton);
    if (includeAll)
    {
        buttonList.push_back(&prevArrow);
        buttonList.push_back(&nextArrow);
    }
    return buttonList;
}

/**
 * Reloads the page layout settings.
 */
void AdvancedSettingsPage::reloadLayout()
{
    setPasswordButton.setButtonText(Password::isPasswordSet() ?
            localeText(change_password) : localeText(set_password));
    std::vector<Button*> buttons = getButtonList();
    if (buttonIndex >= buttons.size() || buttonIndex < 0)
    {
        buttonIndex = 0;
    }
    prevArrow.setVisible(buttonIndex > 0);
    nextArrow.setVisible(buttonIndex + buttonsPerPage < buttons.size());

    ComponentConfigFile config;
    ComponentConfigFile::ComponentSettings prevBtnSettings
            = config.getComponentSettings(ComponentConfigFile::pageUpKey);
    ComponentConfigFile::ComponentSettings nextBtnSettings
            = config.getComponentSettings(ComponentConfigFile::pageDownKey);
    using RowItem = RelativeLayoutManager::ComponentLayout;
    RelativeLayoutManager::Layout layout(
    {
            {
                .weight = 40, .rowItems = 
                {
                    RowItem(buttonIndex > 0 ? nullptr : &titleLabel, 10),
                }
            }
    });

    for (int i = buttonIndex; i < (buttonIndex + buttonsPerPage); i++)
    {
        layout.addRow(
        {
            .weight = 30, .rowItems = 
            {
                RowItem((i < buttons.size()) ? buttons[i] : nullptr, 10)
            }
        });
    }
    
    float yMarginFraction = std::max(prevBtnSettings.getYFraction()
            + prevBtnSettings.getHeightFraction(),
            nextBtnSettings.getYFraction());
    layout.setYMarginFraction(yMarginFraction);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(1);
    updateLayout(layout);
}

/**
 * Reloads page layout whenever the page becomes visible.
 */
void AdvancedSettingsPage::visibilityChanged()
{
    if (isVisible())
    {
        reloadLayout();
    }
}

/**
 * Handle button clicks to open menu pages, close this page, or 
 * scroll the list of page buttons.
 */
void AdvancedSettingsPage::pageButtonClicked(Button * button)
{
    PageComponent::PageType pageType =
            PageComponent::PageType::AdvancedSettings;
    if (button == &setPasswordButton)
    {
        pageType = PageComponent::PageType::SetPassword;
    }
    else if (button == &removePasswordButton)
    {
        pageType = PageComponent::PageType::RemovePassword;
    }
    else if (button == &personalizeButton)
    {
        pageType = PageComponent::PageType::HomeSettings;
    }
    else if (button == &inputOptionsButton)
    {
        pageType = PageComponent::PageType::InputSettings;
    }
    else if (button == &dateTimeButton)
    {
        pageType = PageComponent::PageType::DateTime;
    }
    else if (button == &nextArrow)
    {
        buttonIndex += buttonsPerPage;
        reloadLayout();
    }
    else if (button == &prevArrow)
    {
        buttonIndex -= buttonsPerPage;
        reloadLayout();
    }
    if (pageType != PageComponent::PageType::AdvancedSettings)
    {
        pushPageToStack(pageType);
    }
}

/**
 * Updates the up/down navigation buttons to fit when the page changes
 * size.
 */
void AdvancedSettingsPage::pageResized()
{
    prevArrow.applyConfigBounds();
    nextArrow.applyConfigBounds();
}








