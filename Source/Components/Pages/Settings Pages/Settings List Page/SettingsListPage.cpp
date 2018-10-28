#include "SettingsListPage.h"
#include "Password.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "SettingsListPage";

/* Localized text value keys: */
static const juce::Identifier homeSettingsTitleKey   = "homeSettings";
static const juce::Identifier colorSettingsTitleKey  = "colorSettings";
static const juce::Identifier setPasswordTitleKey    = "setPassword";
static const juce::Identifier changePasswordTitleKey = "changePassword";
static const juce::Identifier removePasswordTitleKey = "removePassword";
static const juce::Identifier dateTimeTitleKey       = "dateTime";
static const juce::Identifier inputSettingsTitleKey  = "inputSettings";

SettingsListPage::SettingsListPage() :
Locale::TextUser(localeClassKey),
PageComponent("SettingsListPage")
{

#    if JUCE_DEBUG
    setName("SettingsListPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    LayoutManager::Layout layout(
    {
        LayoutManager::Row(1,
        {
            LayoutManager::RowItem(&buttonList)
        })
    });
    setLayout(layout);
}

/*
 * Gets button titles for all page buttons.
 */
juce::StringArray SettingsListPage::getButtonTitles()
{
    using namespace juce;
    StringArray titleList;
    titleList.add(localeText(homeSettingsTitleKey));
    titleList.add(localeText(colorSettingsTitleKey));
    if (Password::isPasswordSet())
    {
        titleList.add(localeText(changePasswordTitleKey));
        titleList.add(localeText(removePasswordTitleKey));
    }
    else
    {
        titleList.add(localeText(setPasswordTitleKey));
    }
    titleList.add(localeText(dateTimeTitleKey));
    titleList.add(localeText(inputSettingsTitleKey));
    return titleList;
}

/**
 * Reloads page layout whenever the page becomes visible.
 */
void SettingsListPage::visibilityChanged()
{
    if (isVisible())
    {
        buttonList.refresh();
    }
}

/**
 * Handle button clicks to open menu pages, close this page, or 
 * scroll the list of page buttons.
 */
void SettingsListPage::pageButtonClicked(juce::Button * button)
{
    using namespace juce;
    TextButton * textButton = dynamic_cast<TextButton*>(button);
    if(textButton == nullptr)
    {
        DBG("SettingsListPage::" << __func__ << ": invalid button!");
        return;
    }
    
    Array<PageComponent::PageType> pageTypes;
    pageTypes.add(PageComponent::PageType::HomeSettings);
    pageTypes.add(PageComponent::PageType::ColourSettings);
    pageTypes.add(PageComponent::PageType::SetPassword);
    if(Password::isPasswordSet())
    {
        pageTypes.add(PageComponent::PageType::RemovePassword);
    }
    pageTypes.add(PageComponent::PageType::DateTime);
    pageTypes.add(PageComponent::PageType::InputSettings);
    
    StringArray pageNames = getButtonTitles();
    int typeIndex = pageNames.indexOf(textButton->getButtonText());
    if(typeIndex >= 0 && typeIndex < pageTypes.size())
    {
        pushPageToStack(pageTypes[typeIndex]);
    }
}

SettingsListPage::SettingsList::SettingsList()
{
    setItemsPerPage(buttonsPerPage);
    setYPaddingFraction(yPaddingFraction);
}
     
/*
 * Gets the total number of page buttons.
 */
unsigned int SettingsListPage::SettingsList::getListSize()
{
    return Password::isPasswordSet() ? 6 : 5;
}

/*
 * Updates or creates a page TextButton for a specific button index.
 */
juce::Component* SettingsListPage::SettingsList::updateListItem
(juce::Component* listItem, unsigned int index)
{
    using namespace juce;
    SettingsListPage * parent 
            = static_cast<SettingsListPage*>(getParentComponent());
    String title = parent->getButtonTitles()[index];
    TextButton * textButton = dynamic_cast<TextButton*>(listItem);
    if(textButton == nullptr)
    {
        textButton = new TextButton();
        textButton->addListener(parent);
    }
    textButton->setButtonText(title);
    textButton->setName(title);
    return textButton;
}

/*
 * Refresh list components.
 */
void SettingsListPage::SettingsList::refresh()
{
    refreshListContent();
}
