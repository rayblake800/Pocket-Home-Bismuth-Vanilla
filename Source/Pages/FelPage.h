/**
 * @File FelPage.h
 * 
 * FelPage gives the user the option to either reboot into Fel
 * (flashing) mode, or cancel and return to the last page.  If the system is
 * not a PocketCHIP or the i2c bus is otherwise inaccessible, nothing will
 * happen if the user clicks the confirmation button.
 */
#pragma once
#include "../Basic Components/ScalingLabel.h"
#include "../GridLayoutManager.h"

/**
 * TODO: buttonStateChanged changes button alpha on click, this would be better
 * handled in PokeLookAndFeel
 */
class FelPage : public Component, private Button::Listener {
public:
    FelPage();
    ~FelPage();
private:
    /**
     * Fills in the background color
     */
    void paint(Graphics &g) override;
    
    /**
     * Reposition child components to fit in the page.
     */
    void resized() override;
    
    /**
     * Change button alpha on click.
     */
    void buttonStateChanged(Button*) override;
    
    /**
     * Handle button clicks, either restarting into Fel mode or closing the 
     * page.
     * @param should be either &yesButton or &noButton
     */
    void buttonClicked(Button*) override;
    
    //Scales and positions child components to fit the page.
    GridLayoutManager layoutManager;
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
    bool debounce;
    //Fill color for the page background, currently fixed as Colours::black
    Colour bgColor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FelPage)
};
