#include "Config/MainFile.h"
#include "Config/MainKeys.h"
#include "HomeSettingsPage.h"

/* Localized object class key */
static const juce::Identifier localeClassKey = "HomeSettingsPage";

/* Localized text value keys: */
static const juce::Identifier titleTextKey         = "title";
static const juce::Identifier backgroundTextKey    = "background";
static const juce::Identifier defaultBGOptionKey   = "defaultBGOption";
static const juce::Identifier colorBGOptionKey     = "colorBGOption";
static const juce::Identifier imageBGOptionKey     = "imageBGOption";
static const juce::Identifier hexValueTextKey      = "hexValue";
static const juce::Identifier imagePathTextKey     = "imagePath";
static const juce::Identifier invalidColorTextKey  = "invalidColor";
static const juce::Identifier chooseBGTextKey      = "chooseBG";
static const juce::Identifier chooseBGImageTextKey = "chooseBGImage";
static const juce::Identifier menuTypeTextKey      = "menuType";
static const juce::Identifier scrollingMenuTextKey = "scrollingMenu";
static const juce::Identifier pagedMenuTextKey     = "pagedMenu";
static const juce::Identifier menuColumnsTextKey   = "menuColumns";
static const juce::Identifier menuRowsTextKey      = "menuRows";

HomeSettingsPage::HomeSettingsPage() :
Locale::TextUser(localeClassKey),
PageComponent("HomeSettingsPage"),
title("personalizeTitle", localeText(titleTextKey)),
bgTypeLabel("bgLabel", localeText(backgroundTextKey)),
bgTypePicker("bgTypePicker"),
bgLabel("bgTitle", ""),
bgEditor(localeText(chooseBGTextKey), localeText(chooseBGImageTextKey)),
menuPickerLabel("menuPickerLabel", localeText(menuTypeTextKey)),
menuTypePicker("menuTypePicker"),
columnCountLabel("columnCountLabel", localeText(menuColumnsTextKey)),
rowCountLabel("rowCountLabel", localeText(menuRowsTextKey)),
columnCounter(1, 1, 9),
rowCounter(1, 1, 9)
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("HomeSettingsPage");
#    endif
    setBackButton(PageComponent::leftBackButton);
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(30,
        { 
            RowItem(&title)
        }),
        Row(20,
        {
            RowItem(&bgTypeLabel, 10),
            RowItem(&bgTypePicker, 10)
        }),
        Row(20,
        {
            RowItem(&bgLabel, 10),
            RowItem(&bgEditor, 10)
        }),
        Row(20,
        {
            RowItem(&menuPickerLabel, 10),
            RowItem(&menuTypePicker, 10)
        }),
        Row(20,
        {
            RowItem(&columnCountLabel, 20),
            RowItem(&columnCounter, 10)
        }),
        Row(20,
        {
            RowItem(&rowCountLabel, 20),
            RowItem(&rowCounter, 10)
        })
    });
    layout.setYMarginFraction(0.05);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);

    title.setJustificationType(Justification::centred);
    bgTypePicker.addItem(localeText(defaultBGOptionKey), 1);
    bgTypePicker.addItem(localeText(colorBGOptionKey), 2);
    bgTypePicker.addItem(localeText(imageBGOptionKey), 3);
    bgTypePicker.addListener(this);

    bgEditor.setColour(TextEditor::ColourIds::textColourId,
            Colour::greyLevel(0.f));
    bgEditor.addFileSelectListener(this);

    menuTypePicker.addItem(localeText(scrollingMenuTextKey), 1);
    menuTypePicker.addItem(localeText(pagedMenuTextKey), 2);
    menuTypePicker.addListener(this);
    Config::MainFile mainConfig;
    rowCounter.setValue(mainConfig.getConfigValue<int>
            (Config::MainKeys::pagedMenuWidthKey));

    columnCounter.setValue(mainConfig.getConfigValue<int>
            (Config::MainKeys::pagedMenuHeightKey));

    updateComboBox();
    addAndShowLayoutComponents();
}

/**
 * Update AppMenu dimensions when the page closes.
 */
HomeSettingsPage::~HomeSettingsPage()
{
    // Disabled during AppMenu redesign
    //Config::MainFile mainConfig;
    //mainConfig.setConfigValue<int>(Config::MainKeys::maxRowsKey,
    //                               rowCounter.getValue());
    //mainConfig.setConfigValue<int>(Config::MainKeys::maxColumnsKey,
    //                               columnCounter.getValue());
}

/**
 * Initializes the background and AppMenuType combo boxes with values
 * loaded from the MainConfigFile, and updates their labels to match.
 */
void HomeSettingsPage::updateComboBox()
{
    using juce::String;
    /* Checking the current configuration */
    Config::MainFile mainConfig;
    String background = mainConfig.getConfigValue<String>
        (Config::MainKeys::backgroundKey);
    bool display = false;
    if ((background.length() == 6
         || background.length() == 8)
        && background.containsOnly("0123456789ABCDEF"))
    {
        bgTypePicker.setSelectedItemIndex(1, 
                juce::NotificationType::dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    else if (background.length() > 0)
    {
        bgTypePicker.setSelectedItemIndex(2, 
                juce::NotificationType::dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    bgEditor.setVisible(display);
    bgLabel.setVisible(display);

    String menuType
            = mainConfig.getConfigValue<String>(Config::MainKeys::menuTypeKey);
    int menuIndex = Config::MainKeys::menuTypes.indexOf(menuType);
    if (menuIndex != -1)
    {
        menuTypePicker.setSelectedItemIndex(menuIndex, 
                juce::NotificationType::dontSendNotification);
    }
}

/**
 * If the background type ComboBox is updated, clear the background text
 * field, and update its labels. If the menu type ComboBox is updated,
 * save the changed value to the MainConfigFile
 */
void HomeSettingsPage::comboBoxChanged(juce::ComboBox* box)
{
    using juce::String;
    Config::MainFile mainConfig;
    if (box == &bgTypePicker)
    {
        bgEditor.setText("", 
                juce::NotificationType::dontSendNotification);
        switch (box->getSelectedId())
        {
            case 1:
                mainConfig.setConfigValue<String>
                        (Config::MainKeys::backgroundKey,
                         findColour(PageComponent::backgroundColourId)
                         .toString());
                bgEditor.setVisible(false);
                bgLabel.setVisible(false);
                return;
            case 2:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(hexValueTextKey),
                        juce::NotificationType::dontSendNotification);
                bgEditor.showFileSelectButton(false);
                break;
            case 3:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(imagePathTextKey),
                        juce::NotificationType::dontSendNotification);
                bgEditor.showFileSelectButton(true);
        }
        bgEditor.setVisible(true);
        bgTypeLabel.setVisible(true);
    }
    else if (box == &menuTypePicker && box->getSelectedItemIndex() >= 0)
    {
        mainConfig.setConfigValue<String>
                (Config::MainKeys::menuTypeKey,
                 Config::MainKeys::menuTypes[box->getSelectedItemIndex()]);
    }
}

/**
 * When a value is set in the background editor, attempt to set a new
 * color or image value for the background, depending on the state of
 * bgTypePicker.
 */
void HomeSettingsPage::fileSelected(FileSelectTextEditor * edited)
{
    using juce::String;
    String value = edited->getText();
    Config::MainFile mainConfig;
    //color value
    if (bgTypePicker.getSelectedId() == 2)
    {
        value = value.toUpperCase();
        if (value.length() != 6 || !value.containsOnly("0123456789ABCDEF"))
            bgEditor.setText(localeText(invalidColorTextKey), false);
        else
        {
            mainConfig.setConfigValue<String>
                    (Config::MainKeys::backgroundKey, value);
        }
    }
    else if (bgTypePicker.getSelectedId() == 3)
    {
        mainConfig.setConfigValue<String>
                (Config::MainKeys::backgroundKey, value);
    }
}
