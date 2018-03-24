#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "I2CBus.h"
#include "FelPage.h"

FelPage::FelPage(ComponentConfigFile& config) :
Localized("FelPage"),
PageComponent(config,"FelPage",{
    {2,
        {
            {&infoLine1, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&yesButton, 2},
            {nullptr, 1}
        }},
    {1,
        {
            {nullptr, 1}
        }},
    {2,
        {
            {nullptr, 1},
            {&noButton, 2},
            {nullptr, 1}
        }},
    {1,
        {
            {nullptr, 1}
        }},
    {1,
        {
            {&infoLine2, 1}
        }}
},nullptr,false),
debounce(false),
infoLine1(config,"infoLine1", localeText(ask_to_reboot)),
yesButton(localeText(yes_btn)),
noButton(localeText(no_btn)),
infoLine2(config,"infoLine2", localeText(flashing_info))
{

#    if JUCE_DEBUG
    setName("FelPage");
#    endif
    //TODO: configurable page backgrounds
    setColour(backgroundColourId, Colours::black);
    infoLine1.setJustificationType(Justification::centred);
    infoLine2.setJustificationType(Justification::centred);
    yesButton.addListener(this);
    noButton.addListener(this);
    addAndShowLayoutComponents();
}

/**
 * Handle button clicks, either restarting into Fel mode or closing the 
 * page.
 */
void FelPage::pageButtonClicked(Button* button)
{
    if (button == &noButton)
    {
        removeFromStack(Animation::slideInFromLeft);
    }
    else if (button == &yesButton && !debounce)
    {
        debounce = true;
        try
        {
            I2CBus i2c;
            i2c.enableFelMode();
            ChildProcess c;
            MainConfigFile config;
            c.start(config.getConfigValue<String>
                    (MainConfigFile::restartCommandKey));
            c.waitForProcessToFinish(10000);
        }
        catch (I2CBus::I2CException e)
        {
            DBG("FelPage::" << __func__ << e.getErrorMessage());
        }
    }
}