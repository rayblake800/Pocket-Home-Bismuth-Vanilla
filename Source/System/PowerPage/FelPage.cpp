#include "SystemCommands.h"
#include "I2CBus.h"
#include "FelPage.h"

/* Localized object class key: */
static const juce::Identifier localeClassKey = "FelPage";

/* localized text keys: */
static const juce::Identifier rebootTextKey       = "reboot";
static const juce::Identifier yesTextKey          = "yes";
static const juce::Identifier noTextKey           = "no";
static const juce::Identifier flashingInfoTextKey = "flashingInfo";

FelPage::FelPage() :
Locale::TextUser(localeClassKey),
PageComponent("FelPage"),
debounce(false),
infoLine1("infoLine1", localeText(rebootTextKey)),
yesButton(localeText(yesTextKey)),
noButton(localeText(noTextKey)),
infoLine2("infoLine2", localeText(flashingInfoTextKey))
{
    using namespace juce;
#    if JUCE_DEBUG
    setName("FelPage");
#    endif
    using namespace Layout::Group;
    RelativeLayout layout({
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
    if (button == &noButton)
    {
        removeFromStack(Layout::Transition::Type::moveRight);
    }
    else if (button == &yesButton && !debounce)
    {
        debounce = true;
        try
        {
            I2CBus i2c;
            i2c.enableFelMode();
            SystemCommands systemCommands;
            systemCommands.runActionCommand
                    (SystemCommands::ActionCommand::restart);
        }
        catch (I2CBus::I2CException e)
        {
            DBG("FelPage::" << __func__ << e.getErrorMessage());
        }
    }
}
