#include "PokeLookAndFeel.h"
#include "MainConfigFile.h"
#include "MainConfigKeys.h"
#include "I2CBus.h"
#include "FelPage.h"

/* localized text keys: */
static const constexpr char * ask_to_reboot = "ask_to_reboot";
static const constexpr char * yes_btn = "yes_btn";
static const constexpr char * no_btn = "no_btn";
static const constexpr char * flashing_info = "flashing_info";

FelPage::FelPage() :
Localized("FelPage"),
PageComponent("FelPage"),
debounce(false),
infoLine1("infoLine1", localeText(ask_to_reboot)),
yesButton(localeText(yes_btn)),
noButton(localeText(no_btn)),
infoLine2("infoLine2", localeText(flashing_info))
{
    using namespace juce;
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
    setLayout(layout);
                
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
void FelPage::pageButtonClicked(juce::Button* button)
{
    using namespace juce;
    if (button == &noButton)
    {
        removeFromStack(TransitionAnimator::moveRight);
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
                    (MainConfigKeys::restartCommandKey));
            c.waitForProcessToFinish(10000);
        }
        catch (I2CBus::I2CException e)
        {
            DBG("FelPage::" << __func__ << e.getErrorMessage());
        }
    }
}
