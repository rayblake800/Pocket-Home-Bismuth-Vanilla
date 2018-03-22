#include "HomeSettingsPage.h"

HomeSettingsPage::HomeSettingsPage
(PageFactoryInterface& pageFactory, ainConfigFile& mainConfig) :
PageComponent(pageFactory, "HomeSettingsPage",{
    {
        { 3,
            {
                {&title, 1}
            }},
        { 2,
            {
                {&bgTitle, 1},
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
            }},
        { 3,
            {
                {&colourPageBtn, 1}
            }}
    }
}, true),
config(config),
title("personalizeTitle", "Homepage Settings"),
bgTitle("bgTitle", "Background:"),
bgTypePicker("bgTypePicker"),
bgLabel("bgLabel", ""),
bgEditor("Choose the new background",
        "Please choose your new background image"),
menuPickerLabel("menuPickerLabel", "Application menu:"),
menuTypePicker("menuTypePicker"),
columnCountLabel("columnCountLabel", "Menu columns:"),
rowCountLabel("rowCountLabel", "Menu rows:"),
columnCounter(1, 1, 9),
rowCounter(1, 1, 9),
colourPageBtn("Set colors")
{

#    if JUCE_DEBUG
    setName("HomeSettingsPage");
#    endif
    title.setJustificationType(Justification::centred);
    bgTypePicker.addItem("Default", 1);
    bgTypePicker.addItem("Color", 2);
    bgTypePicker.addItem("Image", 3);
    bgTypePicker.addListener(this);

    bgEditor.setColour(TextEditor::ColourIds::textColourId,
            Colour::greyLevel(0.f));
    bgEditor.addFileSelectListener(this);

    for (int i = 0; i < MainConfigFile::menuTypes.size(); i++)
    {
        menuTypePicker.addItem(MainConfigFile::menuTypes[i], i + 1);
    }
    menuTypePicker.addListener(this);
    
    rowCounter.setValue(config.getConfigValue<int>
            (MainConfigFile::maxRowsKey));

    columnCounter.setValue(config.getConfigValue<int>
            (MainConfigFile::maxColumnsKey));

    colourPageBtn.addListener(this);

    updateComboBox();
    addAndShowLayoutComponents();
}

/**
 * Update AppMenu dimensions when the page closes.
 */
void HomeSettingsPage::pageRemovedFromStack()
{
    
    config.setConfigValue<int>(MainConfigFile::maxRowsKey,
                               rowCounter.getValue());
    config.setConfigValue<int>(MainConfigFile::maxColumnsKey,
                               columnCounter.getValue());
}

void HomeSettingsPage::updateComboBox()
{
    
    /* Checking the current configuration */
    String background = config.getConfigValue<String>(MainConfigFile::backgroundKey);
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

    String menuType = config.getConfigValue<String>(MainConfigFile::menuTypeKey);
    int menuIndex = MainConfigFile::menuTypes.indexOf(menuType);
    if (menuIndex != -1)
    {
        menuTypePicker.setSelectedItemIndex(menuIndex, dontSendNotification);
    }
}

void HomeSettingsPage::comboBoxChanged(ComboBox * box)
{
    if (box == &bgTypePicker)
    {
        bgEditor.setText("", NotificationType::dontSendNotification);
        switch (box->getSelectedId())
        {
            case 1:
                config.setConfigValue<String>(MainConfigFile::backgroundKey,
                                              "4D4D4D");
                bgEditor.setVisible(false);
                bgLabel.setVisible(false);
                return;
            case 2:
                bgLabel.setText("Hex value:", dontSendNotification);
                bgEditor.showFileSelectButton(false);
                break;
            case 3:
                bgLabel.setText("Image path:", dontSendNotification);
                bgEditor.showFileSelectButton(true);
        }
        bgEditor.setVisible(true);
        bgLabel.setVisible(true);
    }
    else if (box == &menuTypePicker && box->getSelectedItemIndex() >= 0)
    {
        config.setConfigValue<String>
                (MainConfigFile::menuTypeKey,
                 MainConfigFile::menuTypes[box->getSelectedItemIndex()]);
    }
}

void HomeSettingsPage::pageButtonClicked(Button* button)
{
    if (button == &colourPageBtn)
    {
        pushPageToStack(PageComponent::PageType::Colour);
    }
}

void HomeSettingsPage::fileSelected(FileSelectTextEditor * edited)
{
    String value = edited->getText();
    //color value
    if (bgTypePicker.getSelectedId() == 2)
    {
        value = value.toUpperCase();
        if (value.length() != 6 || !value.containsOnly("0123456789ABCDEF"))
            bgEditor.setText("Invalid color", false);
        else
        {
            config.setConfigValue<String>(MainConfigFile::backgroundKey, value);
        }
    }
    else if (bgTypePicker.getSelectedId() == 3)
    {
        config.setConfigValue<String>(MainConfigFile::backgroundKey, value);
    }
}
