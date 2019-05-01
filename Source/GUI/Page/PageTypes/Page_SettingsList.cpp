#include "Page_SettingsList.h"
#include "Page_Type.h"
#include "Password.h"

#ifdef JUCE_DEBUG
// Print the full class name before all debug output:
static const constexpr char* dbgPrefix = "Page::SettingsList::";
#endif

// Localized object class key:
static const juce::Identifier localeClassKey = "Page::SettingsList";

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier homeSettings   = "homeSettings";
    static const juce::Identifier colourSettings = "colourSettings";
    static const juce::Identifier setPassword    = "setPassword";
    static const juce::Identifier changePassword = "changePassword";
    static const juce::Identifier removePassword = "removePassword";
    static const juce::Identifier inputSettings  = "inputSettings";
}


// Number of page buttons to show on the screen at a time:
static const constexpr int buttonsPerPage = 4;

// List vertical padding fraction:
static const constexpr float listPaddingFraction = 0.04;

// Sets the page layout on construction.
Page::SettingsList::SettingsList() :
Locale::TextUser(localeClassKey),
pageListener(*this),
buttonList(*this)
{
#if JUCE_DEBUG
    setName("Page::SettingsList");
#endif
    setBackButton(BackButtonType::left);
    updateButtonTitles();
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


// Updates the list of button titles, refreshing the list if needed.
void Page::SettingsList::updateButtonTitles()
{
    if (pageTitles.isEmpty())
    {
        pageTitles.add(localeText(TextKey::homeSettings));
        pageTitles.add(localeText(TextKey::colourSettings));
        pageTitles.add(localeText(TextKey::inputSettings));
        passwordSet = Password::isPasswordSet();
        if (passwordSet)
        {
            pageTitles.add(localeText(TextKey::changePassword));
            pageTitles.add(localeText(TextKey::removePassword));
        }
        else
        {
            pageTitles.add(localeText(TextKey::setPassword));
        }
        buttonList.refresh();
    }
    else if (Password::isPasswordSet() != passwordSet)
    {
        passwordSet = !passwordSet;
        if (passwordSet)
        {
            pageTitles.removeString(localeText(TextKey::setPassword));
            pageTitles.add(localeText(TextKey::changePassword));
            pageTitles.add(localeText(TextKey::removePassword));
        }
        else
        {
            pageTitles.removeString(localeText(TextKey::changePassword));
            pageTitles.removeString(localeText(TextKey::removePassword));
            pageTitles.add(localeText(TextKey::setPassword));
        }
        buttonList.refresh();
    }
}


// Checks if the list should be updated when the page is revealed.
void Page::SettingsList::pageRevealedOnStack()
{
    updateButtonTitles();
}


// Opens new pages when their buttons are clicked.
void Page::SettingsList::PageListener::buttonClicked(juce::Button * button)
{
    using juce::TextButton;
    TextButton * textButton = dynamic_cast<TextButton*>(button);
    if (textButton == nullptr)
    {
        DBG(dbgPrefix << "PageListener::" << __func__ << ": invalid button!");
        jassertfalse;
        return;
    }

    const std::map<juce::String, Type> typeMap =
    {
        {listPage.localeText(TextKey::homeSettings), Type::homeSettings},
        {listPage.localeText(TextKey::colourSettings), Type::colourSettings},
        {listPage.localeText(TextKey::inputSettings), Type::inputSettings},
        {listPage.localeText(TextKey::setPassword), Type::setPassword},
        {listPage.localeText(TextKey::changePassword), Type::setPassword},
        {listPage.localeText(TextKey::removePassword), Type::removePassword}
    };

    auto searchIter = typeMap.find(textButton->getButtonText());
    if (searchIter != typeMap.end())
    {
        listPage.pushPageToStack(searchIter->second);
    }
}

Page::SettingsList::PageList::PageList(SettingsList& page) : page(page)
{
    setItemsPerPage(buttonsPerPage);
    setYPaddingFraction(listPaddingFraction);
}


// Gets the total number of page buttons.
unsigned int Page::SettingsList::PageList::getListSize() const
{
    return page.pageTitles.size();
}


// Updates or creates a page TextButton for a specific button index.
juce::Component* Page::SettingsList::PageList::updateListItem
(juce::Component* listItem, const unsigned int index)
{
    juce::String title = page.pageTitles[index];
    juce::TextButton * textButton;
    if (listItem == nullptr)
    {
        textButton = new juce::TextButton;
        textButton->addListener(&page.pageListener);
    }
    else
    {
        textButton = static_cast<juce::TextButton*>(listItem);
    }
    textButton->setButtonText(title);
    textButton->setName(title);
    return textButton;
}


// Refresh list components.
void Page::SettingsList::PageList::refresh()
{
    refreshListContent();
}
