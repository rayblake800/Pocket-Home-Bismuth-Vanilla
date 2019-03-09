#include "Page_Fel.h"
#include "SystemCommands.h"
#include "I2CBus.h"

#ifdef JUCE_DEBUG
/* Print the full class name before all debug output: */
static const constexpr char* dbgPrefix = "Page::Fel::";
#endif

/* Localized object class key: */
static const juce::Identifier localeClassKey = "Page::Fel";

/* localized text keys: */
namespace TextKey
{
    static const juce::Identifier reboot       = "reboot";
    static const juce::Identifier yes          = "yes";
    static const juce::Identifier no           = "no";
    static const juce::Identifier flashingInfo = "flashingInfo";
}

Page::Fel::Fel() :
Locale::TextUser(localeClassKey),
pageListener(*this),
debounce(false),
infoLine1("infoLine1", localeText(TextKey::reboot)),
yesButton(localeText(TextKey::yes)),
noButton(localeText(TextKey::no)),
infoLine2("infoLine2", localeText(TextKey::flashingInfo))
{
#if JUCE_DEBUG
    setName("FelPage");
#endif
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
                
    infoLine1.setJustificationType(juce::Justification::centred);
    infoLine2.setJustificationType(juce::Justification::centred);
    yesButton.addListener(&pageListener);
    noButton.addListener(&pageListener);
    addAndShowLayoutComponents();
}

/*
 * Handles button click events for the Fel page.
 */
void Page::Fel::PageListener::buttonClicked(juce::Button* button)
{
    if (button == &felPage.noButton)
    {
        felPage.removeFromStack(Layout::Transition::Type::moveRight);
    }
    else if (button == &felPage.yesButton && !felPage.debounce)
    {
        felPage.debounce = true;
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
            DBG(dbgPrefix << __func__ << e.getErrorMessage());
        }
    }
}
