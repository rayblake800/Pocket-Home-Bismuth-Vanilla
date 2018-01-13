#include "../PocketHomeApplication.h"
#include "../GridLayoutManager.h"
#include "PersonalizePageComponent.h"
//TODO: Re-organize this into something that works with AppMenus and
//isn't so messy

/* Personalize class */
PersonalizePageComponent::PersonalizePageComponent(AppConfigFile& appConfig) :
appConfig(appConfig),
bgTitle("bgTitle", "Background"),
iconTitle("lab_icons", "Icon management"),
bgLabel("bgLabel", ""),
nameLabel("nameLabel", "Name:"),
iconLabel("iconLabel", "Icon path:"),
shellLabel("shellLabel", "Command:"),
addFaveAppBtn("Add"),
applyChangesBtn("Apply Changes"),
bgTypePicker("bgTypePicker"),
bgEditor("Choose the new background",
"Please choose your new background image"),
appNameEditor("name"),
appIconEditor("Choose the icon",
"Please choose your icon image (ideal size : 90x70 px)"),
launchCmdEditor("shell"), successLabel("suc", "Success !")
{
    bgColor = Colour(0xffd23c6d);
    bgImage = createImageFromFile(assetFile("settingsBackground.png"));
    Font big_font(Font::getDefaultMonospacedFontName(), 25.f, 0);
    bgTitle.setFont(big_font);
    iconTitle.setFont(big_font);

    Font small_font(Font::getDefaultMonospacedFontName(), 17.5f, 0);
    bgLabel.setFont(small_font);
    nameLabel.setFont(small_font);
    iconLabel.setFont(small_font);
    shellLabel.setFont(small_font);
    successLabel.setFont(big_font);

    addAndMakeVisible(bgTitle);
    addAndMakeVisible(iconTitle);
    addAndMakeVisible(addFaveAppBtn);
    addAndMakeVisible(applyChangesBtn);
    addAndMakeVisible(successLabel);
    applyChangesBtn.addListener(this);
    addFaveAppBtn.addListener(this);
    /* ComboBox */
    bgTypePicker.addItem("Default", 1);
    bgTypePicker.addItem("Color", 2);
    bgTypePicker.addItem("Image", 3);
    bgTypePicker.setSelectedId(1);
    bgTypePicker.addListener(this);
    addAndMakeVisible(bgTypePicker);
    //////////////
    /* + */
    bgEditor.setColour(TextEditor::ColourIds::textColourId,
            Colour::greyLevel(0.f));
    addAndMakeVisible(bgEditor);
    addAndMakeVisible(bgLabel);
    addAndMakeVisible(nameLabel);
    addAndMakeVisible(iconLabel);
    addAndMakeVisible(shellLabel);
    addAndMakeVisible(appNameEditor);
    addAndMakeVisible(appIconEditor);
    addAndMakeVisible(launchCmdEditor);


    showAddComponents(false);
    successLabel.setVisible(false);

    /* Create back button */
    backButton = createImageButton("Back", createImageFromFile
            (assetFile("backIcon.png")));
    backButton->addListener(this);
    backButton->setAlwaysOnTop(true);
    addAndMakeVisible(backButton);

    updateComboBox();
}

PersonalizePageComponent::~PersonalizePageComponent()
{
}

void PersonalizePageComponent::updateComboBox()
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
        bgEditor.setText(background);
    } else
    {
        bgTypePicker.setSelectedItemIndex(2, sendNotificationSync);
        display = true;
        bgEditor.setText(background);
    }

    bgEditor.setVisible(display);
    bgLabel.setVisible(display);
}

void PersonalizePageComponent::paint(Graphics &g)
{
    auto bounds = getLocalBounds();
    g.fillAll(bgColor);
    g.drawImage(bgImage, bounds.getX(), bounds.getY(),
            bounds.getWidth(), bounds.getHeight(),
            0, 0, bgImage.getWidth(), bgImage.getHeight(), false);
}

void PersonalizePageComponent::resized()
{
    auto bounds = getLocalBounds();
    backButton->setBounds(bounds.withWidth(bounds.getWidth() / 8));

    bounds.setLeft(backButton->getBounds().getRight());

    GridLayoutManager layoutManager;
    //Row 0:
    layoutManager.addComponent(&bgTitle, 0, 1);
    layoutManager.addComponent(&bgTypePicker, 0, 1);

    //Row 1:
    layoutManager.addComponent(&bgLabel, 1, 1);
    layoutManager.addComponent(&bgEditor, 1, 2);

    //Row 2: 
    layoutManager.addComponent(&iconTitle, 2, 2);
    layoutManager.addComponent(&addFaveAppBtn, 2, 1);
    layoutManager.addComponent(&successLabel, 2, 1);

    //Row 3: 
    layoutManager.addComponent(&nameLabel, 3, 1);
    layoutManager.addComponent(&appNameEditor, 3, 1);
    layoutManager.addComponent(&iconLabel, 3, 1);
    layoutManager.addComponent(&appIconEditor, 3, 1);

    //Row 4:
    layoutManager.addComponent(&shellLabel, 4, 1);
    layoutManager.addComponent(&launchCmdEditor, 4, 5);

    //Row 5:
    layoutManager.addComponent(&applyChangesBtn, 5, 1);

    layoutManager.layoutComponents(bounds, 4,15);
}

void PersonalizePageComponent::buttonClicked(Button* button)
{
    if (button == backButton)
    {
        PocketHomeApplication::getInstance()->getMainStack()
                .popPage(PageStackComponent::kTransitionTranslateHorizontal);
        resetApplySuccess();
        updateComboBox();
    } else if (button == &applyChangesBtn)
    {
        bool ok = updateJSON();
    } else if (button == &addFaveAppBtn)
    {
        showAddComponents(true);
        successLabel.setVisible(false);
        applyChangesBtn.setVisible(true);
        appNameEditor.setText("");
        appIconEditor.setText("");
        launchCmdEditor.setText("");
    }
}

void PersonalizePageComponent::showAddComponents(bool show)
{
    nameLabel.setVisible(show);
    iconLabel.setVisible(show);
    shellLabel.setVisible(show);
    appNameEditor.setVisible(show);
    appIconEditor.setVisible(show);
    launchCmdEditor.setVisible(show);
}

void PersonalizePageComponent::comboBoxChanged(ComboBox* box)
{
    //Default background
    if (box->getSelectedId() == 1)
    {
        bgEditor.setVisible(false);
        bgLabel.setVisible(false);
        return;
    }        //Color background
    else if (box->getSelectedId() == 2)
    {
        bgLabel.setText("Hex value:", dontSendNotification);
        bgEditor.showFileSelectButton(false);
    }        //Image background
    else if (box->getSelectedId() == 3)
    {
        bgLabel.setText("Image path:", dontSendNotification);
        bgEditor.showFileSelectButton(true);
    }
    bgEditor.setVisible(true);
    bgLabel.setVisible(true);
}

bool PersonalizePageComponent::updateJSON()
{
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    bool name_b = false;
    bool color_b = false;
    if (appNameEditor.isVisible())
    {
        String name = appNameEditor.getText();
        String icon = appIconEditor.getText();
        String shell = launchCmdEditor.getText();
        AppConfigFile::AppItem newFavorite;
        newFavorite.name = name;
        newFavorite.icon = icon;
        newFavorite.shell = shell;
        appConfig.addFavoriteApp(newFavorite, -1);
        name_b = true;

        /* Adding to the grid */
    }
    if (bgTypePicker.getSelectedId() == 1)
    {
        config.setConfigString(MainConfigFile::backgroundKey, "4D4D4D");
    }
    if (bgTypePicker.getSelectedId() == 2)
    {
        String value = bgEditor.getText().toUpperCase();
        if (value.length() != 6 || !value.containsOnly("0123456789ABCDEF"))
            bgEditor.setText("Invalid color");
        else
        {
            config.setConfigString(MainConfigFile::backgroundKey, value);
            color_b = true;

            /* Change background in LauncherComponent */
        }
    }
    if (bgTypePicker.getSelectedId() == 3)
    {
        String value = bgEditor.getText();
        config.setConfigString(MainConfigFile::backgroundKey, value);
        color_b = true;

        /* Change background in LauncherComponent */
    }
    successLabel.setVisible(true);
    return name_b || color_b;
}

void PersonalizePageComponent::resetApplySuccess()
{
    showAddComponents(false);
    successLabel.setVisible(false);
    bgEditor.setVisible(false);
    bgEditor.setText("");
    appNameEditor.setText("");
    appIconEditor.setText("");
    launchCmdEditor.setText("");
    applyChangesBtn.setVisible(true);
}
