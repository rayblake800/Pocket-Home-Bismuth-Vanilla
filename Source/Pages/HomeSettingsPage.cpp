#include "MainConfigFile.h"
#include "HomeSettingsPage.h"

HomeSettingsPage::HomeSettingsPage() :
Localized("HomeSettingsPage"),
PageComponent("HomeSettingsPage",{
    {
        { 3,
            {
                {&title, 1}
            }},
        { 2,
            {
                {&bgTypeLabel, 1},
                {&bgTypePicker, 1}
            }},
        { 2,
            {
                {&bgLabel, 1},
                {&bgEditor, 1}
            }},
        { 2,
            {
                {&menuPickerLabel, 1},
                {&menuTypePicker, 1}
            }},
        { 2,
            {
                {&columnCountLabel, 2},
                {&columnCounter, 1}
            }},
        { 2,
            {
                {&rowCountLabel, 2},
                {&rowCounter, 1}
            }}
    }
}),
title("personalizeTitle", localeText(title_text)),
bgTypeLabel("bgLabel", localeText(background_text)),
bgTypePicker("bgTypePicker"),
bgLabel("bgTitle", ""),
bgEditor(localeText(choose_background), localeText(choose_bg_image)),
menuPickerLabel("menuPickerLabel", localeText(menu_type_text)),
menuTypePicker("menuTypePicker"),
columnCountLabel("columnCountLabel", localeText(menu_columns)),
rowCountLabel("rowCountLabel", localeText(menu_rows)),
columnCounter(1, 1, 9),
rowCounter(1, 1, 9)
{

#    if JUCE_DEBUG
    setName("HomeSettingsPage");
#    endif
    title.setJustificationType(Justification::centred);
    bgTypePicker.addItem(localeText(default_bg), 1);
    bgTypePicker.addItem(localeText(color_bg), 2);
    bgTypePicker.addItem(localeText(image_bg), 3);
    bgTypePicker.addListener(this);

    bgEditor.setColour(TextEditor::ColourIds::textColourId,
            Colour::greyLevel(0.f));
    bgEditor.addFileSelectListener(this);

    for (int i = 0; i < MainConfigFile::menuTypes.size(); i++)
    {
        menuTypePicker.addItem(MainConfigFile::menuTypes[i], i + 1);
    }
    menuTypePicker.addListener(this);
    MainConfigFile mainConfig;
    rowCounter.setValue(mainConfig.getConfigValue<int>
            (MainConfigFile::maxRowsKey));

    columnCounter.setValue(mainConfig.getConfigValue<int>
            (MainConfigFile::maxColumnsKey));

    updateComboBox();
    addAndShowLayoutComponents();
}

/**
 * Update AppMenu dimensions when the page closes.
 */
HomeSettingsPage::~HomeSettingsPage()
{
    MainConfigFile mainConfig;
    mainConfig.setConfigValue<int>(MainConfigFile::maxRowsKey,
                                   rowCounter.getValue());
    mainConfig.setConfigValue<int>(MainConfigFile::maxColumnsKey,
                                   columnCounter.getValue());
}

/**
 * Initializes the background and AppMenuType combo boxes with values
 * loaded from the MainConfigFile, and updates their labels to match.
 */
void HomeSettingsPage::updateComboBox()
{
    /* Checking the current configuration */
    MainConfigFile mainConfig;
    String background
            = mainConfig.getConfigValue<String>(MainConfigFile::backgroundKey);
    bool display = false;
    if ((background.length() == 6
         || background.length() == 8)
        && background.containsOnly("0123456789ABCDEF"))
    {
        bgTypePicker.setSelectedItemIndex(1, dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    else if (background.length() > 0)
    {
        bgTypePicker.setSelectedItemIndex(2, dontSendNotification);
        display = true;
        bgEditor.setText(background, false);
    }
    bgEditor.setVisible(display);
    bgLabel.setVisible(display);

    String menuType
            = mainConfig.getConfigValue<String>(MainConfigFile::menuTypeKey);
    int menuIndex = MainConfigFile::menuTypes.indexOf(menuType);
    if (menuIndex != -1)
    {
        menuTypePicker.setSelectedItemIndex(menuIndex, dontSendNotification);
    }
}

/**
 * If the background type ComboBox is updated, clear the background text
 * field, and update its labels. If the menu type ComboBox is updated,
 * save the changed value to the MainConfigFile
 */
void HomeSettingsPage::comboBoxChanged(ComboBox* box)
{
    MainConfigFile mainConfig;
    if (box == &bgTypePicker)
    {
        bgEditor.setText("", NotificationType::dontSendNotification);
        switch (box->getSelectedId())
        {
            case 1:
                mainConfig.setConfigValue<String>
                        (MainConfigFile::backgroundKey,
                         findColour(PageComponent::backgroundColourId)
                         .toString());
                bgEditor.setVisible(false);
                bgLabel.setVisible(false);
                return;
            case 2:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(bg_color_hex_value),
                        dontSendNotification);
                bgEditor.showFileSelectButton(false);
                break;
            case 3:
                bgLabel.setVisible(true);
                bgLabel.setText(
                        localeText(bg_image_path),
                        dontSendNotification);
                bgEditor.showFileSelectButton(true);
        }
        bgEditor.setVisible(true);
        bgTypeLabel.setVisible(true);
    }
    else if (box == &menuTypePicker && box->getSelectedItemIndex() >= 0)
    {
        mainConfig.setConfigValue<String>
                (MainConfigFile::menuTypeKey,
                 MainConfigFile::menuTypes[box->getSelectedItemIndex()]);
    }
}

/**
 * When a value is set in the background editor, attempt to set a new
 * color or image value for the background, depending on the state of
 * bgTypePicker.
 */
void HomeSettingsPage::fileSelected(FileSelectTextEditor * edited)
{
    String value = edited->getText();
    MainConfigFile mainConfig;
    //color value
    if (bgTypePicker.getSelectedId() == 2)
    {
        value = value.toUpperCase();
        if (value.length() != 6 || !value.containsOnly("0123456789ABCDEF"))
            bgEditor.setText(localeText(invalid_color), false);
        else
        {
            mainConfig.setConfigValue<String>
                    (MainConfigFile::backgroundKey, value);
        }
    }
    else if (bgTypePicker.getSelectedId() == 3)
    {
        mainConfig.setConfigValue<String>
                (MainConfigFile::backgroundKey, value);
    }
}
