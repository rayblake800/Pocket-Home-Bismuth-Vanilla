#include "../PocketHomeApplication.h"
#include "PersonalizePage.h"

PersonalizePage::PersonalizePage() :
bgColor(0xffd23c6d),
backButton(ComponentConfigFile::pageLeftKey),
bgTitle("bgTitle", "Background"),
bgTypePicker("bgTypePicker"),
bgLabel("bgLabel", "", 2),
bgEditor("Choose the new background",
"Please choose your new background image"),
menuPickerLabel("menuPickerLabel", "Application menu type:"),
menuTypePicker("menuTypePicker")
{
    std::vector<GridLayoutManager::ComponentLayoutParams> layout = {
        {&bgTitle, 0, 1},
        {&bgTypePicker, 0, 1},
        {&bgLabel, 1, 1},
        {&bgEditor, 1, 1},
        {&menuPickerLabel, 2, 1},
        {&menuTypePicker, 2, 1}
    };
    layoutManager.addComponents(layout, this);

    backButton.addListener(this);
    addAndMakeVisible(backButton);

    bgTypePicker.addItem("Default", 1);
    bgTypePicker.addItem("Color", 2);
    bgTypePicker.addItem("Image", 3);
    bgTypePicker.setSelectedId(1);
    bgTypePicker.addListener(this);

    bgEditor.setColour(TextEditor::ColourIds::textColourId,
            Colour::greyLevel(0.f));
    bgEditor.addFileSelectListener(this);

    menuTypePicker.addItem("Scrolling menu", 1);
    menuTypePicker.setSelectedId(1);
    menuTypePicker.addListener(this);
    updateComboBox();
}

PersonalizePage::~PersonalizePage()
{
}

void PersonalizePage::updateComboBox()
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    /* Checking the current configuration */
    String background = config.getConfigString(MainConfigFile::backgroundKey);
    bool display = false;
    if (background.length() == 0);
    else if (background.length() == 6 &&
            background.containsOnly("0123456789ABCDEF"))
    {
        bgTypePicker.setSelectedItemIndex(1, sendNotificationSync);
        display = true;
        bgEditor.setText(background, false);
    } else
    {
        bgTypePicker.setSelectedItemIndex(2, sendNotificationSync);
        display = true;
        bgEditor.setText(background, false);
    }

    bgEditor.setVisible(display);
    bgLabel.setVisible(display);
}

void PersonalizePage::paint(Graphics &g)
{
    g.fillAll(bgColor);
}

void PersonalizePage::resized()
{
    backButton.applyConfigBounds();
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(backButton.getBounds().getWidth(), bounds.getHeight()/10);
    layoutManager.layoutComponents(bounds, bounds.getWidth() / 100,
            bounds.getWidth() / 20);
}

void PersonalizePage::buttonClicked(Button* button)
{
    if (button == &backButton)
    {
        PocketHomeApplication::getInstance()->getMainStack()
                .popPage(PageStackComponent::kTransitionTranslateHorizontal);
        updateComboBox();
    }
}

void PersonalizePage::comboBoxChanged(ComboBox* box)
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    if (box == &bgTypePicker)
    {
        switch (box->getSelectedId())
        {
            case 1:
                config.setConfigString(MainConfigFile::backgroundKey, "4D4D4D");
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
    else if (box == &menuTypePicker){
        config.setConfigString(MainConfigFile::menuTypeKey,box->getText());
    }
}

void PersonalizePage::fileSelected(FileSelectTextEditor* edited)
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    String value = edited->getText();
    //color value
    if (bgTypePicker.getSelectedId() == 2)
    {
        value = value.toUpperCase();
        if (value.length() != 6 || !value.containsOnly("0123456789ABCDEF"))
            bgEditor.setText("Invalid color", false);
        else
        {
            config.setConfigString(MainConfigFile::backgroundKey, value);
        }
    } else if (bgTypePicker.getSelectedId() == 3)
    {
        config.setConfigString(MainConfigFile::backgroundKey, value);
    }
}