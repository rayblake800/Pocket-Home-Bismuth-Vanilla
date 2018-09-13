#pragma once
#include "ScalingLabel.h"
#include "Localized.h"
#include "PageComponent.h"

/**
 * @file FelPage.h
 * 
 * @brief  Assists the user in re-flashing the PocketCHIP.
 *
 * FelPage gives the user the option to either reboot into Fel
 * (flashing) mode, or cancel and return to the last page.  If the system is
 * not a PocketCHIP or the i2c bus is otherwise inaccessible, nothing will
 * happen if the user clicks the confirmation button.
 */


class FelPage : public PageComponent, private Localized
{
public:
    FelPage();

    ~FelPage() { }
private:

    /**
     * @brief  Handles button clicks, either restarting into Fel mode or closing
     *         the  page.
     * 
     * @param button  This should be either &yesButton or &noButton.
     */
    void pageButtonClicked(juce::Button* button) override;

    /* Ask for confirmation */
    ScalingLabel infoLine1;
    /* Tell the user where to find flashing instructions. */
    ScalingLabel infoLine2;
    /* Confirm, and reboot into Fel mode */
    juce::TextButton yesButton;
    /* Cancel, and close this page */
    juce::TextButton noButton;
    /* If true, the yes button was clicked already and further clicks should
       be ignored. */
    bool debounce = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FelPage)
};
