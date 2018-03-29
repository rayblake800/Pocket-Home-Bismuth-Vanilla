/**
 * @File FelPage.h
 * 
 * FelPage gives the user the option to either reboot into Fel
 * (flashing) mode, or cancel and return to the last page.  If the system is
 * not a PocketCHIP or the i2c bus is otherwise inaccessible, nothing will
 * happen if the user clicks the confirmation button.
 */

#pragma once
#include "ScalingLabel.h"
#include "Localized.h"
#include "PageComponent.h"

class FelPage : public PageComponent, private Localized
{
public:
    FelPage();

    ~FelPage() { }
private:

    /**
     * Handle button clicks, either restarting into Fel mode or closing the 
     * page.
     * 
     * @param  This should be either &yesButton or &noButton
     */
    void pageButtonClicked(Button*) override;

    //Ask for confirmation
    ScalingLabel infoLine1;
    //Tell the user where to find flashing instructions.
    ScalingLabel infoLine2;
    //Confirm, and reboot into Fel mode
    TextButton yesButton;
    //Cancel, and close this page
    TextButton noButton;

    //If true, the yes button was clicked already and further clicks should
    //be ignored
    bool debounce = false;

    //localized text keys;
    static const constexpr char * ask_to_reboot = "ask_to_reboot";
    static const constexpr char * yes_btn = "yes_btn";
    static const constexpr char * no_btn = "no_btn";
    static const constexpr char * flashing_info = "flashing_info";

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FelPage)
};
