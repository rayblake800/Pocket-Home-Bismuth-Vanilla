#include "Config_MainFile.h"
#include "Config_MainKeys.h"
#include "AppMenu_ConfigFile.h"
#include "AppMenu_Format.h"
#include "HomeSettingsPage.h"

/* Localized object class key */
static const juce::Identifier localeClassKey = "HomeSettingsPage";

/* Localized text value keys: */
namespace TextKey
{
    static const juce::Identifier title           = "title";
    static const juce::Identifier background      = "background";
    static const juce::Identifier defaultBGOption = "defaultBGOption";
    static const juce::Identifier colorBGOption   = "colorBGOption";
    static const juce::Identifier imageBGOption   = "imageBGOption";
    static const juce::Identifier hexValue        = "hexValue";
    static const juce::Identifier imagePath       = "imagePath";
    static const juce::Identifier invalidColor    = "invalidColor";
    static const juce::Identifier chooseBG        = "chooseBG";
    static const juce::Identifier chooseBGImage   = "chooseBGImage";
}

/* Background selection menu indices: */
static const constexpr int defaultBackgroundID = 1;
static const constexpr int colorBackgroundID   = 2;
static const constexpr int imageBackgroundID   = 3;

/*
 * Initializes the page layout.
 */
HomeSettingsPage::HomeSettingsPage() :
Locale::TextUser(localeClassKey),
title("personalizeTitle", localeText(TextKey::title)),
bgTypeLabel("bgLabel", localeText(TextKey::background)),
bgTypePicker("bgTypePicker"),
bgLabel("bgTitle", ""),
bgEditor(localeText(TextKey::chooseBG), localeText(TextKey::chooseBGImage))
{
#if JUCE_DEBUG
    setName("HomeSettingsPage");
#endif
    setBackButton(BackButtonType::left);
    using namespace Layout::Group;
    RelativeLayout layout({
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
            RowItem(menuComponents.getMenuFormatLabel(), 10),
            RowItem(menuComponents.getMenuFormatPicker(), 10)
        }),
        Row(20,
        {
            RowItem(menuComponents.getRowCountLabel(), 20),
            RowItem(menuComponents.getRowCounter(), 10)
        }),
        Row(20,
        {
            RowItem(menuComponents.getColumnCountLabel(), 20),
            RowItem(menuComponents.getColumnCounter(), 10)
        })
    });
    layout.setYMarginFraction(0.05);
    layout.setXPaddingWeight(1);
    layout.setYPaddingWeight(3);
    setLayout(layout);

    title.setJustificationType(juce::Justification::centred);
    bgTypePicker.addItem(localeText(TextKey::defaultBGOption),
            defaultBackgroundID);
    bgTypePicker.addItem(localeText(TextKey::colorBGOption),
            colorBackgroundID);
    bgTypePicker.addItem(localeText(TextKey::imageBGOption), 
            imageBackgroundID);
    bgTypePicker.addListener(this);

    bgEditor.setColour(juce::TextEditor::ColourIds::textColourId,
            juce::Colour::greyLevel(0.f));
    bgEditor.addFileSelectListener(this);

    updateComboBox();
    addAndShowLayoutComponents();
    menuComponents.updateForCurrentSettings();
}

/*
 * Updates AppMenu settings when the page closes.
 */
HomeSettingsPage::~HomeSettingsPage()
{
    menuComponents.applySettingsChanges();
}

/**
 * @brief  Checks if a string is a valid representation of a hexadecimal RGB
 *         or ARGB color value.
 *
 * @param colorString  The potential color string.
 *
 * @return             Whether colorString represents a valid color value.
 */
static bool validColorString(const juce::String colorString)
{
    const int length = colorString.length();
    if(length != 6 && length != 8 && length != 10)
    {
        return false;
    }
    int charIndex = 0;
    if(colorString.startsWithIgnoreCase("0x"))
    {
        if(length == 6)
        {
            return false;
        }
        charIndex = 2;
    }
    for(; charIndex < length; charIndex++)
    {
        char c = colorString[charIndex];
        if(!((c >= '0' && c <= '9')
          || (c >= 'a' && c <= 'f')
          || (c >= 'A' && c <= 'F')))
        {
            return false;
        }
    }
    return true;
}

/*
 * Initializes the background combo box with values loaded from the main config 
 * file, and updates its labels to match.
 */
void HomeSettingsPage::updateComboBox()
{
    using juce::String;
    /* Checking the current configuration */
    Config::MainFile mainConfig;
    String background = mainConfig.getHomeBackground();
    bool display = false;
    if (validColorString(background))
    {
        bgTypePicker.setSelectedItemIndex(colorBackgroundID - 1, 
                juce::NotificationType::dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    else if (background.length() > 0)
    {
        bgTypePicker.setSelectedItemIndex(imageBackgroundID - 1, 
                juce::NotificationType::dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    else
    {
        bgTypePicker.setSelectedItemIndex(defaultBackgroundID - 1,
                juce::NotificationType::dontSendNotification);
    }
    bgEditor.setVisible(display);
    bgLabel.setVisible(display);
}


/*
 * Handles background ComboBox selections.
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
            case defaultBackgroundID:
                mainConfig.setHomeBackground(
                         findColour(Page::Component::backgroundColourId)
                         .toString());
                bgEditor.setVisible(false);
                bgLabel.setVisible(false);
                return;
            case colorBackgroundID:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(TextKey::hexValue),
                        juce::NotificationType::dontSendNotification);
                bgEditor.showFileSelectButton(false);
                break;
            case imageBackgroundID:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(TextKey::imagePath),
                        juce::NotificationType::dontSendNotification);
                bgEditor.showFileSelectButton(true);
        }
        bgEditor.setVisible(true);
        bgTypeLabel.setVisible(true);
    }
}

/*
 * When a value is set in the background editor, attempt to set a new color or 
 * image value for the background, depending on the state of bgTypePicker.
 */
void HomeSettingsPage::fileSelected(FileSelectTextEditor * edited)
{
    juce::String value = edited->getText();
    Config::MainFile mainConfig;
    if(bgTypePicker.getSelectedId() == colorBackgroundID)
    {
        if(validColorString(value))
        {
            bgEditor.setText(localeText(TextKey::invalidColor), false);
        }
        else
        {
            mainConfig.setHomeBackground(value);
        }
    }
    else if(bgTypePicker.getSelectedId() == imageBackgroundID)
    {
        mainConfig.setHomeBackground(value);
    }
}
