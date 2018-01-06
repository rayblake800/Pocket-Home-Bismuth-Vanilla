#include "../PocketHomeApplication.h"
#include "DateTimePage.h"

const Colour DateTimePage::bgColour = Colour(0xffd23c6d);
const String DateTimePage::pageTitle = "Date and time settings";

const String DateTimePage::clockModeLabelText
        = "Select the display mode for the clock:";
const String DateTimePage::clockMode24h = "24h mode";
const String DateTimePage::clockModeAmPm = "AM/PM mode";
const String DateTimePage::clockModeNoShow = "Don't show clock";

const String DateTimePage::reconfigureBtnText = "Reconfigure system clock";
const String DateTimePage::reconfigureCommand
        = " 'sudo dpkg-reconfigure tzdata ; exit'";
const String DateTimePage::reconfErrorTitle = "Failed to launch terminal:";
const String DateTimePage::reconfErrorPreCmd = "Running '";
const String DateTimePage::reconfErrorPostCmd
        = "' failed.\nIs the terminal launch command set correctly?";

DateTimePage::DateTimePage() :
titleLabel("dateTimeTitleLabel", pageTitle),
setClockMode("setClockMode"),
reconfigureBtn(reconfigureBtnText),
clockModeLabel("clockModeLabel", clockModeLabelText)
{
    //Title font
    titleLabel.setFont(Font(27.f));
    clockModeLabel.setFont(Font(20.f));
    //Back button
    backButton = createImageButton("Back",
            createImageFromFile(assetFile("backIcon.png")));
    backButton->addListener(this);
    backButton->setAlwaysOnTop(true);
    //ComboBox
    setClockMode.addItem(clockMode24h, 1);
    setClockMode.addItem(clockModeAmPm, 2);
    setClockMode.addItem(clockModeNoShow, 3);
    setClockMode.addListener(this);

    //Let's check whether there is an option for time format in the config
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    if (config.getConfigBool(MainConfigFile::showClockKey))
    {
        if (config.getConfigBool(MainConfigFile::use24HrModeKey))
        {
            setClockMode.setSelectedId(1);
        } else
        {
            setClockMode.setSelectedId(2);
        }
    } else
    {
        setClockMode.setSelectedId(3);
    }

    reconfigureBtn.addListener(this);

    addAndMakeVisible(clockModeLabel);
    addAndMakeVisible(reconfigureBtn);
    addAndMakeVisible(setClockMode);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(backButton);
}

DateTimePage::~DateTimePage()
{
}

void DateTimePage::buttonClicked(Button* but)
{
    if (but == backButton)
        PocketHomeApplication::getInstance()->getMainStack()
        .popPage(PageStackComponent::kTransitionTranslateHorizontal);
    else if (but == &reconfigureBtn)
    {
        String configureTime = PocketHomeApplication::getInstance()->getConfig()
                .getConfigString(MainConfigFile::termLaunchCommandKey)
                + reconfigureCommand;
        int ret = system(configureTime.toRawUTF8());
        if (ret != 0)
            AlertWindow::showMessageBox(AlertWindow::WarningIcon,
                reconfErrorTitle,
                reconfErrorPreCmd + configureTime + reconfErrorPostCmd);
    }
}

void DateTimePage::comboBoxChanged(ComboBox* c)
{
    if (c != &setClockMode) return;
    MainConfigFile& config = PocketHomeApplication::getInstance()->getConfig();
    bool showClock = (c->getSelectedId() != 3);
    bool use24HrMode = (c->getSelectedId() == 1);
    if (showClock)
    {
        config.setConfigBool(MainConfigFile::use24HrModeKey, use24HrMode);
    }
    config.setConfigBool(MainConfigFile::showClockKey, showClock);
}

void DateTimePage::paint(Graphics& g)
{
    auto bounds = getLocalBounds();
    g.fillAll(bgColour);
}

void DateTimePage::resized()
{
    auto bounds = getLocalBounds();
    int btn_height = 30;
    int btn_width = 345;

    int titlewidth = titleLabel.getFont().getStringWidth(titleLabel.getText());
    titlewidth /= 2;
    titleLabel.setBounds(bounds.getX() + 240 - titlewidth, bounds.getY() + 10,
            btn_width, btn_height);

    backButton->setBounds(bounds.getX(), bounds.getY(), 60, bounds.getHeight());

    int datewidth = clockModeLabel.getFont()
            .getStringWidth(clockModeLabel.getText());
    clockModeLabel.setBounds(bounds.getX() + 60, bounds.getY() + 70, datewidth,
            btn_height);
    int combowidth = 360 - datewidth;
    setClockMode.setBounds(bounds.getX() + 60 + datewidth, bounds.getY() + 70,
            combowidth, btn_height);

    int middle = 240 - btn_width / 2;
    reconfigureBtn.setBounds(bounds.getX() + middle, bounds.getY() + 150,
            btn_width, btn_height);
}
