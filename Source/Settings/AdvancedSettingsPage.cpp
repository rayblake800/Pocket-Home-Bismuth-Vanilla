#include "AdvancedSettingsPage.h"
#include "../PocketHomeApplication.h"

AdvancedSettingsPage::AdvancedSettingsPage(AppConfigFile& appConfig) :
bg_color(0xffd23c6d),
titleLabel("settings", "Advanced Settings"),
setPasswordButton("Set your password"),
removePasswordButton("Remove your password"),
personalizeButton("Personalize your homepage"),
dateTimeButton("Date and time"),
inputOptionsButton("Input settings"),
personalizePage(appConfig),
backButton(ComponentConfigFile::pageLeftKey),
prevArrow("pageUpIcon.svg"),
nextArrow("pageDownIcon.svg")
{
    std::vector<Button*> allButtons = getButtonList(true);
    for (Button* button : allButtons)
    {
        button->addListener(this);
    }
    titleLabel.setJustificationType(Justification::centred);
    addAndMakeVisible(backButton);
    reloadLayout();
}

AdvancedSettingsPage::~AdvancedSettingsPage()
{
}

/**
 * Get pointers to all buttons on the page
 */
std::vector<Button*> AdvancedSettingsPage::getButtonList(bool includeAll)
{
    std::vector<Button*> buttonList;
    buttonList.push_back(&personalizeButton);
    buttonList.push_back(&setPasswordButton);
    if (loginSettingPage.hasPassword() || includeAll)
    {
        buttonList.push_back(&removePasswordButton);
    }
    buttonList.push_back(&dateTimeButton);
    buttonList.push_back(&inputOptionsButton);
    if (includeAll)
    {
        buttonList.push_back(&backButton);
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
    setPasswordButton.setButtonText(loginSettingPage.hasPassword() ?
            "Change your password" : "Set your password");
    std::vector<Button*> buttons = getButtonList();
    if (buttonIndex >= buttons.size() || buttonIndex < 0)
    {
        buttonIndex = 0;
    }
    layoutManager.clearLayout(true);
    layoutManager.addRow(2);
    layoutManager.addComponent(&titleLabel, 0, 1, this);
    layoutManager.addComponent((buttonIndex > 0) ?
            &prevArrow : nullptr,
            layoutManager.getNumRows(), 1, this);
    for (int i = buttonIndex; i < (buttonIndex + buttonsPerPage); i++)
    {
        layoutManager.addRow(2);
        layoutManager.addComponent((i < buttons.size()) ?
                buttons[i] : nullptr,
                layoutManager.getNumRows() - 1, 1, this);
    }
    layoutManager.addComponent((buttonIndex + buttonsPerPage < buttons.size()) ?
            &nextArrow : nullptr,
            layoutManager.getNumRows(), 1, this);
    if (isVisible())
    {
        resized();
    }
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
void AdvancedSettingsPage::buttonClicked(Button* button)
{
    PageStackComponent& mainStack = PocketHomeApplication::getInstance()
            ->getMainStack();
    if (button == &backButton)
    {
        mainStack.popPage(PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &setPasswordButton)
    {
        loginSettingPage.switchToModify();
        mainStack.pushPage(&loginSettingPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &removePasswordButton
            && loginSettingPage.hasPassword())
    {
        loginSettingPage.switchToRemove();
        mainStack.pushPage(&loginSettingPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &personalizeButton)
    {
        mainStack.pushPage(&personalizePage,
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &nextArrow)
    {
        buttonIndex += buttonsPerPage;
        reloadLayout();
    } else if (button == &prevArrow)
    {
        buttonIndex -= buttonsPerPage;
        reloadLayout();
    } else if (button == &inputOptionsButton)
    {
        mainStack.pushPage(&inputPage,
                PageStackComponent::kTransitionTranslateHorizontal);
    } else if (button == &dateTimeButton)
    {
        mainStack.pushPage(&dateTimePage,
                PageStackComponent::kTransitionTranslateHorizontal);
    }
}

/**
 * Fills in the background color
 */
void AdvancedSettingsPage::paint(Graphics& g)
{
    g.fillAll(bg_color);
}

/**
 * Layout child components to fit within the page bounds
 */
void AdvancedSettingsPage::resized()
{
    backButton.applyConfigBounds();
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(backButton.getWidth(), 0);
    layoutManager.layoutComponents(bounds, 0, bounds.getHeight() / 20);
}






