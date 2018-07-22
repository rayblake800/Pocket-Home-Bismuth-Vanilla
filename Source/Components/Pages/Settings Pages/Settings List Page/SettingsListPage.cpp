#include "SettingsListPage.h"
#include "Password.h"

SettingsListPage::SettingsListPage() :
Localized("SettingsListPage"),
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
StringArray SettingsListPage::getButtonTitles()
{
    StringArray titleList;
    titleList.add(localeText(personalize_homepage));
    if (Password::isPasswordSet())
    {
        titleList.add(localeText(change_password));
        titleList.add(localeText(remove_password));
    }
    else
    {
        titleList.add(localeText(set_password));
    }
    titleList.add(localeText(date_and_time));
    titleList.add(localeText(input_settings));
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
void SettingsListPage::pageButtonClicked(Button * button)
{
    TextButton * textButton = dynamic_cast<TextButton*>(button);
    if(textButton == nullptr)
    {
        DBG("SettingsListPage::" << __func__ << ": invalid button!");
        return;
    }
    
    Array<PageComponent::PageType> pageTypes;
    pageTypes.add(PageComponent::PageType::HomeSettings);
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
    return Password::isPasswordSet() ? 5 : 4;
}

/*
 * Updates or creates a page TextButton for a specific button index.
 */
Component* SettingsListPage::SettingsList::updateListItem
(Component* listItem, unsigned int index)
{
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