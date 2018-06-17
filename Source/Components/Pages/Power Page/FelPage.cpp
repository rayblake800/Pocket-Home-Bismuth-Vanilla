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
    using Row = LayoutManager::Row;
    using RowItem = LayoutManager::RowItem;
    LayoutManager::Layout layout({
        Row(10, { RowItem(&infoLine1) } ),
        Row(10,
        { 
            RowItem(),
            RowItem(&yesButton),
            RowItem()
        }),
        Row(10,
        { 
            RowItem(),
            RowItem(&noButton),
            RowItem()
        }),
        Row(10, { RowItem(&infoLine2) } )
    });
    layout.setXMarginFraction(0.1);
    layout.setYMarginFraction(0.05);
    layout.setYPaddingWeight(3);
    setLayout(layout, PageComponent::noBackButton);
                
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
