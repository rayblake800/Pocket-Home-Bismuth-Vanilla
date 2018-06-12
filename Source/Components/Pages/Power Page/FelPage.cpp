#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "I2CBus.h"
#include "FelPage.h"

FelPage::FelPage() :
Localized("FelPage"),
PageComponent("FelPage"),
debounce(false),
infoLine1("infoLine1", localeText(ask_to_reboot)),
yesButton(localeText(yes_btn)),
noButton(localeText(no_btn)),
infoLine2("infoLine2", localeText(flashing_info))
{

#    if JUCE_DEBUG
    setName("FelPage");
#    endif
    setLayout({
        .xMarginFraction = 0,
        .yMarginFraction = 0.1,
        .rows =
        {
            {
                .rowWeight = 20,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = &infoLine1,
                        .componentWeight = 10,
                        .xPaddingWeight = 2
                    }
                }
            },
            {
                .rowWeight = 20,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = &yesButton,
                        .componentWeight = 20,
                        .xPaddingWeight = 20
                    }
                }
            },
            {
                .rowWeight = 10,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = nullptr,
                        .componentWeight = 10,
                        .xPaddingWeight = 2
                    }
                }
            },
            {
                .rowWeight = 20,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = &noButton,
                        .componentWeight = 20,
                        .xPaddingWeight = 20
                    }
                }
            },
            {
                .rowWeight = 10,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = nullptr,
                        .componentWeight = 10,
                        .xPaddingWeight = 2
                    }
                }
            },
            {
                .rowWeight = 10,
                .yPaddingWeight = 2,
                .rowItems =
                {
                    {
                        .component = &infoLine2,
                        .componentWeight = 10,
                        .xPaddingWeight = 2
                    }
                }
            }
        }
    });
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
