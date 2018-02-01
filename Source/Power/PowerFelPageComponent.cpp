#include "../Utils.h"
#include "../PokeLookAndFeel.h"
#include "../PocketHomeApplication.h"
#include "../I2CBus.h"
#include "PowerFelPageComponent.h"

PowerFelPageComponent::PowerFelPageComponent() :
bgColor(Colours::black),
debounce(false),
infoLine1("infoLine1", "Reboot into software flashing mode?"),
yesButton("Yes"),
noButton("No"),
infoLine2("infoLine2", "For instructions, visit pcflash.getchip.com")
{
    infoLine1.setJustificationType(Justification::centred);
    infoLine2.setJustificationType(Justification::centred);
    yesButton.addListener(this);
    noButton.addListener(this);

    std::vector<GridLayoutManager::ComponentLayoutParams> layoutParams = {
        {&infoLine1, 0, 1},
        {nullptr, 1, 1},
        {&yesButton, 1, 2},
        {nullptr, 1, 1},
        {nullptr, 2, 1},
        {nullptr, 3, 1},
        {&noButton, 3, 2},
        {nullptr, 3, 1},
        {nullptr, 4, 1},
        {&infoLine2, 5, 1},
    };
    layoutManager.addComponents(layoutParams, this);
    layoutManager.setRowWeight(0, 2);
    layoutManager.setRowWeight(1, 2);
    layoutManager.setRowWeight(3, 2);
}

PowerFelPageComponent::~PowerFelPageComponent()
{
}

/**
 * Fills in the background color
 */
void PowerFelPageComponent::paint(Graphics &g)
{
    g.fillAll(bgColor);
}

/**
 * Reposition child components to fit in the page.
 */
void PowerFelPageComponent::resized()
{
    Rectangle<int> bounds = getLocalBounds();
    bounds.reduce(bounds.getWidth() / 15, bounds.getHeight() / 15);
    layoutManager.layoutComponents(bounds, 0, 0);
}

/**
 * Change button alpha on click.
 */
void PowerFelPageComponent::buttonStateChanged(Button *btn)
{
    if (btn->isMouseButtonDown() && btn->isMouseOver())
    {
        btn->setAlpha(0.5f);
    } else
    {
        btn->setAlpha(1.0f);
    }
}

/**
 * Handle button clicks, either restarting into Fel mode or closing the 
 * page.
 * @param should be either &yesButton or &noButton
 */
void PowerFelPageComponent::buttonClicked(Button *button)
{
    if (button == &noButton)
    {
        PocketHomeApplication::getInstance()->getMainStack().popPage
                (PageStackComponent::kTransitionTranslateHorizontalLeft);
    } else if (button == &yesButton && !debounce)
    {
        debounce = true;
        try
        {
            I2CBus i2c;
            i2c.enableFelMode();
            ChildProcess c;
            c.start(PocketHomeApplication::getInstance()->getConfig()
                    .getConfigString(MainConfigFile::restartCommandKey));
            c.waitForProcessToFinish(10000);

        } catch (I2CBus::I2CException e)
        {
            DBG(e.getErrorMessage());
        }
    }
}