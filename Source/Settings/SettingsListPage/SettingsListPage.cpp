#include "SettingsListPage.h"
#include "Page_Type.h"
#include "Password.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "SettingsListPage";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier homeSettings   = "homeSettings";
    static const juce::Identifier colorSettings  = "colorSettings";
    static const juce::Identifier setPassword    = "setPassword";
    static const juce::Identifier changePassword = "changePassword";
    static const juce::Identifier removePassword = "removePassword";
    static const juce::Identifier inputSettings  = "inputSettings";
}

/* Number of page buttons to show on the screen at a time: */
static const constexpr int buttonsPerPage = 4;

/* List vertical padding fraction: */
static const constexpr float listPaddingFraction = 0.04;

SettingsListPage::SettingsListPage() :
Locale::TextUser(localeClassKey),
pageListener(*this)
{
#if JUCE_DEBUG
    setName("SettingsListPage");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout(
    {
        Row(1,
        {
            RowItem(&buttonList)
        })
    });
    setLayout(layout);
}

/**
 * @brief  Gets button titles for all page buttons.
 *
 * @return  Titles for all page buttons, in order.
 */
juce::StringArray SettingsListPage::getButtonTitles()
{
    juce::StringArray titleList;
    titleList.add(localeText(TextKey::homeSettings));
    titleList.add(localeText(TextKey::colorSettings));
    if (Password::isPasswordSet())
    {
        titleList.add(localeText(TextKey::changePassword));
        titleList.add(localeText(TextKey::removePassword));
    }
    else
    {
        titleList.add(localeText(TextKey::setPassword));
    }
    titleList.add(localeText(TextKey::inputSettings));
    return titleList;
}

/*
 * Reloads page layout whenever the page becomes visible.
 */
void SettingsListPage::visibilityChanged()
{
    if (isVisible())
    {
        buttonList.refresh();
    }
}

/*
 * Handle button clicks to open menu pages, close this page, or 
 * scroll the list of page buttons.
 */
void SettingsListPage::PageListener::buttonClicked(juce::Button * button)
{
    using namespace juce;
    TextButton * textButton = dynamic_cast<TextButton*>(button);
    if(textButton == nullptr)
    {
        DBG("SettingsListPage::" << __func__ << ": invalid button!");
        return;
    }
    
    Array<Page::Type> pageTypes;
    pageTypes.add(Page::Type::homeSettings);
    pageTypes.add(Page::Type::colourSettings);
    pageTypes.add(Page::Type::setPassword);
    if(Password::isPasswordSet())
    {
        pageTypes.add(Page::Type::removePassword);
    }
    pageTypes.add(Page::Type::inputSettings);
    
    StringArray pageNames = listPage.getButtonTitles();
    int typeIndex = pageNames.indexOf(textButton->getButtonText());
    if(typeIndex >= 0 && typeIndex < pageTypes.size())
    {
        listPage.pushPageToStack(pageTypes[typeIndex]);
    }
}

SettingsListPage::SettingsList::SettingsList()
{
    setItemsPerPage(buttonsPerPage);
    setYPaddingFraction(listPaddingFraction);
}
     
/*
 * Gets the total number of page buttons.
 */
unsigned int SettingsListPage::SettingsList::getListSize() const
{
    return Password::isPasswordSet() ? 5 : 4;
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
        textButton->addListener(&parent->pageListener);
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
